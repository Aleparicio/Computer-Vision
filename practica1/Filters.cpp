#include "Filters.h"

// Contrast

Contrast::Contrast(std::string winname) {
    gain = 0;
    bias = 0;
    cv::createTrackbar("Gain", winname, &gain, MAX_GAIN, NULL);
    cv::createTrackbar("Bias", winname, &bias, MAX_BIAS, NULL);
}

void Contrast::apply(cv::Mat& img) const {
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    img = gain * img;
    img = img + bias;
}

// Histogram Equalization

HistogramEqualization::HistogramEqualization(std::string winname) {}

void HistogramEqualization::apply(cv::Mat& image) const {
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(image, image);
}

// CLAHE

CLAHE::CLAHE(std::string winname) {
    clipLimit = 0;
    cv::createTrackbar("Clip limit", winname, &clipLimit, MAX_CLIP_LIMIT, NULL);
}

void CLAHE::apply(cv::Mat& image) const {
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clipLimit);
    clahe->apply(image, image);
}

// Posterize

Posterization::Posterization(std::string winname) {
    numColors = 1;
    cv::createTrackbar("Colors", winname, &numColors, MAX_NUM_COLORS, NULL);
}

int Posterization::reduce(int value, int numColors) const {
    // Size of each interval
    int size = 255 / numColors;
    return ((value / size)) * size;
}

void Posterization::apply(cv::Mat& image) const {
    for (int i = 0; i < image.rows; i++) {
        uchar* data = image.ptr<uchar>(i);
        for (int j = 0; j < image.cols * image.channels(); j++) {
            data[j] = reduce(data[j], numColors);
            // *data ++= *data/div*div + div/2;
        }
    }
}

// void Posterization::apply(cv::Mat& image) const {
//     for (int i = 0; i < image.rows; i++) {
//         uchar* data = image.ptr<uchar>(i);
//         for (int j = 0; j < image.cols * image.channels(); j++) {
//             // data[j] = data[j]/div*div + div/2
//             *data++ = *data / numColors * numColors + numColors / 2;
//         }
//     }
// }

// Alien
Alien::Alien(std::string winname) {
    R = G = B = alpha = 0;
    cv::createTrackbar("R", winname, &R, 255, NULL);
    cv::createTrackbar("G", winname, &G, 255, NULL);
    cv::createTrackbar("B", winname, &B, 255, NULL);
    cv::createTrackbar("alpha", winname, &alpha, 255, NULL);
}

void Alien::apply(cv::Mat& img) const {
}

// Distortion

Distortion::Distortion(std::string winname) {
}

void Distortion::apply(cv::Mat& img) const {
}
