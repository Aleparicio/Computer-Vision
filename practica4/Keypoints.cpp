#include "opencv2/opencv.hpp"
#include "SIFTPair.h"
#include "AKAZEPair.h"


enum kptType { ORBKPT, SIFTKPT, AKAZEKPT, SURFKPT };

Pair* selectPair(cv::Mat img1, cv::Mat img2, kptType tipo) {
    Pair *pair;
    if(tipo == SIFTKPT)
        pair = new SIFTPair(img1, img2);
    else 
        pair = new AKAZEPair(img1, img2); 
    return pair;
}







int main(int argc, char **argv)
{
    // imread automatically finds out the file format

    cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_GRAYSCALE);
    if (img1.empty())
        return -1;
    if (img2.empty())
        return -1;

    // Windows are associated with a name
    // (error if a different window is closed)
    //cv::namedWindow("Example1", cv::WINDOW_AUTOSIZE);
    //cv::imshow("Example1", img);
    //cv::waitKey(0);

    // HARRIS
    // cv::cornerHarris();
    // ORB
    // cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    // SIFT
    // cv::Ptr<cv::FeatureDetector> detector = cv::SIFT::create();
    // SURF
    // cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::SURF::create();
    // AKAZE

    Pair* pair = selectPair(img1, img2, AKAZEKPT);
    pair->getMatchesApplyNNRatio(0.8);

    std::vector<cv::DMatch> matches(pair->matched1.size());
    for(int i = 0; i < pair->matched1.size(); ++i)
        matches[i] = DMatch(i, i, 0);

    cv::Mat resultado;
    cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, resultado);

    cv::imshow("Keypoints", resultado);
    cv::waitKey(0);
}
