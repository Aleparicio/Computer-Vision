#include <iomanip>
#include <iostream>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Filters.h"

using namespace std;

const std::string window_name = "Filter";

void options() {
    cout << endl
         << endl
         << " SELECCIONE EL FILTRO A EMPLEAR" << endl;
    cout << " ===================================================" << endl;
    cout << "   Filtro a aplicar (0 para terminar):" << endl;
    cout << "   1.- Contraste y/o equalización del histograma" << endl;
    cout << "   2.- Alien " << endl;
    cout << "   3.- Póster " << endl;
    cout << "   4.- Distorsión " << endl;
    cout << "Para terminar la aplicación de un filtro pulsar cualquier tecla." << endl;
    cout << "Seleccionar filtro: " << flush;
}

void optionsEqContrast() {
    cout << endl
         << endl
         << " SELECCIONAR SUBTIPO (0 para terminar)" << endl;
    cout << " =================================================" << endl;
    cout << "   1 - Ampliación contraste" << endl;
    cout << "   2 - Equalización Histograma " << endl;
    cout << "   3 - CLAHE " << endl;
    cout << "Para terminar la aplicación de un filtro pulsar cualquier tecla." << endl;
    cout << "Seleccionar filtro: " << flush;
}

void whiteNoise(std::string winname, int width, int height) {
    cv::Mat noise(width, height, CV_8UC1);
    cv::randn(noise, cv::Scalar(0), cv::Scalar(256));
    cv::imshow(winname, noise);
    cv::waitKey(1);
}

void whiteImage(std::string winname, int width, int height) {
    cv::Mat noise(width, height, CV_8UC1);
    cv::randn(noise, cv::Scalar(0), cv::Scalar(256));
    cv::imshow(winname, noise);
    cv::waitKey(1);
}

int processVideo(cv::VideoCapture& cap, const std::shared_ptr<Filter> filter) {

    while (true) {

        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) // end of video stream
            break;

        filter->apply(frame);

        cv::imshow(window_name, frame);
        if (cv::waitKey(10) >= 0) // stop capturing when key is pressed
            break;
    }

    cv::destroyAllWindows();
    return 0;
}

int main() {

    int operation;
    std::shared_ptr<Filter> filter;

    // Open camera
    cv::VideoCapture cap;
    if (!cap.open(0)) {
        std::cout << "Failed to connect to camera." << std::endl;
        return 1;
    }

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    whiteNoise(window_name, cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH));

    do {
        options();
        std::cin >> operation;

        if (operation == 1) {
            // Contraste and histogram equalization
            optionsEqContrast();
            std::cin >> operation;

            if (operation == 1) {
                // cout << "Introduce ganancia: ";
                // float ganancia = 0.0, sesgo = 0.0;
                // std::cin >> ganancia;
                // cout << "Introduce sesgo: ";
                // std::cin >> sesgo;
                filter = std::make_shared<Contrast>(window_name);
            } else if (operation == 2) {
                filter = std::make_shared<HistogramEqualization>(window_name);
            } else if (operation == 3) {
                filter = std::make_shared<CLAHE>(window_name);
            } else {
                continue;
            }
        } else if (operation == 2) {
            // Alien
            filter = std::make_shared<Alien>(window_name);
        } else if (operation == 3) {
            // Poster
            filter = std::make_shared<Posterization>(window_name);
        } else if (operation == 4) {
            // Distortion
            filter = std::make_shared<Distortion>(window_name);
        } else {
            continue;
        }

        processVideo(cap, filter);

        whiteNoise(window_name, cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH));

        options();

    } while (operation != 0);

    return 0;
}
