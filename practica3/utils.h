
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <iostream>

#ifndef UTILS_H
#define UTILS_H

enum tipoThreshold {
    OTSU,
    ADAPTATIVE
};

struct Descriptor {
    float firstHuMoment = 0.0;
    float secondHuMoment = 0.0;
    float thirdHuMoment = 0.0;
    float area = 0.0;
    float perimeter = 0.0;

    std::string to_string() const {
        return std::to_string(perimeter)
               + "\t" + std::to_string(area)
               + "\t" + std::to_string(firstHuMoment)
               + "\t" + std::to_string(secondHuMoment)
               + "\t" + std::to_string(thirdHuMoment) + "\n";
    }
};

std::ostream& operator<<(std::ostream& strm, const Descriptor& d) {
    return strm << d.to_string();
}

Descriptor getDescriptors(cv::Mat frame);

cv::Mat getDescriptorCalcs(cv::Mat frame);

cv::Mat getConnectComponents(cv::Mat frame);

cv::Mat thresholding(cv::Mat frame, tipoThreshold type);

#endif
