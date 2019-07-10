#ifndef PREPOZNAVANJEROKE_H
#define PREPOZNAVANJEROKE_H

//pomoc: https://picoledelimao.github.io/blog/2015/11/15/fingertip-detection-on-opencv/

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/cvdef.h>

#define PI 3.1415926535897932384626433832795

//Pomagalna funkcija za prepoznavanje notranjih kotov med prsti
float notranjiKoti(float px1, float py1, float px2, float py2, float cx1, float cy1) {
    float Ax, Ay, Bx, By, Cx, Cy, Q1, Q2, P1, P2;

    float razdalja1 = std::sqrt( (px1 - cx1) * (px1 - cx1) * (py1 - cy1) * (py1 - cy1) );
    float razdalja2 = std::sqrt( (px2 - cx1) * (px2 - cx1) * (py2 - cy1) * (py2 - cy1) );

    if (razdalja1 < razdalja2) {
        Bx = px1;
        By = py1;
        Ax = px2;
        Ay = py2;
    } else {
        Bx = px2;
        By = py2;
        Ax = px1;
        Ay = py1;
    }

    Cx = cx1;
    Cy = cy1;
    Q1 = Cx - Ax;
    Q2 = Cy - Ay;
    P1 = Bx - Ax;
    P2 = By - Ay;

    float A = std::acos( (P1*Q1 + P2*Q2) / ( std::sqrt(P1*P1+P2*P2) * std::sqrt(Q1*Q1+Q2*Q2) ) );
    A = A * 180 / PI;

    return A;
}

//Funkcija za prepoznavanje prstov
void odpriKameroInPrepoznajPrsteHSV() {

    const std::string imeOkna = "Prepoznavanje roke in prstov";     //ime za okno

    cv::VideoCapture kamera(0);     //kamera

    //Heu, Saturation, Value
//    int minH = 0;
//    int maxH = 40;
//    int minS = 16;
//    int maxS = 255;
//    int minV = 127;
//    int maxV = 173;
    int minH = 0;
    int maxH = 43;
    int minS = 41;
    int maxS = 255;
    int minV = 29;
    int maxV = 202;

    //Nastavitev za medianBlur
    int blurVelikost = 5;
    int elementVelikost = 5;

    //Trackbari za fine tunnig heu, saturation in value
    cv::namedWindow(imeOkna);    //ustvarimo novo okno
    cv::createTrackbar("MinH", imeOkna, &minH, 180);
    cv::createTrackbar("MaxH", imeOkna, &maxH, 180);
    cv::createTrackbar("MinS", imeOkna, &minS, 255);
    cv::createTrackbar("MaxS", imeOkna, &maxS, 255);
    cv::createTrackbar("MinV", imeOkna, &minV, 255);
    cv::createTrackbar("MaxV", imeOkna, &maxV, 255);

    while(true) {       //main loop v katerem prepoznavamo prste in jih izrisujemo na ekran
        cv::Mat frame;  //shranimo trenutni frame ki ga kamera zajame v spremenljivko frame
        kamera >> frame;

        //------------------------Predprocesiranje slike------------------------
        //Pretvorba v HSV
        cv::Mat hsv;    //zajeti framne pretvorimo v barvno shemo HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        cv::inRange(hsv, cv::Scalar(minH, minS, minV), cv::Scalar(maxH, maxS, maxV), hsv);

        //Dodajanje medianBlura
        cv::medianBlur(hsv, hsv, blurVelikost);
        cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                    cv::Size(2 * elementVelikost + 1, 2 * elementVelikost + 1),
                                                    cv::Point(elementVelikost, elementVelikost));
        cv::dilate(hsv, hsv, element);

        //------------------------Prepoznavanje najvecje povrsine posameznega objekta------------------------
        std::vector<std::vector<cv::Point> > povrsine;      //vector poligonov
        std::vector<cv::Vec4i> hirarhija;       //neven zakaj ampak potrebno za findContours

        cv::findContours(hsv,           //iskanje vseh povrsin oz poligonov
                         povrsine,
                         hirarhija,
                         cv::RETR_EXTERNAL,     //zastavica saj iscemo le zunanje povrsine in ne povrsin znotraj njih
                         cv::CHAIN_APPROX_SIMPLE,
                         cv::Point(0,0));

        int najvecjaPovrsina = 0;   //poiscimo najvecjo povrisno
        for(unsigned int i = 1; i < povrsine.size(); i++) {
            if (cv::contourArea(povrsine.at(i)) > cv::contourArea(povrsine.at(najvecjaPovrsina))) {
                najvecjaPovrsina = i;
            }
        }

        //na frame narisemo povrsino
        cv::drawContours(frame, povrsine, najvecjaPovrsina, cv::Scalar(0, 0, 255), 2);

        //------------------------Konveksna lupina(convex hull)------------------------
        if (!povrsine.empty()) {    //ce najdemo kako povrsino poskusamo okrog nje naredit konveksno lupino
            std::vector<std::vector<cv::Point> > lupina(1);     //vector poligonov namenjen za lupino

            cv::convexHull(cv::Mat(povrsine.at(najvecjaPovrsina)), lupina.at(0), false);    //naredimo konveksno lupino
            //na frame narisemo konveksno lupino
            cv::drawContours(frame, lupina, 0, cv::Scalar(255, 0, 0), 3);

            //poiscemo notranje kote lupine
            if (lupina.at(0).size() > 2) {
                std::vector<int> indexiLupine;
                std::vector<cv::Vec4i> convexityDefects;

                cv::convexHull(cv::Mat(povrsine.at(najvecjaPovrsina)), indexiLupine, true);     //pridobimo notranje tocke konveksne lupine

                cv::convexityDefects(cv::Mat(povrsine.at(najvecjaPovrsina)), indexiLupine, convexityDefects);

                cv::Rect skatlaOkrogRoke = cv::boundingRect(lupina.at(0));
                cv::rectangle(frame, skatlaOkrogRoke, cv::Scalar(128, 127, 88), 1);

                cv::Point center = cv::Point((skatlaOkrogRoke.x + skatlaOkrogRoke.width) / 2,
                                             (skatlaOkrogRoke.y + skatlaOkrogRoke.height) / 2);

                std::vector<cv::Point> pravilneTocke;

                for (unsigned int i = 0; i < convexityDefects.size(); i++) {
                    cv::Point p1 = povrsine.at(najvecjaPovrsina).at(convexityDefects[i][0]);
                    cv::Point p2 = povrsine.at(najvecjaPovrsina).at(convexityDefects[i][1]);
                    cv::Point p3 = povrsine.at(najvecjaPovrsina).at(convexityDefects[i][2]);

                    cv::line(frame, p1, p3, cv::Scalar(0, 255, 0), 1.5);
                    cv::line(frame, p3, p2, cv::Scalar(0, 255, 0), 1.5);

                    double kot = std::atan2(center.y - p1.y, center.x - p1.x) * 180 / PI;
                    double notranjiKot = notranjiKoti(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
                    double dolzina = std::sqrt(std::pow(p1.x - p3.x, 2) + std::pow(p1.y - p3.y, 2));

                    if (kot > -30 && kot < 160 && std::abs(notranjiKot) > 20 && std::abs(notranjiKot) < 120 && dolzina > 0.1 * skatlaOkrogRoke.height) {
                        pravilneTocke.push_back(p1);
                    }
                }

                for (unsigned int i = 0; i < pravilneTocke.size(); i++) {
                    cv::circle(frame, pravilneTocke.at(i), 9, cv::Scalar(255, 0, 255), 3);
                }

                cv::putText(frame, "Stevilo prstov: " + std::to_string(pravilneTocke.size()), cv::Point(3, 100), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0), 2);
            }
        }

        //------------------------Risanje na monitor------------------------
        cv::imshow(imeOkna, frame);     //prikazemo zajeti frame oz. obdelani frame na ekranu
        if(cv::waitKey(30) >= 0) break;
    }
}

#endif // PREPOZNAVANJEROKE_H
