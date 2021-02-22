#ifndef FILTERS
#define FILTERS

#include <opencv2/opencv.hpp>

enum filtro {CONTRASTE, ALIEN, POSTER, DISTORSION, EQHISTO, CLAHE};

void contrastAdjustment(cv::Mat& image, float gain, float sesgo);

void equalizeHist(cv::Mat& image);

void equalizeCLAHE(cv::Mat& image, float clipLimit = 3.0);

#endif