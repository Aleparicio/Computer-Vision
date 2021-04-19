#ifndef SIFTFEATURES_H
#define SIFTFEATURES_H

#include "Pair.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class SIFTPair : public Pair {

  public:
    SIFTPair(Mat& _img1, Mat& _img2) {
        img1 = _img1;
        img2 = _img2;
    }

    SIFTPair(Mat& _img1, Mat& _img2, float _cth, int _eth, int _nf, int _nL, int _sigma)
        : contrastThreshold(_cth), edgeThreshold(_eth), nfeatures(_nf), nOctaveLayers(_nL), sigma(_sigma) {
        img1 = _img1;
        img2 = _img2;
    }

    ~SIFTPair() {}

    // Extrae en NNMatches los keypoints resultantes de pasar el test del vecino más próximo con
    // ratio de nn_match_ratio
    void getMatchesApplyNNRatio(const float nn_match_ratio = 0.8f) override;

  private:
    // ==========================================
    // Atributos de configuración
    // ==========================================
    // Number of features
    int nfeatures = 0;
    // Number of layers per octave
    int nOctaveLayers = 3;
    // Contrast threshold
    double contrastThreshold = 0.09;
    // Edge threshold
    double edgeThreshold = 10;
    // Sigma
    double sigma = 1.6;

    int getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) override;
};

#endif
