#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include "vanishing_points.hpp"
#include "canny_operator.hpp"

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

    bool rotate = false;
    cv::VideoCapture cap;
    cv::Mat img;
    std::string WINDOW_NAME = "Vanishing point - ";

    if (argc <= 1) {
        rotate = false;
        WINDOW_NAME += "Camera";
        cap.open(0);
        if (!cap.isOpened()) {
            std::cout << "Failed to connect to camera." << std::endl;
            return 1;
        }
    } else {
        rotate = false;
        WINDOW_NAME += argv[1];
        cap = cv::VideoCapture(argv[1]);
        if (!cap.isOpened()) {
            std::cout << "Cannot open " << argv[1] << std::endl;
            return 1;
        }
    }

    cv::Mat frame;
    cap >> frame;
    // cv::VideoWriter video("outcpp.avi",cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(frame.cols, frame.rows), true);

    while (true) {

        cap >> frame;
        if (frame.empty()) // end of video stream
            break;
        if (rotate)
            cv::rotate(frame, frame, cv::ROTATE_90_CLOCKWISE);
        frame = getVanishingPoints(frame);

        int key = cv::waitKey(40);
        if (key == 'c') {
            // Capture image when c is pressed
            std::cout << "Written: " << imwriteSafe("image.png", frame) << std::endl;
            frame = cv::Scalar(255, 255, 255);
        } else if (key == 27 || key == 'q') {
            // Finish when ESC or q is pressed
            break;
        }

        cv::imshow("Frame", frame);
    }

    cap.release();
    return 0;
}

// image_path = cv::samples::findFile("../../images/hendrix/poster.pgm");
// image_path = cv::samples::findFile("../../images/hendrix/pasillo2.pgm");
