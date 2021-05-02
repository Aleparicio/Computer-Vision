#include "HARRISPair.h"

// En desc devuelve los valores para cada píxel
int HARRISPair::getKeypoints(Mat& img, Mat& desc, vector<KeyPoint>& kpts) {
    kpts.clear();

    // Apply Harris detector
    Mat dst = Mat::zeros(img.size(), CV_32FC1);
    dst = Mat::zeros(img.size(), CV_32FC1);
    cornerHarris(img, dst, blockSize, apertureSize, k);

    // Normalize and show
    // Mat dst_norm, dst_norm_scaled;
    // normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1);
    // convertScaleAbs(dst_norm, dst_norm_scaled);
    // cv::imshow("Harris", dst_norm_scaled);
    // cv::waitKey(0);

    double minVal;
    double maxVal;
    minMaxLoc(dst, &minVal, &maxVal);

    // Find Harris keypoints
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            // El threshold para encontrar las esquinas depende de cada imagen
            if ((int)dst.at<float>(i, j) > threshold_ratio * maxVal) {
                // double response = 1;
                KeyPoint kp(j, i, blockSize);
                kpts.push_back(kp);
                // cv::circle(img, cv::Point(j, i), blockSize, cv::Scalar(0, 0, 255));
            }
        }
    }

    cv::Ptr<cv::DescriptorExtractor> descriptor = cv::SIFT::create();
    descriptor->compute(img, kpts, desc);

    // std::cout << "Núm keypoints encontrados: " << kpts.size() << std::endl;
    // cv::drawKeypoints(img, kpts, img);
    // cv::imshow("keypoints" + std::to_string(rand()), img);

    return (int)kpts.size();
}

// Extrae en NNMatches los keypoints resultantes de pasar el test del
// vecino más próximo con ratio de nn_match_ratio
void HARRISPair::getMatchesApplyNNRatio(const float nn_match_ratio) {
    getKeypoints(img1, desc1, kpts1);
    getKeypoints(img2, desc2, kpts2);

    Ptr<BFMatcher> matcher = BFMatcher::create(cv::NormTypes::NORM_L2);

    // Busca un máximo de 2 emparejamientos por descriptor
    vector<vector<DMatch>> nn_matches;
    matcher->knnMatch(desc1, desc2, nn_matches, 2);

    // Conservar emparejamiento si es mejor que el segundo más parecido
    applyNNRatio(nn_matches, nn_match_ratio);
}
