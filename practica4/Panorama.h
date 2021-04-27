#ifndef PANORAMA_H
#define PANORAMA_H

#include <opencv2/opencv.hpp>

#include "HARRISPair.h"
#include "ORBPair.h"
#include "SIFTPair.h"
#include "AKAZEPair.h"
#include "Ransac.h"

#ifdef HAVE_XFEATURES2D_NONFREE_H
#include "SURFPair.h"
#endif

const cv::String& imwriteSafe(const cv::String& filename, cv::InputArray img,
                              const std::vector<int>& params = std::vector<int>());

void drawHomographyBox(const cv::Mat& img, cv::Mat& dst, const cv::Mat& homography);

void drawHomographyOverlay(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& dst, const cv::Mat& homography, float alpha = 0.5);

enum BlendType {
    NONE,
    LINEAR,
    FEATHER,
    MULTI_BAND
};

cv::Mat warpImages(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& homography, BlendType blend_type = BlendType::NONE);

void doPanorama(cv::Mat& img1, cv::Mat& img2, cv::Mat& img_panorama, BlendType blend_type = BlendType::NONE);

#endif
