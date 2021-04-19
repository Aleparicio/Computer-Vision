#include "AKAZEPair.h"

int AKAZEPair::getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) {

    kpts.clear();
    Ptr<AKAZE> akaze = AKAZE::create(AKAZE::DESCRIPTOR_MLDB, 0, 3, initTh, nOct, nOctLayers);
    akaze->detectAndCompute(img, noArray(), kpts, desc);

    std::cout << kpts.size() << std::endl;
    return (int)kpts.size();
}

// Extrae en NNMatches los keypoints resultantes de pasar el test del
// vecino más próximo con ratio de nn_match_ratio
void AKAZEPair::getMatchesApplyNNRatio(const float nn_match_ratio) {

    getKeypoints(img1, desc1, kpts1);
    getKeypoints(img2, desc2, kpts2);
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);

    // Busca un máximo de 2 emparejamientos por descriptor
    vector<vector<DMatch>> nn_matches;
    matcher->knnMatch(desc1, desc2, nn_matches, 2);

    // Conservar emparejamiento si es mejor que el segundo más parecido
    applyNNRatio(nn_matches, nn_match_ratio);
}

// Extrae los keypoints simétricos obtenidos por fuerza bruta
/*void AKAZEPair::getSymMatches(){

    getKeypoints(img1, desc1, kpts1);
    getKeypoints(img2, desc2, kpts2);
    Ptr<BFMatcher> matcher = cv::BFMatcher::create(NORM_HAMMING, true);

    // Busca los matches simétricos
    vector<vector<DMatch>> nn_matches;
    matcher->knnMatch(desc1, desc2, nn_matches, 1);

    for(int i = 0; i < nn_matches.size(); ++i){
      vector<DMatch> matchVector = nn_matches[i];
      if(matchVector.size() != 0){
        DMatch match = matchVector[0];
        matched1.push_back(kpts1[match.queryIdx]);
        matched2.push_back(kpts2[match.trainIdx]);
        matchIdx1.push_back(match.queryIdx);
        matchIdx2.push_back(match.trainIdx);
      }
    }
}*/
