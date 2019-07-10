//Sistemske knjiznice
#include <iostream>

//OpenCv knjiznice
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

//Moje knjiznice
#include "prepoznavanjeroke.h"
#include "prepoznavanjeobraza.h"
#include "odprikamero.h"
#include "odprisliko.h"

int main()
{
    int izbira = 0;

    do {
        std::system("clear");
        std::cout << "Zacetek testiranja programa!" << std::endl;

        std::cout << "\nIzberite nekaj od naslednjega:\n" << std::endl;
        std::cout << "1.) Odpri sliko," << std::endl;
        std::cout << "2.) Odpri kamero," << std::endl;
        std::cout << "3.) Odpri kamero in zacni z prepoznavanje obraza," << std::endl;
        std::cout << "4.) Odpri kamero in zacni z prepoznavanjem prstov(HSV nacin)," << std::endl;
        std::cout << "5.) Odpri kamero in zacni z prepoznavanjem prstov(Haarcascade + HSV nacin)," << std::endl;
        std::cout << "10.) Izhod iz programa.\n" << std::endl;
        std::cout << "Vasa izbira je: ";
        std::cin >> izbira;

        switch (izbira) {
        case 1:
            odpriInPrikaziSliko();
            break;
        case 2:
            branjeZkamere();
            break;
        case 3:
            odpriKameroInPrepoznajObraz();
            break;
        case 4:
            odpriKameroInPrepoznajPrsteHSV();
            break;
        case 5:
            break;
        case 10:
            std::cout << "Konec testiranja" << std::endl;
            break;
        default:
            std::cout << "Napacna vrednost!" << std::endl;
            break;
        }

    } while (izbira != 10);

    return 0;
}

