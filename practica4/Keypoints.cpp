#include <opencv2/opencv.hpp>

#include "HARRISPair.h"
#include "ORBPair.h"
#include "SIFTPair.h"
#include "AKAZEPair.h"
#include "Ransac.h"

#ifdef HAVE_XFEATURES2D_NONFREE_H
#include "SURFPair.h"
#endif

// https://answers.opencv.org/question/65222/is-there-a-way-to-keep-imwrite-from-overwriting-files/
const cv::String& imwriteSafe(const cv::String& filename, cv::InputArray img,
                              const std::vector<int>& params = std::vector<int>()) {

    static FILE* f;
    static cv::String newname;

    f = fopen(filename.c_str(), "rb");
    newname = filename;

    if (f) {
        int counter = 0;
        const int extension_dot = filename.find_first_of('.');
        cv::String name = filename.substr(0, extension_dot);
        cv::String extension = filename.substr(extension_dot);
        do {
            ++counter;
            newname = (name + "_" + std::to_string(counter) + extension);
            fclose(f);
            f = fopen(newname.c_str(), "rb");
        } while (f);
    }

    if (cv::imwrite(newname, img, params))
        return newname;
    else
        throw 0;
}

void drawHomographyBox(const cv::Mat& img, cv::Mat& dst, const cv::Mat& homography) {
    cv::Mat esquinas = cv::Mat::ones(3, 4, CV_64F);
    esquinas.at<double>(0, 0) = 0.0;
    esquinas.at<double>(1, 0) = 0.0;
    esquinas.at<double>(0, 1) = img.cols;
    esquinas.at<double>(1, 1) = 0.0;
    esquinas.at<double>(0, 2) = 0.0;
    esquinas.at<double>(1, 2) = img.rows;
    esquinas.at<double>(0, 3) = img.cols;
    esquinas.at<double>(1, 3) = img.rows;

    cv::Mat resul = homography * esquinas;
    vector<Point> pt(4);
    for (int i = 0; i < 4; ++i) {
        pt[i].x = resul.at<double>(0, i) / resul.at<double>(2, i);
        pt[i].y = resul.at<double>(1, i) / resul.at<double>(2, i);
    }
    dst = img.clone();
    cv::line(dst, pt[0], pt[1], Scalar(255, 255, 255));
    cv::line(dst, pt[0], pt[2], Scalar(255, 255, 255));
    cv::line(dst, pt[3], pt[1], Scalar(255, 255, 255));
    cv::line(dst, pt[3], pt[2], Scalar(255, 255, 255));
}

// Superponer img1 sobre img2
void drawHomographyOverlay(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& dst, const cv::Mat& homography, float alpha = 0.5) {
    // Máscara
    cv::Mat imgMask = cv::Mat(img1.size(), CV_8UC1, cv::Scalar(255));
    cv::warpPerspective(imgMask, imgMask, homography, img2.size());

    // Warp img1
    cv::warpPerspective(img1, dst, homography, img2.size());
    // Mezclar las dos imágenes
    dst = img2 * alpha + dst * (1 - alpha);
}

std::vector<cv::Point2f> get_esquinas(const cv::Mat& img) {

    int cols = img.cols;
    int rows = img.rows;
    std::vector<cv::Point2f> pt(4);
    pt[0] = cv::Point2f(0, 0);
    pt[1] = cv::Point2f(0, rows);
    pt[2] = cv::Point2f(cols, rows);
    pt[3] = cv::Point2f(cols, 0);
    return pt;
}

cv::Mat get_identity() {
    cv::Mat ident = cv::Mat::zeros(3, 3, CV_64F);
    ident.at<double>(0, 0) = 1;
    ident.at<double>(1, 1) = 1;
    ident.at<double>(2, 2) = 1;
    return ident;
}

// Seguido tutorial https://stackoverflow.com/questions/13063201/how-to-show-the-whole-image-when-using-opencv-warpperspective
cv::Mat warpImages(cv::Mat& img1, cv::Mat& img2, cv::Mat& homography) {

    std::vector<cv::Point2f> pt_im1 = get_esquinas(img2);
    std::vector<cv::Point2f> pt_im2_dest, pt_im2 = get_esquinas(img1);
    cv::perspectiveTransform(pt_im2, pt_im2_dest, homography);
    pt_im1.insert(pt_im1.end(), pt_im2_dest.begin(), pt_im2_dest.end());

    int xmin = INT_MAX, xmax = INT_MIN;
    int ymin = INT_MAX, ymax = INT_MIN;
    for (int i = 0; i < pt_im1.size(); ++i) {
        if (pt_im1[i].x < xmin)
            xmin = pt_im1[i].x;
        if (pt_im1[i].x > xmax)
            xmax = pt_im1[i].x;
        if (pt_im1[i].y < ymin)
            ymin = pt_im1[i].y;
        if (pt_im1[i].y > ymax)
            ymax = pt_im1[i].y;
    }

    cv::Mat translation = get_identity();
    translation.at<double>(0, 2) = -xmin;
    translation.at<double>(1, 2) = -ymin;
    cv::Mat result, modification = translation * homography;
    cv::warpPerspective(img1, result, modification, cv::Size(xmax - xmin, ymax - ymin));
    img2.copyTo(result.rowRange(-ymin, -ymin + img2.rows).colRange(-xmin, -xmin + img2.cols));
    return result;
}

void do_panorama(cv::Mat& img1, cv::Mat& img2, cv::Mat& img_panorama) {

    std::shared_ptr<Pair> pair = std::make_shared<AKAZEPair>(img1, img2);
    pair->getMatchesApplyNNRatio(0.8);
    // std::cout << "Núm matches: " << pair->matched1.size() << std::endl;
    std::vector<cv::DMatch> matches = pair->getMatchArray();

    Ransac robust_solver(pair->matched1, pair->matched2);
    cv::Mat resultado, homography = robust_solver.execute();

    // cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, resultado);
    // cv::imshow("Resultado", resultado);
    // cv::waitKey(0);

    img_panorama = warpImages(img1, img2, homography);
}

int main(int argc, char** argv) {

    cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_GRAYSCALE);
    cv::Mat img3 = cv::imread("../../images/BuildingScene/building3.JPG", cv::IMREAD_GRAYSCALE);
    cv::Mat img4 = cv::imread("../../images/BuildingScene/building4.JPG", cv::IMREAD_GRAYSCALE);
    cv::Mat img5 = cv::imread("../../images/BuildingScene/building5.JPG", cv::IMREAD_GRAYSCALE);

    if (img1.empty() || img2.empty())
        return -1;

    cv::Mat panorama;
    do_panorama(img3, img4, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);

    do_panorama(panorama, img2, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);

    do_panorama(panorama, img1, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);

    do_panorama(panorama, img5, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);
}

/*
int main(int argc, char* argv[]) {

    cv::VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        std::cout << "Failed to connect to camera." << std::endl;
        return 1;
    }


    std::cout << "Se tomarán imágenes en vivo." << std::endl;
    
    cv::Mat frame, panorama, new_panorama;
    cap >> panorama;
    int i = 1;
    while (true) {

        cap >> frame;
        if (frame.empty()) // end of video stream
            break;

        if(i % 75 == 0){
            do_panorama(frame, panorama, new_panorama);
            panorama = new_panorama.clone();
        }

        if(i % 375 == 0)
            break;

        i++;
    }

    cv::imshow("Frame", panorama);
    cv::waitKey(0);
    cap.release();
    return 0;
}*/
