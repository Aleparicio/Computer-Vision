#include "canny_operator.hpp"

#include <iostream>

cv::Mat gaussianKernel(float sigma, int n) {
    if (!(n & 1)) {
        throw("The kernel size must be odd");
    }

    cv::Mat kernel(1, n, CV_32F);

    for (int i = 0; i < n; i++) {
        float x = i - n / 2;
        kernel.at<float>(i) = expf(-(x * x) / (2 * sigma * sigma));
    }

    return kernel;
}

cv::Mat gaussianDerivativeKernel(float sigma, int n) {
    if (!(n & 1)) {
        throw("The kernel size must be odd");
    }

    cv::Mat kernel(1, n, CV_32F);

    for (int i = 0; i < n; i++) {
        float x = i - n / 2;
        kernel.at<float>(i) = -x / (sigma * sigma) * expf(-(x * x) / (2 * sigma * sigma));
    }

    return kernel;
}

float sumPositive(cv::Mat mat) {
    // std::cout << mat << std::endl;

    // cv::Mat mask = (mat > 0);
    // std::cout << mask << std::endl;
    // cv::Mat nonZero;
    // cv::bitwise_and(mat, mask, nonZero);
    // float k = sum(nonZero)[0];

    // std::cout << cv::sum(nonZero) << std::endl;

    // std::cout << cv::sum(kernel & mask) << std::endl;

    float k = 0;
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (mat.at<float>(i, j) > 0) {
                k += mat.at<float>(i, j);
            }
        }
    }

    return k;
}

void CannyGradient(cv::InputArray src, cv::OutputArray grad_x, cv::OutputArray grad_y, float sigma, float ksize) {
    cv::Mat G = gaussianKernel(sigma, ksize);
    cv::Mat dG = gaussianDerivativeKernel(sigma, ksize);
    std::cout << "sigma: " << sigma << std::endl;
    std::cout << "gaussian: " << G << std::endl;
    std::cout << "gaussian derivative: " << dG << std::endl;

    // Convert input image to float
    cv::Mat src_converted;
    src.getMat().convertTo(src_converted, CV_16S);

    cv::Mat kernel;
    float k;

    // x gradient
    kernel = -G.t() * dG;
    k = sumPositive(kernel);
    kernel /= k;
    cv::filter2D(src_converted, grad_x, CV_16S, kernel);

    // y gradient
    kernel = -dG.t() * G;
    k = sumPositive(kernel);
    kernel /= k;
    cv::filter2D(src_converted, grad_y, CV_16S, kernel);
}
