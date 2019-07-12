#ifndef ODPRIKAMERO_H
#define ODPRIKAMERO_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>

void branjeZkamere() {
    cv::VideoCapture cap(0);

    bool update_bg_model = true;

    cv::Mat frames, bgmask, out_mask;

    if (!cap.isOpened()) {
        std::cout << "Pri odpiranji kamere je prislo do napake!" << std::endl;
        return;
    }

    cv::namedWindow("Original", 1);
    cv::namedWindow("Maska", 1);

    cv::Ptr<cv::BackgroundSubtractorMOG2> bgsubtractor=cv::createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(10);
    bgsubtractor->setHistory(10);
    bgsubtractor->setDetectShadows(false);


    while(true) {
        cap >> frames;

        //cv::cvtColor(frames, edges, cv::COLOR_BGR2BGRA);

        bgsubtractor->apply(frames, bgmask, 0);


        cv::imshow("Original", frames);
        cv::imshow("Maska", bgmask);


        char keycode = (char)cv::waitKey(30);
        if( keycode == 27 )
            break;
        if( keycode == ' ' )
        {
            update_bg_model = !update_bg_model;
            printf("Learn background is in state = %d\n",update_bg_model);
        }
    }
}

#endif // ODPRIKAMERO_H
