#ifndef ODPRISLIKO_H
#define ODPRISLIKO_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

void odpriInPrikaziSliko() {
    cv::Mat image;
    image = cv::imread("slika.jpg", cv::IMREAD_ANYCOLOR);

    if(!image.data) {
        std::cout << "Napaka pri iskanju slike!" << std::endl;
        return;
    }

    cv::namedWindow("Prikaz slike", cv::WINDOW_AUTOSIZE);
    cv::imshow("Prikaz slike", image);

    cv::waitKey(0);
}

#endif // ODPRISLIKO_H
