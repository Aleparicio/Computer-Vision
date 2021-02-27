#include <iomanip>
#include <iostream>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "EnhancedWindow.h"

#include "Filters.h"

using namespace std;

const std::string WINDOW_NAME = "Filter";

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
    cv::Mat noise(height, width, CV_8UC1);
    cv::randn(noise, cv::Scalar(0), cv::Scalar(256));
    cv::imshow(winname, noise);
    cv::waitKey(1);
}

void whiteImage(std::string winname, int width, int height) {
    cv::Mat white(height, width, CV_8UC1);
    white = cv::Scalar(255);
    cv::imshow(winname, white);
    cv::waitKey(1);
}

int processVideo(cv::VideoCapture& cap, const std::shared_ptr<Filter> filter) {

    while (true) {

        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) // end of video stream
            break;

        filter->apply(frame);

        cv::imshow(WINDOW_NAME, frame);
        if (cv::waitKey(10) >= 0) // stop capturing when key is pressed
            break;
    }

    cv::destroyAllWindows();
    return 0;
}

/*
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
    // whiteNoise(window_name, cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH));

    cv::Mat frame = cv::Mat(2000, 3000, CV_8UC3);

    EnhancedWindow mode(10, 50, 270, 180, "Mode");
    EnhancedWindow settings(100, 500, 270, 180, "Settings");

    cvui::init(window_name);

    cv::Mat camera;

    // Contrast f(window_name);
    double value = 12.4;

    bool use_canny = false;
    int low_threshold = 50, high_threshold = 50;

    while (true) {
        frame = cv::Scalar(100, 100, 100);

        cvui::text(frame, 110, 80, "Hello, world!");
        cvui::text(frame, 110, 120, "cvui is awesome!");

        // std::cout << value << std::endl;

        cap >> camera;

        // cv::Mat flor = cv::imread("flor.png", cv::IMREAD_COLOR);
        // cvui::image(frame, 10, 10, flor);

        cvui::text(frame, 10, 15, "Hola");

        // cvui::trackbar(frame, 40, 30, 220, &value, (double)10.0, (double)15.0);

        // cvui::window(camera, 60, 10, 130, 90, "Title");

        bool contraste, alien, poster, distorsion = false;

        if (cvui::button(camera, 100, 40, "Button")) {
            std::cout << "click" << std::endl;
        }

        if (use_canny) {
            settings.begin(camera);
            if (!settings.isMinimized()) {
                cvui::checkbox("Use Canny Edge", &use_canny);
                cvui::trackbar(settings.width() - 20, &low_threshold, 5, 150);
                cvui::trackbar(settings.width() - 20, &high_threshold, 80, 300);
                cvui::space(20); // add 20px of empty space
                cvui::text("Drag and minimize this settings window", 0.4, 0xff0000);
            }
            settings.end();
        }

        mode.begin(camera);
        if (!mode.isMinimized()) {
            // cvui::checkbox("Contraste", &contraste);
            // cvui::checkbox("Alien", &alien);
            // cvui::checkbox("Poster", &poster);
            // cvui::checkbox("Distorsion", &distorsion);
            if (cvui::button("Contraste")) {
                // button was clicked
                std::cout << "contraste" << std::endl;
            }
            if (cvui::button("Alien")) {
                // button was clicked
                std::cout << "alien" << std::endl;
            }
        }
        mode.end();

        // cvui::image(frame, 0, 0, camera);

        // cvui::image(camera, 200, 0, camera);

        // float valor = 2;
        // // cvui::trackbar(frame, 100, 2000, 200, &valor, -5.0f, 5.0f);
        // cvui::trackbar(frame, 100, 2000, 200, &valor, -5.0f, 5.0f);

        cvui::trackbar(camera, 40, 30, 220, &value, (double)10.0, (double)15.0);
        cvui::imshow(window_name, camera);

        if (cv::waitKey(20) == 27) {
            break;
        }
    }

    // do {
    //     options();
    //     std::cin >> operation;

    //     if (operation == 1) {
    //         // Contraste and histogram equalization
    //         optionsEqContrast();
    //         std::cin >> operation;

    //         if (operation == 1) {
    //             // cout << "Introduce ganancia: ";
    //             // float ganancia = 0.0, sesgo = 0.0;
    //             // std::cin >> ganancia;
    //             // cout << "Introduce sesgo: ";
    //             // std::cin >> sesgo;
    //             filter = std::make_shared<Contrast>(window_name);
    //         } else if (operation == 2) {
    //             filter = std::make_shared<HistogramEqualization>(window_name);
    //         } else if (operation == 3) {
    //             filter = std::make_shared<CLAHE>(window_name);
    //         } else {
    //             continue;
    //         }
    //     } else if (operation == 2) {
    //         // Alien
    //         filter = std::make_shared<Alien>(window_name);
    //     } else if (operation == 3) {
    //         // Poster
    //         filter = std::make_shared<Posterization>(window_name);
    //     } else if (operation == 4) {
    //         // Distortion
    //         filter = std::make_shared<Distortion>(window_name);
    //     } else {
    //         continue;
    //     }

    //     processVideo(cap, filter);

    //     whiteNoise(window_name, cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH));

    //     options();

    // } while (operation != 0);

    return 0;
}
*/

int main() {

    // Open camera
    cv::VideoCapture cap;
    if (!cap.open(0)) {
        std::cout << "Failed to connect to camera." << std::endl;
        return 1;
    }

    std::shared_ptr<Filter> filter = nullptr;
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    // whiteNoise(window_name, cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH));

    EnhancedWindow mode(10, 50, 200, 340, "Filter");
    EnhancedWindow settings(1000, 50, 270, 180, "Settings");

    cvui::init(WINDOW_NAME);

    cv::Mat frame;

    while (true) {

        cap >> frame;

        // frame = cv::imread("../flor.jpg", cv::IMREAD_COLOR);
        // frame = cv::imread("../lenna.png", cv::IMREAD_COLOR);
        // frame = cv::imread("../garden.jpg", cv::IMREAD_COLOR);

        if (frame.empty()) // end of video stream
            break;

        if (filter != nullptr) {
            filter->apply(frame);
            filter->showSettings(settings, frame);
        }

        // Select filter
        mode.begin(frame);
        if (!mode.isMinimized()) {
            if (cvui::button("None")) {
                filter = nullptr;
            }
            if (cvui::button(Contrast::name)) {
                filter = std::make_shared<Contrast>(1, 1);
            }
            if (cvui::button(HistogramEqualization::name)) {
                filter = std::make_shared<HistogramEqualization>();
            }
            if (cvui::button(CLAHE::name)) {
                filter = std::make_shared<CLAHE>(1);
            }
            if (cvui::button(Posterization::name)) {
                filter = std::make_shared<Posterization>(5);
            }
            if (cvui::button(Alien::name)) {
                filter = std::make_shared<Alien>(0, 255, 0, 100);
            }
            if (cvui::button(Distortion::name)) {
                filter = std::make_shared<Distortion>(0, 0);
            }
            if (cvui::button(Thresholding::name)) {
                filter = std::make_shared<Thresholding>(120);
            }
            if (cvui::button(Twirl::name)) {
                filter = std::make_shared<Twirl>(frame.cols / 2, frame.rows / 2, frame.rows / 1.5, 2);
            }
        }
        mode.end();

        cvui::imshow(WINDOW_NAME, frame);

        // Finish when ESC is pressed
        if (cv::waitKey(40) == 27) {
            break;
        }
    }

    return 0;
}
