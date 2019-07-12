#ifndef PREPOZNAVANJEROKEMODEL_H
#define PREPOZNAVANJEROKEMODEL_H

//pomoc: https://www.learnopencv.com/hand-keypoint-detection-using-deep-learning-and-opencv/

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/dnn/dnn.hpp>

const int POSE_PAIRS[20][2] =
{
    {0,1}, {1,2}, {2,3}, {3,4},         // thumb
    {0,5}, {5,6}, {6,7}, {7,8},         // index
    {0,9}, {9,10}, {10,11}, {11,12},    // middle
    {0,13}, {13,14}, {14,15}, {15,16},  // ring
    {0,17}, {17,18}, {18,19}, {19,20}   // small
};

std::string protoDatoteka = "pose_deploy.prototxt";
std::string modelDatotekka = "modelRoke.caffemodel";

std::string slikaRoke = "roka.jpg";

int steviloTockNaRoki = 22;

float tresh = 0.01;

void odpriKameroInPrepoznajPrsteModel() {

    //Nalozim sliko
    cv::Mat frame = cv::imread(slikaRoke, cv::IMREAD_ANYCOLOR);
    cv::Mat kopijaFrame = frame.clone();

    //Nalozimo model
    cv::dnn::Net mreza = cv::dnn::readNetFromCaffe(protoDatoteka, modelDatotekka);

    std::cout << "Sem nalozil sliko in model" << std::endl;

    //sirina in visina slike in izracunamo razmerje
    int sirinaSlike = frame.cols;
    int velikostSlike = frame.rows;

    float razmerje = sirinaSlike / (float)velikostSlike;

    int inVelikost = 368;
    int inSirina = (int(razmerje * inVelikost) * 8) / 8;


    //ustvarimo blob
    cv::Mat slikaBlob = cv::dnn::blobFromImage(frame, 1.0 / 255, cv::Size(inVelikost, inSirina), cv::Scalar(0, 0, 0), false, false);

    mreza.setInput(slikaBlob);

    cv::Mat izhodIzMreze = mreza.forward();

    std::cout << "Sem ustvaril blob" << std::endl;

    //iskanje tock na roki
    int V = izhodIzMreze.size[2];
    int S = izhodIzMreze.size[3];
    std::vector<cv::Point> tocke(steviloTockNaRoki);
    for(int i = 0; i < steviloTockNaRoki; i++) {
        cv::Mat probMat(V, S, CV_32F, izhodIzMreze.ptr(0, i));
        cv::resize(probMat, probMat, cv::Size(sirinaSlike, velikostSlike));

        cv::Point maxLock;
        double verjetnost;
        cv::minMaxLoc(probMat, 0, &verjetnost, 0, &maxLock);
        if (verjetnost > tresh) {
            cv::circle(kopijaFrame, cv::Point((int)maxLock.x, (int)maxLock.y), 8, cv::Scalar(255, 0, 0), -1);
            cv::putText(kopijaFrame, cv::format("%d", i), cv::Point((int)maxLock.x, (int)maxLock.y), CV_FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0,0), 2);
        }
        tocke.at(i) = maxLock;
    }

    std::cout << "Sem pred tem da prikazem sliko" << std::endl;

    cv::namedWindow("Prpeoznavanje prstov", cv::WINDOW_AUTOSIZE);

    cv::imshow("Prpeoznavanje prstov", kopijaFrame);

    cv::waitKey(0);
}

#endif // PREPOZNAVANJEROKEMODEL_H
