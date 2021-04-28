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

enum class BlendType {
    LINEAR,
    FEATHER,
    MULTI_BAND,
    NO
};

enum class SeamType {
    VORONOI,
    DP_COLOR,
    NO
};

enum class FeaturesType {
    HARRIS,
    ORB,
    SIFT,
    SURF,
    AKAZE
};

const cv::String& imwriteSafe(const cv::String& filename, cv::InputArray img,
                              const std::vector<int>& params = std::vector<int>());

void drawHomographyBox(const cv::Mat& img, cv::Mat& dst, const cv::Mat& homography);

void drawHomographyOverlay(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& dst, const cv::Mat& homography, float alpha = 0.5);

cv::Mat warpImages(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& homography, BlendType blend_type = BlendType::NO, SeamType seam_type = SeamType::NO);

void doPanorama(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& img_panorama, FeaturesType features_type, float nn_ratio, bool use_flann, BlendType blend_type, SeamType seam_type);

#endif
