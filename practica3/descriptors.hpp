#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <array>

enum ThresholdType {
    OTSU,
    ADAPTATIVE
};

struct Descriptors {
    union {
        std::array<float, 5> data = {0};

        struct {
            float perimeter;
            float area;
            float firstHuMoment;
            float secondHuMoment;
            float thirdHuMoment;
        };
    };

    Descriptors() {}

    Descriptors(float _perimeter, float _area, float _firstHuMoment, float _secondHuMoment, float _thirdHuMoment)
        : area(_area), perimeter(_perimeter),
          firstHuMoment(_firstHuMoment), secondHuMoment(_secondHuMoment), thirdHuMoment(_thirdHuMoment) {}

    Descriptors(std::array<float, 5> _data) : data(_data) {}
};

inline std::ostream& operator<<(std::ostream& os, const Descriptors& d) {
    for (int i = 0; i < d.data.size(); i++) {
        os << d.data[i] << ' ';
    }
    return os;
}

inline std::istream& operator>>(std::istream& is, Descriptors& d) {
    for (int i = 0; i < d.data.size(); i++) {
        is >> d.data[i];
    }
    return is;
}

// Get file descriptors
Descriptors descriptors(cv::InputArray array);

// Get image contours
void drawContours(cv::InputArray frame, cv::Mat& out);

void drawConnectedComponents(cv::InputArray frame, cv::Mat& out);

void thresholding(cv::InputArray frame, cv::Mat& thresholded, ThresholdType type);
