#include "descriptors.hpp"

Descriptors descriptors(cv::InputArray frame) {

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(frame, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Moments> momentos(contours.size());
    std::vector<double> perimeters(contours.size(), 0.0);
    std::vector<double> area(contours.size(), 0.0);
    double momentosHu[contours.size()][7];

    for (size_t i = 0; i < contours.size(); i++) {
        momentos[i] = cv::moments(contours[i]);
        cv::HuMoments(momentos[i], momentosHu[i]);
        perimeters[i] = cv::arcLength(contours[i], true);
        area[i] = cv::contourArea(contours[i]);
    }

    int biggest = 0;
    float biggestArea = 0.0;
    for (int i = 0; i < contours.size(); i++) {
        if (biggestArea < area[i]) {
            biggestArea = area[i];
            biggest = i;
        }
    }

    return Descriptors(perimeters[biggest], area[biggest],
                       momentosHu[biggest][0], momentosHu[biggest][1], momentosHu[biggest][2]);
}

void drawContours(cv::InputArray frame, cv::OutputArray out) {

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(frame, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Moments> momentos(contours.size());
    std::vector<double> perimeters(contours.size(), 0.0);
    std::vector<double> area(contours.size(), 0.0);
    double momentosHu[contours.size()][7];

    for (size_t i = 0; i < contours.size(); i++) {
        momentos[i] = moments(contours[i]);
        cv::HuMoments(momentos[i], momentosHu[i]);
        perimeters[i] = arcLength(contours[i], true);
        area[i] = contourArea(contours[i]);
    }

    // https://docs.opencv.org/3.4/d0/d49/tutorial_moments.html
    cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++) {
        cv::Scalar color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
        drawContours(drawing, contours, (int)i, color, 2);
    }

    std::cout << "\t Info: Area and Contour Length \n";
    for (size_t i = 0; i < contours.size(); i++) {
        std::cout << " * Contour[" << i << "] "
                  << " - Area OpenCV: " << area[i]
                  << " - Length: " << perimeters[i] << std::endl;
    }

    out.getMat() = drawing;
}
// https://answers.opencv.org/question/168403/connected-components-with-opencv/
void drawConnectedComponents(cv::InputArray frame, cv::OutputArray out) {

    srand(time(NULL));
    cv::Mat labels;
    int nlabels = cv::connectedComponents(frame, labels, 4, CV_32S);

    std::vector<cv::Vec3b> colores(nlabels, cv::Vec3b(0, 0, 0));
    for (int i = 1; i < nlabels; ++i)
        colores[i] = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);

    cv::Mat dst(frame.size(), CV_8UC3);
    for (int row = 0; row < dst.rows; ++row) {
        for (int col = 0; col < dst.cols; ++col) {
            int label = labels.at<int>(row, col);
            cv::Vec3b& pixel = dst.at<cv::Vec3b>(row, col);
            pixel = colores[label];
        }
    }

    out.getMat() = dst;
}

void thresholding(cv::InputArray frame, cv::OutputArray out, ThresholdType type) {

    cv::Mat thresholded;

    switch (type) {
    case OTSU:
        cv::threshold(frame, thresholded, 0.0, 255.0, cv::THRESH_OTSU);
        cv::bitwise_not(thresholded, thresholded);
        break;

    case ADAPTATIVE:
        cv::adaptiveThreshold(frame, thresholded, 255.0, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 99, 1);
        break;

    default:
        break;
    }

    out.getMat() = thresholded;
}
