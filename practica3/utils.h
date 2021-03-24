
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

struct descriptores {
    float fHuMoment = 0.0;
    float secHuMoment = 0.0;
    float thirdHuMoment = 0.0;
    float area = 0.0;
    float perimeter = 0.0;

    std::string to_string(){
        return std::to_string(perimeter) + "\t" + std::to_string(area) + "\t" + std::to_string(fHuMoment)  + "\t"
             + std::to_string(secHuMoment) + "\t" + std::to_string(thirdHuMoment) + "\n";
    }
}; 

descriptores getDescriptors(cv::Mat frame);

cv::Mat getDescriptorCalcs(cv::Mat frame);

cv::Mat getConnectComponents(cv::Mat frame);

cv::Mat thresholding(cv::Mat frame, tipoThreshold type);

#endif