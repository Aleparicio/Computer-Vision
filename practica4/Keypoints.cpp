#include "Panorama.h"

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
* /

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
}
*/
