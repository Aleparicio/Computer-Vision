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

    HARRISPair(Mat& _img1, Mat& _img2, int _blockSize, int _apertureSize, double _k, double _threshold)
        : blockSize(_blockSize), apertureSize(_apertureSize), k(_k), threshold(_threshold) {
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
    // Tamaño de la vecindad (neighborhood) considerada alrededor del píxel
    int blockSize = 2;
    // Parámetro de apertura para el operador de Sobel
    int apertureSize = 3;
    // Parámetro k libre del detector de Harris
    double k = 0.04;
    // Threshold para obtener un resultado óptimo
    double threshold = 100; // [0 - 255]

    int getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) override;
};

#endif
