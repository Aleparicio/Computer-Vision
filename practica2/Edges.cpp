#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#define CVUI_IMPLEMENTATION
#include "cvui.h"
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

    bool rotate = true;
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

        rotate = true;
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

        cv::rotate(frame, frame, cv::ROTATE_90_CLOCKWISE);
        //frame = getVanishingPoints(frame);

        int key = cv::waitKey(40);
        if (key == 'c') {
            // Capture image when c is pressed
            std::cout << "Written: " << imwriteSafe("image.png", frame) << std::endl;
            frame = cv::Scalar(255, 255, 255);
        } else if (key == 27 || key == 'q') {
            // Finish when ESC or q is pressed
            break;
        }
        
        cv::imshow( "Frame", frame );
    }

    cap.release();
    return 0;
}











/*int main(int argc, char* argv[]) {

    std::string image_path;

    image_path = cv::samples::findFile("../../images/hendrix/poster.pgm");
    // image_path = cv::samples::findFile("../../images/hendrix/pasillo2.pgm");
    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

    if (img.empty()) {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    cv::Mat src, src_gray;
    src = img.clone();
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    cv::imshow("Imagen", src_gray);
    
    ///////////////////
    // Canny Operator
    ///////////////////

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Mat dst;

    CannyGradient(src_gray, grad_x, grad_y, 1.4, 5);
    grad_x.convertTo(abs_grad_x, CV_8U, 0.5, 128);
    grad_y.convertTo(abs_grad_y, CV_8U, 0.5, 128);
    imshow("Canny gradient x", abs_grad_x);
    imshow("Canny gradient y", abs_grad_y);    
    cv::Canny(grad_x, grad_y, dst, 6, 15, true);
    imshow("Canny edges gaussian", dst);

    cv::waitKey(0);
    cv::destroyAllWindows();


    ////////////////////
    // Hough transform
    ////////////////////
    cv::Mat cdst, cdstP;
    cv::Vec2i punto_fuga(0,0);
    cv::cvtColor(dst, cdst, cv::COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    // Standard Hough Line Transform
    std::vector<cv::Vec2f> lines;                    
    std::vector<cv::Vec4f> flines; 
    cv::HoughLines(dst, lines, 1, CV_PI / 180, 85, 0, 0, 0.0, CV_PI / 2.0); // Run the detection
    std::cout << "Hay " << lines.size() << " líneas" << std::endl;
    filterLinesAndVote(lines, dst, flines, punto_fuga);
    draw2Dlines(cdst, flines, punto_fuga);
    std::cout << "Hay " << flines.size() << " líneas" << std::endl;

    // Probabilistic Line Transform
     std::vector<cv::Vec4i> linesP;
    cv::HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10);
    std::cout << "Hay " << linesP.size() << " líneas" << std::endl;
    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++) {
        cv::Vec4i l = linesP[i];
        line(cdstP, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    cv::imshow("Source", src);
    cv::imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    //cv::imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

    cv::waitKey(0); // Wait for a keystroke in the window
    cv::destroyAllWindows();

    return 0;
}*/






























