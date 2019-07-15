#ifndef PREPOZNAVANJEROKEHAAR_H
#define PREPOZNAVANJEROKEHAAR_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

void odpriKameroInPrepoznajPrsteHaar() {
    cv::CascadeClassifier prepoznajRoko("roka.xml");
    cv::VideoCapture cap(0);

    cv::Mat frames, greyFrames;

    std::vector<cv::Rect> hands;

    if (!cap.isOpened()) {
        std::cout << "Pri odpiranju kamere je prislo do napake!" << std::endl;
        return;
    }
    std::cout << "Prepoznavanje obraza se je zacelo.." << std::endl;

    while(true) {
        cap >> frames;

        cv::cvtColor(frames, greyFrames, cv::COLOR_BGR2GRAY);

        prepoznajRoko.detectMultiScale(greyFrames, hands, 1.1, 5);

        for(unsigned int i = 0; i < hands.size(); i++) {
            int x, y, w, h;
            x = hands.at(i).x;
            y = hands.at(i).y;
            w = hands.at(i).x + hands.at(i).width;
            h = hands.at(i).y + hands.at(i).height;
            cv::rectangle(frames, cv::Point(x, y), cv::Point(w, h), cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Prepoznavanje obraza", frames);
        if (cv::waitKey(30) >= 0) break;
    }
}

#endif // PREPOZNAVANJEROKEHAAR_H
