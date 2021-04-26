#include "Ransac.h"

#include <numeric>

int Ransac::get_attempts(float p, float P) {
    // std::cout << p << " " << P << " " << k << std::endl;
    int attempts = log(1.0 - P) / log(1.0 - pow(p, k));
    return std::max(0, attempts);
}

cv::Mat Ransac::compute_model(std::vector<int> idxs) {

    std::vector<cv::Point2f> puntos_im_1, puntos_im_2;
    for (int i = 0; i < idxs.size(); ++i) {
        puntos_im_1.push_back(matched_points_img_1[idxs[i]].pt);
        puntos_im_2.push_back(matched_points_img_2[idxs[i]].pt);
    }

    return cv::findHomography(puntos_im_1, puntos_im_2, 0);
}

std::vector<bool> Ransac::check_inliers(cv::Mat model) {

    cv::Mat projection = model * mat_p1;
    std::vector<bool> inliers(matched_points_img_1.size(), false);
    for (int i = 0; i < matched_points_img_1.size(); ++i) {
        double proj_x = projection.at<double>(0, i) / projection.at<double>(2, i);
        double proj_y = projection.at<double>(1, i) / projection.at<double>(2, i);
        double dist = sqrt(pow(proj_x - matched_points_img_2[i].pt.x, 2) + pow(proj_y - matched_points_img_2[i].pt.y, 2));

        if (dist < epsilon)
            inliers[i] = true;
    }
    return inliers;
}

cv::Mat Ransac::refine_model(std::vector<bool> inliers) {

    std::vector<cv::Point2f> puntos_im_1, puntos_im_2;
    for (int i = 0; i < inliers.size(); ++i) {
        if (inliers[i]) {
            puntos_im_1.push_back(matched_points_img_1[i].pt);
            puntos_im_2.push_back(matched_points_img_2[i].pt);
        }
    }

    return cv::findHomography(puntos_im_1, puntos_im_2, 0);
}

cv::Mat Ransac::execute() {

    int t = get_attempts();

    // Create array filled with 0, ..., matched_points - 1
    std::vector<int> idxs(matched_points_img_1.size());
    std::iota(idxs.begin(), idxs.end(), 0);

    std::cout << "Intentos: " << t << std::endl;
    for (int i = 0; i < t; ++i) {
        std::random_shuffle(idxs.begin(), idxs.end());
        std::vector<int> selected_idxs(idxs.begin(), idxs.begin() + k);

        cv::Mat model = compute_model(selected_idxs);
        std::vector<bool> inliers = check_inliers(model);

        //for(int i = 0; i < inliers.size(); ++i)
        //    cout << inliers[i] << endl;

        int score = std::count(inliers.begin(), inliers.end(), true);
        int possible_attempts = get_attempts((float)score / (float)inliers.size());

        if (possible_attempts < t) {
            std::cout << score << " " << inliers.size() << " " << possible_attempts << std::endl;
            std::cout << "Adaptativo, reducido el número de intentos de " << t << " a " << possible_attempts << std::endl;
            t = possible_attempts;
        }

        std::cout << "Nuevo t:" << t << std::endl;
        std::cout << "i: " << i << std::endl;

        // t = std::min(t, possible_attempts);

        if (score > best_score) {
            best_score = score;
            best_inliers = inliers;
            best_model = model;
        }
    }

    std::cout << "Best score: " << best_score << std::endl;
    if (best_score > 0)
        best_model = refine_model(best_inliers);

    return best_model;
}
