#ifndef ODPRIKAMERO_H
#define ODPRIKAMERO_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>

cv::Mat getHeatMap(cv::Mat input) // input is of type CV_8UC1, return is of type CV_8UC3
{
    cv::Mat result(input.rows, input.cols, CV_8UC3);
    for (int yy = 0; yy < input.rows; ++yy)
    {
        for (int xx = 0; xx < input.cols; ++xx)
        {
            int pixelValue = input.at<uchar>(yy, xx);
            if (pixelValue < 128) {
                result.at<cv::Vec3b>(yy, xx) = cv::Vec3b(0, 0 + 2*pixelValue, 255 - 2 * pixelValue);
            } else {
                result.at<cv::Vec3b>(yy, xx) = cv::Vec3b(0 + 2*pixelValue, 255 - 2 * pixelValue, 0);
            }
        }
    }
    return result;
}

void branjeZkamere() {
    cv::VideoCapture cap(0);

    bool update_bg_model = true;

    cv::Mat frames, bgmask, grey, heat;

    if (!cap.isOpened()) {
        std::cout << "Pri odpiranji kamere je prislo do napake!" << std::endl;
        return;
    }

    cv::namedWindow("Original", 1);
    cv::namedWindow("Maska", 1);
    cv::namedWindow("Heat", 1);

    cv::Ptr<cv::BackgroundSubtractorMOG2> bgsubtractor=cv::createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(10);
    bgsubtractor->setHistory(10);
    bgsubtractor->setDetectShadows(false);


    while(true) {
        cap >> frames;

        //cv::cvtColor(frames, edges, cv::COLOR_BGR2BGRA);
        cv::cvtColor(frames, grey, cv::COLOR_BGR2BGRA);

        heat = getHeatMap(grey);

        bgsubtractor->apply(frames, bgmask, 0);


        cv::imshow("Original", frames);
        cv::imshow("Maska", bgmask);
        cv::imshow("Heat", heat);


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
