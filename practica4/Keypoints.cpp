#include <opencv2/opencv.hpp>

#include "HARRISPair.h"
#include "ORBPair.h"
#include "SIFTPair.h"
#include "SURFPair.h"
#include "AKAZEPair.h"

int main(int argc, char** argv) {
    // imread automatically finds out the file format

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

    cv::Mat resultado;
    cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, resultado);

    cv::imshow("Keypoints", resultado);
    cv::waitKey(0);
}
