#include "Ransac.h"


float Ransac::get_attempts(float p, float P){
    return log(1 - P) / log(1.0 - pow(p, k));
}

cv::Mat Ransac::compute_model(std::vector<int> idxs){

    std::vector<cv::Point2f> puntos_im_1, puntos_im_2;
    for(int i = 0; i < idxs.size(); ++i){
        puntos_im_1.push_back(matched_points_img_1[idxs[i]].pt);
        puntos_im_2.push_back(matched_points_img_2[idxs[i]].pt);
    }

    return cv::findHomography(puntos_im_1, puntos_im_2, 0);
}

std::vector<bool> Ransac::check_inliers(cv::Mat model){

    cv::Mat projection = model * mat_p1;
    std::vector<bool> inliers(matched_points_img_1.size(), false);
    for(int i = 0; i < matched_points_img_1.size(); ++i){
        double proj_x = projection.at<double>(0, i) / projection.at<double>(2, i);
        double proj_y = projection.at<double>(1, i) / projection.at<double>(2, i);
        double dist = sqrt( pow(proj_x - matched_points_img_2[i].pt.x, 2) +
                            pow(proj_y - matched_points_img_2[i].pt.y, 2));

        if(dist < epsilon)
            inliers[i] = true;
    }
    return inliers;
}

cv::Mat Ransac::refine_model(std::vector<bool> inliers){

    std::vector<cv::Point2f> puntos_im_1, puntos_im_2;
    for(int i = 0; i < inliers.size(); ++i){
        if(inliers[i]){
            puntos_im_1.push_back(matched_points_img_1[i].pt);
            puntos_im_2.push_back(matched_points_img_2[i].pt);
        }
    }

    return cv::findHomography(puntos_im_1, puntos_im_2, 0);
}



cv::Mat Ransac::execute(){
    

    int t = get_attempts();
    std::vector<int> idxs(matched_points_img_1.size());
    for (int i = 0; i < matched_points_img_1.size(); i++)
        idxs[i] = i;

    cout << "Intentos: " << t << endl;
    for(int i = 0; i < t; ++i){

        std::random_shuffle(idxs.begin(), idxs.end());
        std::vector<int> selected_idxs;
        for(int j = 0 ; j < k; ++j)
            selected_idxs.push_back(idxs[j]);

        cv::Mat model = compute_model(selected_idxs);
        std::vector<bool> inliers = check_inliers(model);

        //for(int i = 0; i < inliers.size(); ++i)
        //    cout << inliers[i] << endl;


        int score = std::count(inliers.begin(), inliers.end(), true);

        if(score > best_score){
            best_score = score;
            best_inliers = inliers;
            best_model = model;
        }

    }

    cout << "Best score: " << best_score << endl;
    if(best_score > 0)
        best_model = refine_model(best_inliers);

    return best_model;
}
