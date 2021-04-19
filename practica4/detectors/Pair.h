#ifndef PAIR_H
#define PAIR_H

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Pair {
  public:
    // Extrae en NNMatches los keypoints resultantes de pasar el test del vecino más próximo con
    // ratio de nn_match_ratio
    virtual void getMatchesApplyNNRatio(const float nn_match_ratio = 0.8f) = 0;

  protected:
    // Aplica el nearest neighbour ratio para los matches de nn_matches.
    // Los matches que superan el test se almacenan en matched1 y matched2
    void applyNNRatio(std::vector<std::vector<cv::DMatch>>& nn_matches, float nn_match_ratio);

    // Devuelve el número de keypoints encontrados, que será mayor o igual que nFeatures
    // a menos que no se encuentren suficientes keypoints con un umbral mayor que 1e-10
    virtual int getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) = 0;

    // ==========================================
    // Atributos base
    // ==========================================
    // Descriptores
    cv::Mat desc1, desc2;
    // Imágenes
    cv::Mat img1, img2;

  public:
    // ==========================================
    // Atributos del matching
    // ==========================================
    // Matches
    std::vector<cv::KeyPoint> matched1, matched2;
    // Index de ambos keypoints en el emparejamiento
    std::vector<int> matchIdx1, matchIdx2;
    // Matches
    std::vector<cv::DMatch> matches;
    // Keypoints de cada imagen
    std::vector<cv::KeyPoint> kpts1, kpts2;
};

#endif
