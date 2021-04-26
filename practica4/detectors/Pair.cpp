#include "Pair.h"

// Aplica el nearest neighbour ratio para los matches de nn_matches.
// Los matches que superan el test se almacenan en matched1 y matched2
void Pair::applyNNRatio(vector<vector<DMatch>>& nn_matches, float nn_match_ratio) {
    matched1.clear(), matched2.clear();
    for (size_t i = 0; i < nn_matches.size(); i++) {
        DMatch first = nn_matches[i][0];
        float dist1 = nn_matches[i][0].distance;
        float dist2 = nn_matches[i][1].distance;
        if (dist1 < nn_match_ratio * dist2) {
            matched1.push_back(kpts1[first.queryIdx]);
            matched2.push_back(kpts2[first.trainIdx]);
            matchIdx1.push_back(first.queryIdx);
            matchIdx2.push_back(first.trainIdx);
        }
    }
}


// https://stackoverflow.com/questions/44184159/opencv-c-flann-indexparams-and-searchparams-error
void Pair::flannMatchesNNRatio(const float nn_match_ratio){

    getKeypoints(img1, desc1, kpts1);
    getKeypoints(img2, desc2, kpts2);

    if(desc1.type() != CV_32F)
        desc1.convertTo(desc1, CV_32F);
    if(desc2.type() != CV_32F)
        desc2.convertTo(desc2, CV_32F);

    std::vector<std::vector<cv::DMatch>> nn_matches;
    cv::FlannBasedMatcher matcher;
    matcher.knnMatch(desc1, desc2, nn_matches, 2);

    applyNNRatio(nn_matches, nn_match_ratio);
}