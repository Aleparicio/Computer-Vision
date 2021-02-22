#include "Filters.h"

void contrastAdjustment(cv::Mat& image, float gain, float sesgo){
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    image = gain*image;
    image = image+sesgo;
}

void equalizeHist(cv::Mat& image){
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(image, image);
}

void equalizeCLAHE(cv::Mat& image, float clipLimit){
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clipLimit);
    clahe->apply(image, image);
}