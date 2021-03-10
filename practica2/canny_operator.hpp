#ifndef canny
#define canny

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

float sumPositive(cv::Mat mat);

cv::Mat gaussianKernel(float sigma, int n);

cv::Mat gaussianDerivativeKernel(float sigma, int n);

void CannyGradient(cv::InputArray src, cv::OutputArray grad_x, cv::OutputArray grad_y, float sigma, float ksize);

#endif

