#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "descriptors.hpp"

int main(int argc, char* argv[]) {

    std::string image_path = cv::samples::findFile("../../images/imagenesL3/reco1.pgm");

    cv::Mat thresholded, frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

    cv::imshow("Frame-pre-ADAPTATIVE", frame);
    cv::imwrite("original.png", frame);
    thresholding(frame, thresholded, ADAPTATIVE);
    cv::imshow("Frame-post-ADAPTATIVE", thresholded);
    cv::imwrite("adapatative.png", thresholded);

    cv::imshow("Frame-pre-OTSU", frame);
    thresholding(frame, thresholded, OTSU);
    cv::imshow("Frame-post-OTSU", thresholded);
    cv::imwrite("otsu.png", thresholded);

    cv::Mat connected;
    drawConnectedComponents(thresholded, connected);
    cv::imshow("Connected components", connected);
    cv::imwrite("Connected.png", connected);

    cv::Mat contours;
    drawContours(thresholded, contours);
    cv::imshow("Contornos encontrados", contours);
    cv::imwrite("Contornos.png", contours);

    std::vector<cv::Mat> components;
    getConnectedComponents(thresholded, components);

    for (int i = 0; i < components.size(); i++) {
        cv::imshow("Componentes " + std::to_string(i), components[i]);
        drawConnectedComponents(components[i], connected);
        cv::imshow("Componentes conexas " + std::to_string(i), connected);
        drawContours(components[i], contours);
        cv::imshow("Contornos " + std::to_string(i), contours);
        cv::waitKey();
    }

    cv::waitKey();

    return 0;
}
