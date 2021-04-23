#include "SURFPair.h"

int SURFPair::getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) {

    kpts.clear();
    Ptr<SURF> surf = SURF::create(hessianThreshold, nOctaves, nOctaveLayers);
    surf->detectAndCompute(img, noArray(), kpts, desc);

    std::cout << kpts.size() << std::endl;
    return (int)kpts.size();
}

// Extrae en NNMatches los keypoints resultantes de pasar el test del
// vecino más próximo con ratio de nn_match_ratio
void SURFPair::getMatchesApplyNNRatio(const float nn_match_ratio) {

    getKeypoints(img1, desc1, kpts1);
    getKeypoints(img2, desc2, kpts2);
    Ptr<DescriptorMatcher> matcher = BFMatcher::create(NormTypes::NORM_L2);

    // Busca un máximo de 2 emparejamientos por descriptor
    vector<vector<DMatch>> nn_matches;
    matcher->knnMatch(desc1, desc2, nn_matches, 2);

    // Conservar emparejamiento si es mejor que el segundo más parecido
    applyNNRatio(nn_matches, nn_match_ratio);
}
