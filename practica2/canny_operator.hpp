#ifndef canny
#define canny

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// Normalize both gradients between 255 and 255
void normalizeGradients(cv::InputArray src_x, cv::InputArray src_y,
                        cv::OutputArray dst_x, cv::OutputArray dst_y);

float sumPositive(cv::Mat mat);

cv::Mat gaussianKernel(float sigma, int n);

cv::Mat gaussianDerivativeKernel(float sigma, int n);

void CannyGradient(cv::InputArray src, cv::OutputArray grad_x, cv::OutputArray grad_y, float sigma, float ksize);

#endif
