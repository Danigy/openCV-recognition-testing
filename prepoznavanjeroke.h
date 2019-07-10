#ifndef PREPOZNAVANJEROKE_H
#define PREPOZNAVANJEROKE_H

//pomoc: https://picoledelimao.github.io/blog/2015/11/15/fingertip-detection-on-opencv/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


void odpriKameroInPrepoznajPrste() {

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

        //------------------------Risanje na monitor------------------------
        cv::imshow(imeOkna, frame);     //prikazemo zajeti frame oz. obdelani frame na ekranu
        if(cv::waitKey(30) >= 0) break;
    }
}

#endif // PREPOZNAVANJEROKE_H
