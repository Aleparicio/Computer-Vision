/*#ifndef SURFFEATURES_H
#define SURFFEATURES_H

#include "Pair.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace xfeatures2d;
using namespace std;

class SURFPair : public Pair {

  public:
    SURFPair(Mat& _img1, Mat& _img2) {
        img1 = _img1;
        img2 = _img2;
    }

    SURFPair(Mat& _img1, Mat& _img2, float _hessianThreshold, int _nOctaves, int _nOctaveLayers)
        : hessianThreshold(_hessianThreshold), nOctaves(_nOctaves), nOctaveLayers(_nOctaveLayers) {
        img1 = _img1;
        img2 = _img2;
    }

    ~SURFPair() {}

    // Extrae en NNMatches los keypoints resultantes de pasar el test del vecino más próximo con
    // ratio de nn_match_ratio
    void getMatchesApplyNNRatio(const float nn_match_ratio = 0.8f) override;

    // Extrae los keypoints simétricos obtenidos por fuerza bruta
    void getSymMatches();

  private:
    // ==========================================
    // Atributos de configuración
    // ==========================================
    // Threshold
    float hessianThreshold = 100;
    // Número de octavas
    int nOctaves = 4;
    // Layers por octava
    int nOctaveLayers = 3;

    int getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) override;
};

#endif
*/