#include "SIFTPair.h"

int SIFTPair::getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts){
    
    kpts.clear();
    Ptr<SIFT> sift = SIFT::create(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
    sift->detectAndCompute(img, noArray(), kpts, desc);

    std::cout << kpts.size() << std::endl;
    return (int) kpts.size();
}

// Aplica el nearest neighbour ratio para los matches de nn_matches.
// Los matches que superan el test se almacenan en matched1 y matched2
void SIFTPair::applyNNRatio(vector<vector<DMatch>>& nn_matches, float nn_match_ratio){

    matched1.clear(), matched2.clear();
    for(size_t i = 0; i < nn_matches.size(); i++) {
      DMatch first = nn_matches[i][0];
      float dist1 = nn_matches[i][0].distance;
      float dist2 = nn_matches[i][1].distance;
      if(dist1 < nn_match_ratio * dist2) {
        matched1.push_back(kpts1[first.queryIdx]);
        matched2.push_back(kpts2[first.trainIdx]);
        matchIdx1.push_back(first.queryIdx);
        matchIdx2.push_back(first.trainIdx);
      }
    }
}

// Extrae en NNMatches los keypoints resultantes de pasar el test del 
// vecino más próximo con ratio de nn_match_ratio
void SIFTPair::getMatchesApplyNNRatio(const float nn_match_ratio){

    getKeypoints(img1, desc1, kpts1);
    getKeypoints(img2, desc2, kpts2);
    Ptr<BFMatcher> matcher = BFMatcher::create(cv::NormTypes::NORM_L2);

    // Busca un máximo de 2 emparejamientos por descriptor
    vector<vector<DMatch>> nn_matches;
    matcher->knnMatch(desc1, desc2, nn_matches, 2);

    // Conservar emparejamiento si es mejor que el segundo más parecido
    applyNNRatio(nn_matches, nn_match_ratio);
    
}
