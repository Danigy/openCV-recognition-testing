#ifndef ODPRIKAMERO_H
#define ODPRIKAMERO_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

void branjeZkamere() {
    cv::VideoCapture cap(0);

    cv::Mat frames, edges;

    if (!cap.isOpened()) {
        std::cout << "Pri odpiranji kamere je prislo do napake!" << std::endl;
        return;
    }

    cv::namedWindow("Prikaz z kamere", 1);
    while(true) {
        cap >> frames;
        cv::cvtColor(frames, edges, cv::COLOR_BGR2BGRA);

        //cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
        //cv::Canny(edges, edges, 0, 30, 3);
        cv::imshow("Prikaz z kamere", edges);
        if (cv::waitKey(30) >= 0) break;
    }
}

#endif // ODPRIKAMERO_H
