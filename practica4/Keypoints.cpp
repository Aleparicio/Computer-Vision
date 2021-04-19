#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

int main(int argc, char **argv)
{
    // imread automatically finds out the file format
    cv::Mat img = cv::imread("../../images/BuildingScene/building1.JPG", -1);
    if (img.empty())
        return -1;
    // Windows are associated with a name
    // (error if a different window is closed)
    cv::namedWindow("Example1", cv::WINDOW_AUTOSIZE);
    cv::imshow("Example1", img);
    cv::waitKey(0);

    // HARRIS
    // cv::cornerHarris();
    // ORB
    // cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    // SIFT
    // cv::Ptr<cv::FeatureDetector> detector = cv::SIFT::create();
    // SURF
    // cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::SURF::create();
    // AKAZE
    cv::Ptr<cv::FeatureDetector> detector = cv::AKAZE::create();

    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;

    // detectAndCompute() también es una función
    detector->detect(img, keypoints);
    detector->compute(img, keypoints, descriptors);

    cv::drawKeypoints(img, keypoints, img);

    cv::imshow("Keypoints", img);
    cv::waitKey(0);
}
