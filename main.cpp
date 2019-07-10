#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

cv::CascadeClassifier prepoznajObraz("haarcascade_frontalface_default.xml");
cv::CascadeClassifier prepoznajRoko("hand.xml");

cv::VideoCapture cap(0);

cv::Mat frames, edges, greyFrames;

std::vector<cv::Rect> obrazi;
std::vector<cv::Rect> roka;

void beriKameroInPrepoznajObraz() {
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

void beriKameroInPrepoznajRoko() {
    if (!cap.isOpened()) {
        std::cout << "Pri odpiranju kamere je prislo do napake!" << std::endl;
        return;
    }
    std::cout << "Prepoznavanje obraza se je zacelo.." << std::endl;

    while(true) {
        cap >> frames;

        cv::cvtColor(frames, greyFrames, cv::COLOR_BGR2GRAY);

        prepoznajRoko.detectMultiScale(greyFrames, roka);

        for(unsigned int i = 0; i < roka.size(); i++) {
            int x, y, w, h;
            x = roka.at(i).x;
            y = roka.at(i).y;
            w = roka.at(i).x + roka.at(i).width;
            h = roka.at(i).y + roka.at(i).height;
            cv::rectangle(frames, cv::Point(x, y), cv::Point(w,h), cv::Scalar(255, 0, 0), 1);
        }

        cv::imshow("Prepoznavanje roke", frames);
        if (cv::waitKey(30) >= 0) break;
    }
}

void branjeZkamere() {
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

void prikazSlike() {
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

int main()
{
    std::cout << "Hello World!" << std::endl;

    //prikazSlike();

    //branjeZkamere();

    //beriKameroInPrepoznajObraz();

    beriKameroInPrepoznajRoko();

    return 0;
}

