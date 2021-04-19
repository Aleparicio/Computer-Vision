#ifndef HARRISFEATURES_H
#define HARRISFEATURES_H

#include "Pair.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class HARRISPair : public Pair {

  public:
    HARRISPair(Mat& _img1, Mat& _img2) {
        img1 = _img1;
        img2 = _img2;
    }

    HARRISPair(Mat& _img1, Mat& _img2, float _th, int _nf, int _nO, int _nL)
        : initTh(_th), nFeat(_nf), nOct(_nO), nOctLayers(_nL) {
        img1 = _img1;
        img2 = _img2;
    }

    ~HARRISPair() {}

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

    int getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) override;
};

#endif
