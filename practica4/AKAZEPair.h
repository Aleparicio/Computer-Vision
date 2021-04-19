#ifndef AKAZEFEATURES_H
#define AKAZEFEATURES_H

#include "Pair.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class AKAZEPair : public Pair {

public:

    AKAZEPair(Mat& _img1, Mat& _img2) {
        img1 = _img1;
        img2 = _img2;
    }

    AKAZEPair(Mat& _img1, Mat& _img2, float _th, int _nf, int _nO, int _nL) 
            : initTh(_th), nFeat(_nf), nOct(_nO), nOctLayers(_nL) {
        img1 = _img1;
        img2 = _img2;
    }

    ~AKAZEPair(){}

    // Extrae en NNMatches los keypoints resultantes de pasar el test del vecino más próximo con
    // ratio de nn_match_ratio
    void getMatchesApplyNNRatio(const float nn_match_ratio = 0.8f) override;

    // Extrae los keypoints simétricos obtenidos por fuerza bruta
    void getSymMatches();

private:

    // ==========================================
    // Atributos de configuración 
    // ==========================================
    // Init threshold
    float initTh = 0.005f;
    // Número de features
    int nFeat = 1000;
    // Número de octavas 
    int nOct = 3;
    // Layers por octava
    int nOctLayers = 4;


    // Aplica el nearest neighbour ratio para los matches de nn_matches.
    // Los matches que superan el test se almacenan en matched1 y matched2
    void applyNNRatio(std::vector<std::vector<cv::DMatch>>& nn_matches, float nn_match_ratio) override;

    int getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) override;

};

#endif 
