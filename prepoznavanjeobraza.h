#ifndef PREPOZNAVANJEOBRAZA_H
#define PREPOZNAVANJEOBRAZA_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

void odpriKameroInPrepoznajObraz() {
    cv::CascadeClassifier prepoznajObraz("haarcascade_frontalface_default.xml");
    cv::VideoCapture cap(0);

    cv::Mat frames, greyFrames;

    std::vector<cv::Rect> obrazi;

    if (!cap.isOpened()) {
        std::cout << "Pri odpiranju kamere je prislo do napake!" << std::endl;
        return;
    }
    std::cout << "Prepoznavanje obraza se je zacelo.." << std::endl;

    while(true) {
        cap >> frames;

        cv::cvtColor(frames, greyFrames, cv::COLOR_BGR2GRAY);

        prepoznajObraz.detectMultiScale(greyFrames, obrazi);

        for(unsigned int i = 0; i < obrazi.size(); i++) {
            int x, y, w, h;
            x = obrazi.at(i).x;
            y = obrazi.at(i).y;
            w = obrazi.at(i).x + obrazi.at(i).width;
            h = obrazi.at(i).y + obrazi.at(i).height;
            cv::rectangle(frames, cv::Point(x, y), cv::Point(w, h), cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Prepoznavanje obraza", frames);
        if (cv::waitKey(30) >= 0) break;
    }
}

#endif // PREPOZNAVANJEOBRAZA_H
