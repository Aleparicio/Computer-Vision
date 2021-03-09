#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "canny_operator.hpp"

void apply2(cv::Mat& imagen, int size = 5, float sigma = 1.4f);

int main(int argc, char* argv[]) {

    std::string image_path;

    // image_path = cv::samples::findFile("../../images/flor.png");
    // image_path = cv::samples::findFile("../../images/garden.jpg");
    // image_path = cv::samples::findFile("../../images/cuadro.png");
    // image_path = cv::samples::findFile("../../images/pasillo.png");
    // image_path = cv::samples::findFile("../../images/lena.png");
    // image_path = cv::samples::findFile("../../images/hendrix/poster.pgm");
    image_path = cv::samples::findFile("../../images/hendrix/pasillo2.pgm");

    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

    if (img.empty()) {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    cv::Mat src, src_gray;

    // cv::GaussianBlur(img, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
    src = img.clone();

    // Convert the image to grayscale
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    ////////////////////
    // Sobel
    ////////////////////

    cv::imshow("Imagen", src_gray);

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

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
    cv::destroyAllWindows();

    ///////////////////
    // Canny Operator
    ///////////////////

    cv::Mat dst;
    cv::Canny(src_gray, dst, 10, 20, 3);
    imshow("Canny edges sobel", dst);

    CannyGradient(src_gray, grad_x, grad_y, 1.2, 5);
    grad_x.convertTo(abs_grad_x, CV_8U, 0.5, 128);
    grad_y.convertTo(abs_grad_y, CV_8U, 0.5, 128);
    imshow("Canny gradient x", abs_grad_x);
    imshow("Canny gradient y", abs_grad_y);
    cv::Canny(grad_x, grad_y, dst, 10, 20);
    imshow("Canny edges gaussian", dst);

    cv::waitKey(0);
    cv::destroyAllWindows();

    ////////////////////
    // Hough transform
    ////////////////////

    // Edge detection with canny operator
    // Copy edges to the images that will display the results in BGR
    cv::Mat cdst, cdstP;
    cv::cvtColor(dst, cdst, cv::COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    // Standard Hough Line Transform
    std::vector<cv::Vec2f> lines;                          // will hold the results of the detection
    cv::HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0); // Run the detection
    std::cout << "Hay " << lines.size() << " líneas" << std::endl;
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

    // Probabilistic Line Transform
    std::vector<cv::Vec4i> linesP;
    cv::HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10);
    std::cout << "Hay " << linesP.size() << " líneas" << std::endl;
    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++) {
        cv::Vec4i l = linesP[i];
        line(cdstP, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    // Show results
    cv::imshow("Source", src);
    cv::imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    cv::imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

    cv::waitKey(0); // Wait for a keystroke in the window
    cv::destroyAllWindows();

    return 0;
}

// void apply2(cv::Mat& imagen, int size, float sigma) {

//     cv::imshow("imagen inicial", imagen);

//     std::cout << "\n\nAntes conversion " << imagen.depth() << " " << imagen.channels() << std::endl;
//     imagen.convertTo(imagen, CV_32F);
//     std::cout << "Después conversion " << imagen.depth() << " " << imagen.channels() << std::endl;
//     // imagen = imagen / 255;

// // #define SOBEL
// // Todo entre 0 y 1 aquí
// #ifndef SOBEL
//     /*
//     std::cout << "hola" << std::endl;
//     cv::Mat firstKernel = cv::Mat::zeros(1, 5, CV_32F);
//     firstKernel.at<float>(0, 0) = -0.2707;
//     firstKernel.at<float>(0, 1) = -0.6065;
//     firstKernel.at<float>(0, 2) = 0.0;
//     firstKernel.at<float>(0, 3) = 0.6065;
//     firstKernel.at<float>(0, 4) = 0.2707;

//     cv::Mat secKernel = cv::Mat::zeros(1, 5, CV_32F);
//     secKernel.at<float>(0, 0) = 0.1353;
//     secKernel.at<float>(0, 1) = 0.6065;
//     secKernel.at<float>(0, 2) = 1.0;
//     secKernel.at<float>(0, 3) = 0.6065;
//     secKernel.at<float>(0, 4) = 0.1353;

//     float k1 = 0.6065 + 0.2707;
//     float k2 = 1.0 + 20.1353 + 20.6065;
//     float k = k1 * k2;
//     std::cout << k << std::endl;
//     */

//     // cv::Mat firstKernel = gaussianKernel(1, 5);
//     // cv::Mat secKernel = -gaussianDerivativeKernel(1, 5);

//     cv::Mat G = gaussianKernel(1, 5);
//     cv::Mat dG = gaussianDerivativeKernel(1, 5);

//     float k;

// #else

//     cv::Mat firstKernel = cv::Mat::zeros(1, 3, CV_32F);
//     firstKernel.at<float>(0, 0) = 1;
//     firstKernel.at<float>(0, 1) = 2;
//     firstKernel.at<float>(0, 2) = 1;

//     cv::Mat secKernel = cv::Mat::zeros(1, 3, CV_32F);
//     secKernel.at<float>(0, 0) = -1;
//     secKernel.at<float>(0, 1) = 0;
//     secKernel.at<float>(0, 2) = 1;

//     float k = 4;

// #endif

//     //cv::Mat firstKernel = (cv::Mat<CV32F>(1, 5) << -0.2707, -0.6065, 0.0, 0.6065, 0.2707);
//     //cv::Mat secKernel = (cv::Mat<CV_32F>(1, 5) << 0.1353, 0.6065, 1.0, 0.6065, 0.1353);
//     std::cout << "gaussian" << G << std::endl;
//     std::cout << "gaussian derivative" << dG << std::endl;
//     cv::Mat kernel = -G.t() * dG;
//     // cv::Mat kernel = secKernel.t() * firstKernel;
//     std::cout << "kernel: " << kernel << std::endl;

//     k = sumNonZero(kernel);
//     std::cout << "k: " << k << std::endl;

//     kernel /= k;
//     std::cout << kernel << std::endl;

//     cv::Mat imgDst;
//     // imgDst.convertTo(imgDst, CV_32F);
//     cv::filter2D(imagen, imgDst, CV_32F, kernel);

//     double min, max;
//     cv::minMaxLoc(imgDst, &min, &max);
//     std::cout << "grad_x: " << min << " " << max << std::endl;

//     cv::Mat plot;
//     imgDst.cv::Mat::convertTo(plot, CV_8U, 0.5, 128);
//     // imgDst = imgDst / 2 + 128;
//     // cv::imshow("letsgo", imgDst / 255.0);
//     // cv::imshow("letsgo", imgDst * 255);
//     cv::imshow("letsgo", plot);
//     cv::waitKey();
// }
