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
        : perimeter(_perimeter), area(_area),
          firstHuMoment(_firstHuMoment), secondHuMoment(_secondHuMoment), thirdHuMoment(_thirdHuMoment) {}

    Descriptors(std::array<float, 5> _data) : data(_data) {}
};

inline std::ostream& operator<<(std::ostream& os, const Descriptors& d) {
    os << d.data[0];
    for (int i = 1; i < d.data.size(); i++) {
        os << ' ' << d.data[i];
    }
    return os;
}

inline std::istream& operator>>(std::istream& is, Descriptors& d) {
    for (int i = 0; i < d.data.size(); i++) {
        is >> d.data[i];
    }
    return is;
}

// Calcula los descriptores de una imagen binaria en la que se supone que sólo hay un objeto
Descriptors descriptors(cv::InputArray array);

void drawContours(cv::InputArray frame, cv::OutputArray out);

void drawConnectedComponents(cv::InputArray frame, cv::OutputArray out);

void thresholding(cv::InputArray frame, cv::OutputArray out, ThresholdType type);

// Devuelve un vector con cada componente conexa de frame separada en una imagen binaria
void getConnectedComponents(cv::InputArray frame, std::vector<cv::Mat>& components);

// Añade sobre la imagen de entrada el número de cada componente conexa del vector components
void drawComponentsNumbers(cv::InputArray frame, cv::OutputArray out, const std::vector<cv::Mat>& components);
