#ifndef PREPOZNAVANJEROKESUBSTRAKCIJA_H
#define PREPOZNAVANJEROKESUBSTRAKCIJA_H

//pomoc: https://picoledelimao.github.io/blog/2015/11/15/fingertip-detection-on-opencv/

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/video/background_segm.hpp>

#include <prepoznavanjeroke.h>

#define PI 3.1415926535897932384626433832795

cv::Mat narediBgSubstrakcijo(cv::VideoCapture kamera) {


}

//Funkcija za prepoznavanje prstov
void odpriKameroInPrepoznajPrsteHSVSub() {

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

    cv::namedWindow("Maska", 1);
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgsubtractor=cv::createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(30);

    while(true) {       //main loop v katerem prepoznavamo prste in jih izrisujemo na ekran
        cv::Mat frame;  //shranimo trenutni frame ki ga kamera zajame v spremenljivko frame
        kamera >> frame;

        //------------------------Predprocesiranje slike------------------------
        //Pretvorba v HSV
        cv::Mat hsv;    //zajeti framne pretvorimo v barvno shemo HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        cv::inRange(hsv, cv::Scalar(minH, minS, minV), cv::Scalar(maxH, maxS, maxV), hsv);

        //bgsubtractor->apply(frame, hsv, 0);

        //Dodajanje medianBlura
        cv::medianBlur(hsv, hsv, blurVelikost);
        cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                    cv::Size(2 * elementVelikost + 1, 2 * elementVelikost + 1),
                                                    cv::Point(elementVelikost, elementVelikost));
        cv::dilate(hsv, hsv, element);

        cv::imshow("Maska", hsv);

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

#endif // PREPOZNAVANJEROKESUBSTRAKCIJA_H
