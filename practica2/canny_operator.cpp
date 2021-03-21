#include "canny_operator.hpp"

#include <iostream>

void normalizeGradient(cv::InputArray src, cv::OutputArray dst, float min, float max) {
    for (float i = 0; i < src.getMat().rows; i++) {
        for (float j = 0; j < src.getMat().cols; j++) {
            float val;
            val = src.getMat().at<float>(i, j);
            if (val < 0.0f)
                dst.getMat().at<float>(i, j) = (val / std::abs(min)) * 255.0f;
            else if (val > 0.0f)
                dst.getMat().at<float>(i, j) = (val / max) * 255.0f;
        }
    }
}

void normalizeGradients(cv::InputArray src_x, cv::InputArray src_y,
                        cv::OutputArray dst_x, cv::OutputArray dst_y) {
    double min_x, min_y, max_x, max_y;
    cv::minMaxLoc(src_x, &min_x, &max_x);
    // std::cout << "Gradient X: " << min_x << " " << max_x << std::endl;
    cv::minMaxLoc(src_y, &min_y, &max_y);
    // std::cout << "Gradient Y: " << min_y << " " << max_y << std::endl;
    double min = std::min(min_x, min_y);
    double max = std::max(max_x, max_y);

    normalizeGradient(src_x, dst_x, min, max);
    normalizeGradient(src_y, dst_y, min, max);
}

cv::Mat gaussianKernel(float sigma, int n) {

    if (!(n & 1))
        throw("The kernel size must be odd");

    cv::Mat kernel(1, n, CV_32F);
    for (int i = 0; i < n; i++) {
        float x = i - n / 2;
        kernel.at<float>(i) = expf(-(x * x) / (2 * sigma * sigma));
    }

    return kernel;
}

cv::Mat gaussianDerivativeKernel(float sigma, int n) {

    if (!(n & 1))
        throw("The kernel size must be odd");

    cv::Mat kernel(1, n, CV_32F);
    for (int i = 0; i < n; i++) {
        float x = i - n / 2;
        kernel.at<float>(i) = -x / (sigma * sigma) * expf(-(x * x) / (2 * sigma * sigma));
    }
    return kernel;
}

float sumPositive(cv::Mat mat) {

    /* std::cout << mat << std::endl;
    // cv::Mat mask = (mat > 0);
    // std::cout << mask << std::endl;
    // cv::Mat nonZero;
    // cv::bitwise_and(mat, mask, nonZero);
    // float k = sum(nonZero)[0];
    // std::cout << cv::sum(nonZero) << std::endl;
    // std::cout << cv::sum(kernel & mask) << std::endl; */

    float k = 0;
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (mat.at<float>(i, j) > 0)
                k += mat.at<float>(i, j);
        }
    }

    return k;
}

void CannyGradient(cv::InputArray src, cv::OutputArray grad_x, cv::OutputArray grad_y, float sigma, float ksize) {

    cv::Mat G = gaussianKernel(sigma, ksize);
    cv::Mat dG = gaussianDerivativeKernel(sigma, ksize);

    // Convert input image to 16S
    cv::Mat src_converted;
    src.getMat().convertTo(src_converted, CV_32F);

    cv::Mat kernel;
    float k;

    // // Square kernels
    // // x gradient
    // kernel = -G.t() * dG;
    // k = sumPositive(kernel);
    // kernel /= k;
    // cv::filter2D(src_converted, grad_x, CV_32F, kernel);
    // // y gradient
    // kernel = -dG.t() * G;
    // k = sumPositive(kernel);
    // kernel /= k;
    // cv::filter2D(src_converted, grad_y, CV_32F, kernel);

    // Kernels applied as separable filters
    // x gradient
    cv::filter2D(src_converted, grad_x, CV_32F, G.t());
    cv::filter2D(grad_x, grad_x, CV_32F, -dG);
    // y gradient
    cv::filter2D(src_converted, grad_y, CV_32F, -dG.t());
    cv::filter2D(grad_y, grad_y, CV_32F, G);

    // cv::normalize(grad_x.getMat(), grad_x.getMat(), -255, 255, cv::NORM_MINMAX);
    // cv::normalize(grad_y.getMat(), grad_y.getMat(), -255, 255, cv::NORM_MINMAX);
}
