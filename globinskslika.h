#ifndef GLOBINSKSLIKA_H
#define GLOBINSKSLIKA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

void globinskaSlika() {
    cv::VideoCapture kamera1(0);
    cv::VideoCapture kamera2(1);

    kamera1.set(cv::CAP_PROP_FRAME_WIDTH, 80);
    kamera1.set(cv::CAP_PROP_FRAME_HEIGHT, 30);

    kamera2.set(cv::CAP_PROP_FRAME_WIDTH, 80);
    kamera2.set(cv::CAP_PROP_FRAME_HEIGHT, 30);

    cv::Mat frame1;
    cv::Mat frame2;

    cv::namedWindow("Kamera1", 1);
    cv::namedWindow("Kamera2", 1);

    while (true) {

        kamera1 >> frame1;

        kamera2 >> frame2;

        if (kamera1.isOpened()) {
            cv::imshow("Kamera1", frame1);
        }
        if (kamera2.isOpened()) {
            cv::imshow("Kamera2", frame2);
        }


        char keycode = (char)cv::waitKey(30);
        if( keycode == 27 )
            break;
    }
}

#endif // GLOBINSKSLIKA_H
