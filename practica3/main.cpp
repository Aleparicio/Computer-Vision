#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <iostream>

enum tipoThreshold {
    OTSU,
    ADAPTATIVE
};


cv::Mat getDescriptorCalcs(cv::Mat frame){

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours( frame, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Moments> momentos(contours.size() );
    std::vector<double> perimeters(contours.size(), 0.0);
    std::vector<double> area(contours.size(), 0.0);
    double momentosHu[contours.size()][7];

    for( size_t i = 0; i < contours.size(); i++ ){
        momentos[i] = moments( contours[i] );
        cv::HuMoments(momentos[i], momentosHu[i]);
        perimeters[i] = arcLength( contours[i], true );
        area[i] = contourArea(contours[i]);
    }

    // https://docs.opencv.org/3.4/d0/d49/tutorial_moments.html
    cv::Mat drawing = cv::Mat::zeros( frame.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ ) {
        cv::Scalar color = cv::Scalar( rand() % 256, rand() % 256, rand() % 256 );
        drawContours( drawing, contours, (int)i, color, 2 );
    }

    std::cout << "\t Info: Area and Contour Length \n";
    for( size_t i = 0; i < contours.size(); i++ ){
        std::cout << " * Contour[" << i << "] " << " - Area OpenCV: " << area[i]
                  << " - Length: " << perimeters[i] << std::endl;
    }

    return drawing;
}


// https://answers.opencv.org/question/168403/connected-components-with-opencv/
cv::Mat getConnectComponents(cv::Mat frame){

    srand(time(NULL));
    cv::Mat labels;
    int nlabels = cv::connectedComponents(frame, labels, 4, CV_32S); 

    std::vector<cv::Vec3b> colores(nlabels, cv::Vec3b(0, 0, 0));
    for (int i = 1; i < nlabels; ++i) 
        colores[i] = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);

    cv::Mat dst(frame.size(), CV_8UC3);
    for (int row = 0; row < dst.rows; ++row) {
        for (int col = 0; col < dst.cols; ++col) {
            int label = labels.at<int>(row, col);
            cv::Vec3b &pixel = dst.at<cv::Vec3b>(row, col);
            pixel = colores[label];
        }
    }

    return dst;
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

    cv::Mat contours;
    contours = getDescriptorCalcs(thresholded);
    cv::imshow("Contornos encontrados", contours);


    cv::waitKey();
    return 0;
}
