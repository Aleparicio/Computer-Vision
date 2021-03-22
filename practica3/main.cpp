#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

enum tipoThreshold {
    OTSU,
    ADAPTATIVE
};

cv::Mat getConnectComponents(cv::Mat frame) {
    cv::Mat labels;
    int nlabels = cv::connectedComponents(frame, labels, 4, CV_32S);
    std::vector<cv::Vec3b> colors(nlabels);
    colors[0] = cv::Vec3b(0, 0, 0); //background
    for (int label = 1; label < nLabels; ++label) {
        colors[label] = cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
    }
    at dst(frame.size(), CV_8UC3);
    for (int r = 0; r < dst.rows; ++r) {
        for (int c = 0; c < dst.cols; ++c) {
            int label = labelImage.at<int>(r, c);
            Vec3b& pixel = dst.at<Vec3b>(r, c);

            pixel = colors[label];
        }
    }
    imshow("Connected Components", dst);
}

return labels;
}

cv::Mat thresholding(cv::Mat frame, tipoThreshold type) {

    cv::Mat thresholded;

    switch (type) {
    case OTSU:
        cv::threshold(frame, thresholded, 0.0, 255.0, cv::THRESH_OTSU);
        cv::bitwise_not(thresholded, thresholded);
        break;

    case ADAPTATIVE:
        cv::adaptiveThreshold(frame, thresholded, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 7, 20);
        break;

    default:
        break;
    }

    return thresholded;
}

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
    cv::waitKey();
    return 0;
}
