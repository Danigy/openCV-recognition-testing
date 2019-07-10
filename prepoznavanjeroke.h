#ifndef PREPOZNAVANJEROKE_H
#define PREPOZNAVANJEROKE_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>








//cv::CascadeClassifier prepoznajRoko("hand.xml");

//cv::VideoCapture cap(0);

//cv::Mat frames, edges, greyFrames;
//std::vector<cv::Rect> roka;
//void beriKameroInPrepoznajRoko() {
//    if (!cap.isOpened()) {
//        std::cout << "Pri odpiranju kamere je prislo do napake!" << std::endl;
//        return;
//    }
//    std::cout << "Prepoznavanje obraza se je zacelo.." << std::endl;

//    while(true) {
//        cap >> frames;

//        cv::cvtColor(frames, greyFrames, cv::COLOR_BGR2GRAY);

//        prepoznajRoko.detectMultiScale(greyFrames, roka);

//        for(unsigned int i = 0; i < roka.size(); i++) {
//            int x, y, w, h;
//            x = roka.at(i).x;
//            y = roka.at(i).y;
//            w = roka.at(i).x + roka.at(i).width;
//            h = roka.at(i).y + roka.at(i).height;
//            cv::rectangle(frames, cv::Point(x, y), cv::Point(w,h), cv::Scalar(255, 0, 0), 1);
//        }

//        cv::imshow("Prepoznavanje roke", frames);
//        if (cv::waitKey(30) >= 0) break;
//    }
//}

#endif // PREPOZNAVANJEROKE_H
