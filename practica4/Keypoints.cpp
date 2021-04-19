#include <opencv2/opencv.hpp>

#include "HARRISPair.h"
#include "ORBPair.h"
#include "SIFTPair.h"
#include "SURFPair.h"
#include "AKAZEPair.h"
#include "Ransac.h"

void drawHomographyBox(const cv::Mat& img, cv::Mat& dst, const cv::Mat& homography) {
    cv::Mat esquinas = cv::Mat::ones(3, 4, CV_64F);
    esquinas.at<double>(0, 0) = 0.0;
    esquinas.at<double>(1, 0) = 0.0;
    esquinas.at<double>(0, 1) = img.cols;
    esquinas.at<double>(1, 1) = 0.0;
    esquinas.at<double>(0, 2) = 0.0;
    esquinas.at<double>(1, 2) = img.rows;
    esquinas.at<double>(0, 3) = img.cols;
    esquinas.at<double>(1, 3) = img.rows;

    cv::Mat resul = homography * esquinas;
    vector<Point> pt(4);
    for (int i = 0; i < 4; ++i) {
        pt[i].x = resul.at<double>(0, i) / resul.at<double>(2, i);
        pt[i].y = resul.at<double>(1, i) / resul.at<double>(2, i);
    }
    dst = img.clone();
    cv::line(dst, pt[0], pt[1], Scalar(255, 255, 255));
    cv::line(dst, pt[0], pt[2], Scalar(255, 255, 255));
    cv::line(dst, pt[3], pt[1], Scalar(255, 255, 255));
    cv::line(dst, pt[3], pt[2], Scalar(255, 255, 255));
}

// Superponer img1 sobre img2
void drawHomographyOverlay(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& dst, const cv::Mat& homography, float alpha = 0.5) {
    // Máscara
    cv::Mat imgMask = cv::Mat(img1.size(), CV_8UC1, cv::Scalar(255));
    cv::warpPerspective(imgMask, imgMask, homography, img2.size());

    // Warp img1
    cv::warpPerspective(img1, dst, homography, img2.size());

    // Mezclar las dos imágenes
    dst = img2 * alpha + dst * (1 - alpha);

    // dst = img2.clone();
    // img2.copyTo(dst, imgMask);
}

int main(int argc, char** argv) {

    cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_GRAYSCALE);

    if (img1.empty() || img2.empty())
        return -1;

    // std::shared_ptr<Pair> pair = std::make_shared<HARRISPair>(img1, img2);
    std::shared_ptr<Pair> pair = std::make_shared<ORBPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<SIFTPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<SURFPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<AKAZEPair>(img1, img2);

    pair->getMatchesApplyNNRatio(0.8);

    std::vector<cv::DMatch> matches(pair->matched1.size());
    for (int i = 0; i < pair->matched1.size(); ++i)
        matches[i] = DMatch(i, i, 0);

    Ransac robust_solver(pair->matched1, pair->matched2);
    cv::Mat homography = robust_solver.execute();

    cv::Mat matches_resul, homography_resul;
    cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, matches_resul);
    // drawHomographyBox(img2, homography_resul, homography);
    drawHomographyOverlay(img1, img2, homography_resul, homography);
    cv::imshow("Keypoints", matches_resul);
    cv::imshow("Homografia", homography_resul);
    cv::waitKey(0);
}
