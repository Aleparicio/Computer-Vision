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

std::vector<cv::Point2f> getCorners(const cv::Mat& img) {

    int cols = img.cols;
    int rows = img.rows;
    std::vector<cv::Point2f> corners(4);
    corners[0] = cv::Point2f(0, 0);
    corners[1] = cv::Point2f(0, rows);
    corners[2] = cv::Point2f(cols, rows);
    corners[3] = cv::Point2f(cols, 0);
    return corners;
}

enum BlendType {
    NONE,
    LINEAR,
    FEATHER,
    MULTI_BAND
};

// Seguido tutorial https://stackoverflow.com/questions/13063201/how-to-show-the-whole-image-when-using-opencv-warpperspective
cv::Mat warpImages(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& homography, BlendType blend_type = BlendType::NONE) {

    // Transformar img1 según homography y colocarle encima img2

    std::vector<cv::Point2f> corners_img1 = getCorners(img1);
    std::vector<cv::Point2f> corners_img2 = getCorners(img2);

    std::vector<cv::Point2f> corners_img1_warped;
    cv::perspectiveTransform(corners_img1, corners_img1_warped, homography);

    std::vector<cv::Point2f> all_corners = corners_img1_warped;
    all_corners.insert(all_corners.end(), corners_img2.begin(), corners_img2.end());

    // Buscar las dimensiones de la imagen final
    int xmin = INT_MAX, xmax = INT_MIN;
    int ymin = INT_MAX, ymax = INT_MIN;
    for (auto corner : all_corners) {
        xmin = std::min((int)corner.x, xmin);
        xmax = std::max((int)corner.x, xmax);
        ymin = std::min((int)corner.y, ymin);
        ymax = std::max((int)corner.y, ymax);
    }

    // Crear matriz de translación
    cv::Mat translation = cv::Mat::eye(3, 3, CV_64F);
    translation.at<double>(0, 2) = -xmin;
    translation.at<double>(1, 2) = -ymin;

    cv::Mat mask_result1 = cv::Mat(img1.size(), CV_8UC1, cv::Scalar(255));
    cv::Mat mask_result2 = cv::Mat(img2.size(), CV_8UC1, cv::Scalar(255));

    std::vector<cv::Point2f> corners_result1;
    std::vector<cv::Point2f> corners_result2;

    cv::Size result_size = cv::Size(xmax - xmin, ymax - ymin);

    cv::Mat result1, result2;
    // cv::warpPerspective(img1, result1, translation * homography, result_size, cv::INTER_LINEAR, cv::BORDER_REFLECT);
    cv::warpPerspective(img1, result1, translation * homography, result_size);
    cv::warpPerspective(mask_result1, mask_result1, translation * homography, result_size);
    cv::perspectiveTransform(corners_img1, corners_result1, translation * homography);

    // cv::warpPerspective(img2, result2, translation, result_size, cv::INTER_LINEAR, cv::BORDER_REFLECT);
    cv::warpPerspective(img2, result2, translation, result_size);
    cv::warpPerspective(mask_result2, mask_result2, translation, result_size);
    cv::perspectiveTransform(corners_img2, corners_result2, translation);

    cv::Mat mask_and;
    cv::bitwise_and(mask_result1, mask_result2, mask_and);

    // Mezclar las dos imágenes
    cv::Mat result;

    if (blend_type == BlendType::LINEAR) {
        cv::addWeighted(result1, 0.5, result2, 0.5, 0, result);
        cv::copyTo(result1, result, mask_result1 - mask_and);
        cv::copyTo(result2, result, mask_result2 - mask_and);

    } else if (blend_type == BlendType::MULTI_BAND || blend_type == BlendType::FEATHER) {
        cv::detail::Blender blender;
        if (blend_type == BlendType::MULTI_BAND) {
            blender = cv::detail::MultiBandBlender(false, 5);
        } else {
            blender = cv::detail::FeatherBlender(0.019999);
        }
        cv::Mat result1_s, result2_s;
        result1.convertTo(result1_s, CV_16S);
        result2.convertTo(result2_s, CV_16S);
        blender.prepare(std::vector<Point>({corners_img1[0], corners_img2[0]}), std::vector<Size>({result_size, result_size}));
        blender.feed(result1_s, mask_result1, corners_img1[0]);
        blender.feed(result2_s, mask_result2, corners_img2[0]);

        cv::Mat result_blend, result_mask;
        blender.blend(result_blend, result_mask);
        result_blend.convertTo(result, CV_8U);

    } else {
        result = result1;
        result2.copyTo(result, mask_result2);
    }

    return result;
}

void doPanorama(cv::Mat& img1, cv::Mat& img2, cv::Mat& img_panorama, BlendType blend_type = BlendType::NONE) {
    cv::Mat img1_gray, img2_gray;

    cv::cvtColor(img1, img1_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);

    std::shared_ptr<Pair> pair = std::make_shared<AKAZEPair>(img1_gray, img2_gray);
    // std::shared_ptr<Pair> pair = std::make_shared<SIFTPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<SURFPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<ORBPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<HARRISPair>(img1, img2);
    pair->getMatchesApplyNNRatio(0.8);
    // std::cout << "Núm matches: " << pair->matched1.size() << std::endl;

    Ransac robust_solver(pair->matched1, pair->matched2);
    cv::Mat homography = robust_solver.execute();

    // std::vector<cv::Point2f> puntos_1, puntos_2;
    // for (int i = 0; i < pair->matched1.size(); i++) {
    //     puntos_1.push_back(pair->matched1[i].pt);
    //     puntos_2.push_back(pair->matched2[i].pt);
    // }
    // cv::Mat homography = cv::findHomography(puntos_1, puntos_2, cv::RANSAC);

    cv::Mat resultado;
    std::vector<cv::DMatch> matches = pair->getMatchArray();
    cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, resultado);
    cv::imshow("Resultado", resultado);

    img_panorama = warpImages(img1, img2, homography, blend_type);
}

/*
int main(int argc, char** argv) {

    // cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_GRAYSCALE);
    // cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_GRAYSCALE);
    // cv::Mat img3 = cv::imread("../../images/BuildingScene/building3.JPG", cv::IMREAD_GRAYSCALE);
    // cv::Mat img4 = cv::imread("../../images/BuildingScene/building4.JPG", cv::IMREAD_GRAYSCALE);
    // cv::Mat img5 = cv::imread("../../images/BuildingScene/building5.JPG", cv::IMREAD_GRAYSCALE);

    cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_COLOR);
    cv::Mat img3 = cv::imread("../../images/BuildingScene/building3.JPG", cv::IMREAD_COLOR);
    cv::Mat img4 = cv::imread("../../images/BuildingScene/building4.JPG", cv::IMREAD_COLOR);
    cv::Mat img5 = cv::imread("../../images/BuildingScene/building5.JPG", cv::IMREAD_COLOR);

    // cv::Mat img1_gray, img2_gray, img3_gray, img4_gray, img5_gray;
    // cv::cvtColor(img1, img1_gray, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(img3, img3_gray, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(img4, img4_gray, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(img5, img5_gray, cv::COLOR_BGR2GRAY);

    if (img1.empty() || img2.empty())
        return -1;

    // std::vector<cv::Mat> imgs{img1, img2, img3, img4, img5};
    // cv::Mat pano;
    // cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    // cv::Stitcher::Status status = stitcher->stitch(imgs, pano);
    // cv::imshow("Prueba", pano);
    // cv::waitKey(0);
    // exit(1);

    cv::Mat panorama;
    doPanorama(img3, img4, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);

    doPanorama(panorama, img2, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);

    doPanorama(panorama, img1, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);

    doPanorama(panorama, img5, panorama);
    cv::imshow("Homografia", panorama);
    cv::waitKey(0);
}
*/


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
            doPanorama(frame, panorama, new_panorama);
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

int main(int argc, char** argv) {

    // cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_COLOR);
    // cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_COLOR);

    cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_UNCHANGED);
    cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_UNCHANGED);

    std::cout << img1.type() << std::endl;

    cv::imshow("Imagen sin convertir", img1);
    // img1.convertTo(img1, CV_16SC3, 255);
    cv::imshow("Imagen convertida", img1);
    cv::waitKey(0);

    // exit(1);

    if (img1.empty() || img2.empty())
        return -1;

    cv::Mat img1_gray, img2_gray;
    cv::cvtColor(img1, img1_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);

    std::shared_ptr<Pair> pair = std::make_shared<HARRISPair>(img1_gray, img2_gray);
    // std::shared_ptr<Pair> pair = std::make_shared<ORBPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<SIFTPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<SURFPair>(img1, img2);
    // std::shared_ptr<Pair> pair = std::make_shared<AKAZEPair>(img1, img2);

    pair->getMatchesApplyNNRatio(0.8);

    std::cout << "Núm matches: " << pair->matched1.size() << std::endl;

    std::vector<cv::DMatch> matches(pair->matched1.size());
    for (int i = 0; i < pair->matched1.size(); ++i)
        matches[i] = DMatch(i, i, 0);

    Ransac robust_solver(pair->matched1, pair->matched2);
    cv::Mat homography = robust_solver.execute();

    cv::Mat matches_resul, homography_resul;
    cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, matches_resul);

    // drawHomographyBox(img2, homography_resul, homography);
    // drawHomographyOverlay(img1, img2, homography_resul, homography);
    // warpImages(img1, img2, homography_resul, homography);
    homography_resul = warpImages(img1, img2, homography, BlendType::LINEAR);
    cv::imshow("Keypoints", matches_resul);
    cv::imshow("Homografia", homography_resul);
    cv::waitKey(0);
}


/*
int main(int argc, char* argv[]) {

    cv::Mat img1 = cv::imread("../../images/BuildingScene/building1.JPG", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("../../images/BuildingScene/building2.JPG", cv::IMREAD_GRAYSCALE);

    std::shared_ptr<Pair> pair = std::make_shared<SIFTPair>(img1, img2);
    pair->flannMatchesNNRatio(0.8);
    std::vector<cv::DMatch> matches = pair->getMatchArray();

    cv::Mat resultado;
    cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, resultado);
    cv::imshow("Resultado", resultado);
    cv::waitKey(0);

    return 0;
}*/