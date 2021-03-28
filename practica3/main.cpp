#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "descriptors.hpp"

int main(int argc, char* argv[]) {

    std::string image_path = cv::samples::findFile("../../images/imagenesL3/reco1.pgm");

    cv::Mat thresholded, frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

    cv::imshow("Frame-pre-ADAPTATIVE", frame);
    thresholding(frame, thresholded, ADAPTATIVE);
    cv::imshow("Frame-post-ADAPTATIVE", thresholded);

    cv::imshow("Frame-pre-OTSU", frame);
    thresholding(frame, thresholded, OTSU);
    cv::imshow("Frame-post-OTSU", thresholded);

    cv::Mat connected;
    drawConnectedComponents(thresholded, connected);
    cv::imshow("Connected components", connected);

    cv::Mat contours;
    drawContours(thresholded, contours);
    cv::imshow("Contornos encontrados", contours);

    cv::waitKey();
    return 0;
}
