#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "canny_operator.hpp"

int main(int argc, char* argv[]) {
    std::string image_path;
    std::string kernel;

    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " operator image"
                  << std::endl
                  << std::endl;
        std::cout << "  operator: SOBEL, SCHARR or CANNY" << std::endl;
        std::cout << "  image: image path" << std::endl;
        return 1;
    } else {
        kernel = argv[1];
        image_path = argv[2];
    }

    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    cv::Mat src, src_gray;
    src = img.clone();

    // Convert the image to grayscale
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    cv::imshow("Original image", src_gray);

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    // Calculate gradient
    if (kernel == "SOBEL") {
        // Apply gaussian filter
        cv::GaussianBlur(src_gray, src_gray, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

        int ddepth = CV_32F;
        int ksize = 3;
        int scale = 1;
        int delta = 0;

        cv::Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
        cv::Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

        // grad_x /= 4;
        // grad_y /= 4;
        cv::normalize(grad_x, grad_x, -255, 255, cv::NORM_MINMAX);
        cv::normalize(grad_y, grad_y, -255, 255, cv::NORM_MINMAX);

    } else if (kernel == "SCHARR") {
        int ddepth = CV_32F;
        int ksize = 3;
        int scale = 1;
        int delta = 0;

        cv::Mat dst;

        cv::Scharr(src_gray, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT);
        cv::Scharr(src_gray, grad_y, ddepth, 0, 1, scale, delta, cv::BORDER_DEFAULT);

        // grad_x /= 16;
        // grad_y /= 16;
        cv::normalize(grad_x, grad_x, -255, 255, cv::NORM_MINMAX);
        cv::normalize(grad_y, grad_y, -255, 255, cv::NORM_MINMAX);

    } else if (kernel == "CANNY") {
        float sigma = 1;
        int ksize = 5;
        CannyGradient(src_gray, grad_x, grad_y, sigma, ksize);
        cv::Mat abs_grad_x, abs_grad_y;
    } else {
        std::cout << "Unknown kernel: " << kernel << std::endl;
        return 1;
    }

    // Max and min values of the gradientes
    double min, max;
    cv::minMaxLoc(grad_x, &min, &max);
    std::cout << "Gradient X: " << min << " " << max << std::endl;
    cv::minMaxLoc(grad_y, &min, &max);
    std::cout << "Gradient Y: " << min << " " << max << std::endl;

    cv::convertScaleAbs(grad_x, abs_grad_x, 0.5, 128);
    cv::convertScaleAbs(grad_y, abs_grad_y, 0.5, 128);

    cv::imshow(kernel + " Gradient in x direction", abs_grad_x);
    cv::imshow(kernel + " Gradient in y direction", abs_grad_y);

    // Gradient magnitude and angle
    grad_x.convertTo(grad_x, CV_32F);
    grad_y.convertTo(grad_y, CV_32F);

    cv::Mat mag, angle;
    cv::cartToPolar(grad_x, grad_y, mag, angle);

    mag.convertTo(mag, CV_8U);
    angle.convertTo(angle, CV_8U, 128 / CV_PI);
    cv::imshow(kernel + " Gradient magnitude", mag);
    cv::imshow(kernel + " Gradient orientation", angle);

    // Get edges using Canny gradient maxima
    grad_x.convertTo(grad_x, CV_16S);
    grad_y.convertTo(grad_y, CV_16S);
    cv::Mat edges;
    cv::Canny(grad_x, grad_y, edges, 10, 100);

    cv::imshow(kernel + " Edges", edges);

    cv::waitKey(0); // Wait por a keystroke in the window

    // Calculate lines with Hough transform

    // Copy edges to the images that will display the results in BGR
    cv::Mat cdst, cdstP;
    cv::cvtColor(edges, cdst, cv::COLOR_GRAY2BGR);

    // Standard Hough Line Transform
    std::vector<cv::Vec2f> lines;                            // will hold the results of the detection
    cv::HoughLines(edges, lines, 1, CV_PI / 180, 100, 0, 0); // Run the detection
    std::cout << "Number of lines: " << lines.size() << std::endl;
    // Draw the lines
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        cv::line(cdst, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }
    // Show results
    cv::imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);

    // // Probabilistic Line Transform
    // std::vector<cv::Vec4i> linesP;
    // cv::HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10);
    // std::cout << "Hay " << linesP.size() << " líneas" << std::endl;
    // // Draw the lines
    // for (size_t i = 0; i < linesP.size(); i++) {
    //     cv::Vec4i l = linesP[i];
    //     line(cdstP, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    // }
    // Show results
    // cv::imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

    cv::waitKey(0); // Wait for a keystroke in the window

    return 0;
}
