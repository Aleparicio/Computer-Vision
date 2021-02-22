#ifndef FILTERS
#define FILTERS

#include <opencv2/opencv.hpp>

//enum eqType { EQHISTO, CLAHE};

void contrastAdjustment(cv::Mat& image, float gain, float sesgo);

//void equalizeImage(cv::Mat& image);


#endif