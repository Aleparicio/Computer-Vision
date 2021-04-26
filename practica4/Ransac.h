#ifndef RANSAC_H
#define RANSAC_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>

class Ransac {

  public:
    Ransac(std::vector<cv::KeyPoint> puntos_img_1,
           std::vector<cv::KeyPoint> puntos_img_2, float epsilon_ = 3.0) {

        epsilon = epsilon_;
        matched_points_img_1 = puntos_img_1;
        matched_points_img_2 = puntos_img_2;

        mat_p1 = cv::Mat::ones(3, matched_points_img_1.size(), CV_64F);
        for (size_t i = 0; i < matched_points_img_1.size(); i++) {
            mat_p1.at<double>(0, i) = matched_points_img_1[i].pt.x;
            mat_p1.at<double>(1, i) = matched_points_img_1[i].pt.y;
        }
    }

    cv::Mat execute();

    int best_score = -1;
    std::vector<bool> best_inliers;
    cv::Mat best_model;

  protected:
    cv::Mat compute_model(std::vector<int> idxs);
    cv::Mat refine_model(std::vector<bool> inliers);
    std::vector<bool> check_inliers(cv::Mat model);
    int get_attempts(float p = 0.5, float P = 0.99);

    const int k = 4;
    float epsilon = 5;

    std::vector<cv::KeyPoint> matched_points_img_1;
    std::vector<cv::KeyPoint> matched_points_img_2;
    cv::Mat mat_p1;
};

#endif
