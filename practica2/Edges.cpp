#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "vanishing_points.hpp"
#include "canny_operator.hpp"

int main(int argc, char* argv[]) {

    std::string image_path;

    // image_path = cv::samples::findFile("../../images/hendrix/poster.pgm");
    image_path = cv::samples::findFile("../../images/hendrix/pasillo2.pgm");
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
    /* std::vector<cv::Vec4i> linesP;
    cv::HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10);
    std::cout << "Hay " << linesP.size() << " líneas" << std::endl;
    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++) {
        cv::Vec4i l = linesP[i];
        line(cdstP, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }*/

    // Show results
    cv::imshow("Source", src);
    cv::imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    //cv::imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

    cv::waitKey(0); // Wait for a keystroke in the window
    cv::destroyAllWindows();

    return 0;
}


















































void aux(){
        /*
    // int ddepth = CV_16S;
    int ddepth = CV_32F;
    // int ddepth = CV_8U;
    int ksize = 3;
    int scale = 1;
    int delta = 0;

    cv::Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

    grad_x /= 4;
    grad_y /= 4;

    double min, max;
    cv::minMaxLoc(grad_x, &min, &max);
    std::cout << "grad_x: " << min << " " << max << std::endl;
    cv::minMaxLoc(grad_y, &min, &max);
    std::cout << "grad_y: " << min << " " << max << std::endl;

    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::Mat grad;
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    cv::imshow("Grad", grad);

    cv::imshow("Grad x abs", abs_grad_x);

    // grad_x /= 3;
    // grad_y /= 3;

    // grad_x = grad_x / 8;
    // grad_y = grad_y / 8;

    // cv::convertScaleAbs(grad_x, abs_grad_x, 0.5, 128);
    // cv::convertScaleAbs(grad_y, abs_grad_y, 0.5, 128);
    // abs_grad_x = (grad_x * 0.5) + 128;
    // cv::minMaxLoc(abs_grad_x, &min, &max);
    // std::cout << min << " " << max << std::endl;
    // cv::minMaxLoc(grad_y, &min, &max);
    // std::cout << min << " " << max << std::endl;

    grad_x.convertTo(abs_grad_x, CV_8U, 0.5, 128);
    grad_y.convertTo(abs_grad_y, CV_8U, 0.5, 128);

    // Invert y gradient
    // grad_y = -grad_y;

    // std::cout << abs_grad_x << std::endl;
    // std::cout << grad_x / 2 + 128 << std::endl;

    // cv::imshow("Grad x", grad_y / 2 + 128);
    // cv::imshow("Grad y", grad_y / 2 + 128);

    cv::imshow("Grad x", abs_grad_x);
    cv::imshow("Grad y", abs_grad_y);

    // Gradient magnitude
    cv::Mat mag, angle;
    // cv::sqrt(abs_grad_x * abs_grad_x + abs_grad_y * abs_grad_y, mag);
    // cv::sqrt(grad_x * grad_x + grad_y * grad_y, mag);
    // cv::magnitude(abs_grad_x, abs_grad_y, mag);
    // cv::magnitude(grad_x, grad_y, mag);

    // cv::cartToPolar(abs_grad_x, abs_grad_y, mag, angle);
    cv::cartToPolar(grad_x, grad_y, mag, angle);

    // cv::convertScaleAbs(mag, mag);
    mag.convertTo(mag, CV_8U);
    angle.convertTo(angle, CV_8U, 128 / CV_PI);
    cv::imshow("Magnitude", mag);
    cv::imshow("Angle", angle);

    // cv::Mat mag = grad_x * grad_x + grad_y * grad_y;
    // std::cout << grad_x << std::endl;
    // std::cout << mag.t() << std::endl;

    cv::imshow("Display window", img);
    cv::waitKey(0); // Wait por a keystroke in the window
    cv::destroyAllWindows();*/

}