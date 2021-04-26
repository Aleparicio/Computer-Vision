#include <opencv2/opencv.hpp>

#include "HARRISPair.h"
#include "ORBPair.h"
#include "SIFTPair.h"
#include "AKAZEPair.h"
#include "Ransac.h"

#ifdef HAVE_XFEATURES2D_NONFREE_H
#include "SURFPair.h"
#endif

void fillImages(std::vector<cv::Mat>& imgs) {

    imgs.clear();

    cv::Mat img1 = cv::imread("../../images/chessboard/left01.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("../../images/chessboard/left02.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img3 = cv::imread("../../images/chessboard/left03.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img4 = cv::imread("../../images/chessboard/left04.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img5 = cv::imread("../../images/chessboard/left05.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img6 = cv::imread("../../images/chessboard/left06.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img7 = cv::imread("../../images/chessboard/left07.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img8 = cv::imread("../../images/chessboard/left08.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img9 = cv::imread("../../images/chessboard/left09.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img11 = cv::imread("../../images/chessboard/left11.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img12 = cv::imread("../../images/chessboard/left12.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img13 = cv::imread("../../images/chessboard/left13.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img14 = cv::imread("../../images/chessboard/left14.jpg", cv::IMREAD_GRAYSCALE);

    imgs.push_back(img1);
    imgs.push_back(img2);
    imgs.push_back(img3);
    imgs.push_back(img4);
    imgs.push_back(img5);
    imgs.push_back(img6);
    imgs.push_back(img7);
    imgs.push_back(img8);
    imgs.push_back(img9);
    imgs.push_back(img11);
    imgs.push_back(img12);
    imgs.push_back(img13);
    imgs.push_back(img14);
}

int main(int argc, char** argv) {

    Size patternsize(9, 6); //interior number of corners
    std::vector<cv::Mat> imagenes;
    fillImages(imagenes);

    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;

    std::vector<cv::Point3f> calibrate_points;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 9; ++j) {
            calibrate_points.push_back(cv::Point3f(j, i, 0));
        }
    }

    for (int i = 0; i < imagenes.size(); ++i) {

        cv::Mat actual_img = imagenes[i];
        std::vector<cv::Point2f> corners;

        bool patternfound = findChessboardCorners(actual_img, patternsize, corners,
                                                  cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
                                                      + cv::CALIB_CB_FAST_CHECK);

        if (patternfound) {
            cv::cornerSubPix(actual_img, corners, patternsize, Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

            object_points.push_back(calibrate_points);
            image_points.push_back(corners);

            //cv::Mat to_draw;
            //cv::cvtColor(actual_img.clone(), to_draw, cv::COLOR_GRAY2BGR);
            //for(int i = 0; i < corners.size(); ++i)
            //    cv::circle(to_draw, corners[i], 3, Scalar(0,0,255), 1);
            //cv::imwrite("drawed.jpg", to_draw);
        }
    }

    cv::Mat undistorted_img1, img1 = cv::imread("../../images/chessboard/left01.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat distCoeffs, intrinsic = Mat(3, 3, CV_32FC1);
    vector<Mat> rvecs, tvecs;
    double rms = cv::calibrateCamera(object_points, image_points, img1.size(), intrinsic, distCoeffs, rvecs, tvecs);

    cv::imwrite("chess1.jpg", img1);
    undistort(img1, undistorted_img1, intrinsic, distCoeffs);
    cv::imwrite("chess2.jpg", undistorted_img1);

    //cv::drawChessboardCorners(img1, patternsize, Mat(corners), patternfound);
    //cv::imwrite("sa.jpg", img1);
    return 0;
}
