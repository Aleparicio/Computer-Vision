#ifndef ORBFEATURES_H
#define ORBFEATURES_H

#include "Pair.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ORBPair : public Pair {

  public:
    ORBPair(Mat& _img1, Mat& _img2) {
        img1 = _img1;
        img2 = _img2;
    }

    ORBPair(Mat& _img1, Mat& _img2, float _nfeatures, int _scaleFactor, int _nlevels, int _edgeThreshold)
        : nfeatures(_nfeatures), scaleFactor(_scaleFactor), nlevels(_nlevels), edgeThreshold(_edgeThreshold) {
        img1 = _img1;
        img2 = _img2;
    }

    ~ORBPair() {}

    // Extrae en NNMatches los keypoints resultantes de pasar el test del vecino más próximo con
    // ratio de nn_match_ratio
    void getMatchesApplyNNRatio(const float nn_match_ratio = 0.8f) override;

    // Extrae los keypoints simétricos obtenidos por fuerza bruta
    void getSymMatches();

  private:
    // ==========================================
    // Atributos de configuración
    // ==========================================
    // Number of features
    int nfeatures = 500;
    // Number of layers per octave
    double scaleFactor = 1.2;
    // Contrast threshold
    double nlevels = 8;
    // Edge threshold
    double edgeThreshold = 31;

    // (int 	nfeatures = 500, float 	scaleFactor = 1.2f, int 	nlevels = 8, int 	edgeThreshold = 31)

    int getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) override;
};

#endif
