#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include "utils.h"

int main(int argc, char* argv[]) {

    std::string image_path = cv::samples::findFile("../../images/imagenesL3/reco1.pgm");

    cv::Mat thresholded, frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    cv::imshow("Frame-pre-ADAPTATIVE", frame);
    thresholded = thresholding(frame, ADAPTATIVE);
    cv::imshow("Frame-post-ADAPTATIVE", thresholded);

    cv::imshow("Frame-pre-OTSU", frame);
    thresholded = thresholding(frame, OTSU);
    cv::imshow("Frame-post-OTSU", thresholded);

    cv::Mat connected;
    connected = getConnectComponents(thresholded);
    cv::imshow("Connected components", connected);

    cv::Mat contours;
    contours = getDescriptorCalcs(thresholded);
    cv::imshow("Contornos encontrados", contours);

    cv::waitKey();
    return 0;
}
