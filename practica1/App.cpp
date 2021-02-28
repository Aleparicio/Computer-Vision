#include <iostream>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "EnhancedWindow.h"

#include "Filters.h"

void whiteNoise(int width, int height) {
    cv::Mat noise(height, width, CV_8UC1);
    cv::randn(noise, cv::Scalar(0), cv::Scalar(256));
    cv::waitKey(1);
}

void whiteImage(std::string winname, int width, int height) {
    cv::Mat white(height, width, CV_8UC1);
    white = cv::Scalar(255);
    cv::imshow(winname, white);
    cv::waitKey(1);
}

// https://answers.opencv.org/question/65222/is-there-a-way-to-keep-imwrite-from-overwriting-files/
const cv::String& imwriteSafe(const cv::String& filename, cv::InputArray img,
                              const std::vector<int>& params = std::vector<int>()) {

    static FILE* f;
    static cv::String newname;

    f = fopen(filename.c_str(), "rb");
    newname = filename;

    if (f) {
        int counter = 0;
        const int extension_dot = filename.find_first_of('.');
        cv::String name = filename.substr(0, extension_dot);
        cv::String extension = filename.substr(extension_dot);
        do {
            ++counter;
            newname = (name + "_" + std::to_string(counter) + extension);
            fclose(f);
            f = fopen(newname.c_str(), "rb");
        } while (f);
    }

    if (cv::imwrite(newname, img, params))
        return newname;
    else
        throw 0;
}

int main(int argc, char* argv[]) {

    bool useCamera;

    cv::VideoCapture cap;
    cv::Mat img;

    std::string WINDOW_NAME = "Filter - ";

    if (argc <= 1) {
        WINDOW_NAME += "Camera";
        useCamera = true;
        // Open camera
        if (!cap.open(0)) {
            std::cout << "Failed to connect to camera." << std::endl;
            return 1;
        }
    } else {
        WINDOW_NAME += argv[1];
        useCamera = false;
        img = cv::imread(argv[1], cv::IMREAD_COLOR);
        if (img.empty()) {
            std::cout << "Cannot open " << argv[1] << std::endl;
            return 1;
        }
    }

    std::shared_ptr<Filter> filter = nullptr;
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);

    EnhancedWindow mode(10, 50, 200, 340, "Filter");
    EnhancedWindow settings(1000, 50, 270, 180, "Settings");

    cvui::init(WINDOW_NAME);
    cv::Mat frame;
    cap >> frame;
    // cv::VideoWriter video("outcpp.avi",cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(frame.cols, frame.rows), true);

    while (true) {

        if (useCamera) {
            cap >> frame;
        } else {
            frame = img.clone();
        }

        if (frame.empty()) // end of video stream
            break;

        if (filter != nullptr) {
            filter->apply(frame);
        }

        int key = cv::waitKey(40);
        if (key == 'c') {
            // Capture image when c is pressed
            std::cout << "Written: " << imwriteSafe("image.png", frame) << std::endl;
            frame = cv::Scalar(255, 255, 255);
        } else if (key == 27 || key == 'q') {
            // Finish when ESC or q is pressed
            break;
        }

        if (filter != nullptr) {
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
            if (cvui::button(DuoTone::name)) {
                filter = std::make_shared<DuoTone>(1);
            }
            if (cvui::button(Twirl::name)) {
                filter = std::make_shared<Twirl>(frame.cols / 2, frame.rows / 2, frame.rows / 1.5, 2);
            }
        }
        mode.end();

        cvui::imshow(WINDOW_NAME, frame);
    }

    // video.release();
    return 0;
}
