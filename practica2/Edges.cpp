#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main() {
    // std::string image_path = cv::samples::findFile("../../images/flor.png");
    // std::string image_path = cv::samples::findFile("../../images/garden.jpg");
    std::string image_path = cv::samples::findFile("../../images/cuadro.png");
    // std::string image_path = cv::samples::findFile("../../images/pasillo.png");
    // std::string image_path = cv::samples::findFile("../../images/lena.png");
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

    cv::convertScaleAbs(grad_x, abs_grad_x, 0.5, 128);
    cv::convertScaleAbs(grad_y, abs_grad_y, 0.5, 128);

    // Invert y gradient
    grad_y = -grad_y;

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
    angle.convertTo(angle, CV_8U, 180 / CV_PI);
    cv::imshow("Magnitude", mag);
    cv::imshow("Angle", angle);

    // cv::Mat mag = grad_x * grad_x + grad_y * grad_y;
    // std::cout << grad_x << std::endl;
    // std::cout << mag.t() << std::endl;

    cv::imshow("Display window", img);
    int k = cv::waitKey(0); // Wait por a keystroke in the window

    cv::destroyAllWindows();

    ////////////////////
    // Hough transform
    ////////////////////

    // Edge detection
    cv::Mat dst;
    Canny(src_gray, dst, 50, 200, 3);

    // Copy edges to the images that will display the results in BGR
    cv::Mat cdst, cdstP;
    cv::cvtColor(dst, cdst, cv::COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    // Standard Hough Line Transform
    std::vector<cv::Vec2f> lines;                          // will hold the results of the detection
    cv::HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0); // Run the detection
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

    k = cv::waitKey(0); // Wait for a keystroke in the window
    cv::destroyAllWindows();

    /////////////////////////////
    // Gaussian derivatives test
    /////////////////////////////

    // cv::Mat kernel = cv::getGaussianKernel(5, 1, CV_32F);
    // kernel = kernel.reshape(1, 1);
    cv::Mat kernel_y{0.2707, 0.6065, 0.0, -0.6065, -0.2707};
    cv::Mat kernel_x{-0.2707, -0.6065, 0.0, 0.6065, 0.2707};
    // cv::Mat kernel_x = kernel_y.t();
    kernel_x = kernel_x.reshape(1, 1);
    std::cout << kernel_x << std::endl;
    std::cout << kernel_y << std::endl;

    cv::Mat dgauss_x;
    cv::Mat dgauss_y;
    cv::filter2D(src_gray, dgauss_x, CV_8U, kernel_x);
    cv::filter2D(src_gray, dgauss_y, CV_8U, kernel_y);
    cv::imshow("Gaussian derivative x", dgauss_x);
    cv::imshow("Gaussian derivative y", dgauss_y);
    cv::waitKey(0);

    return 0;
}
