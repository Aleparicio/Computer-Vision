#include "Panorama.h"

// https://answers.opencv.org/question/65222/is-there-a-way-to-keep-imwrite-from-overwriting-files/
const cv::String& imwriteSafe(const cv::String& filename, cv::InputArray img, const std::vector<int>& params) {

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
void drawHomographyOverlay(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& dst, const cv::Mat& homography, float alpha) {
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

// Seguido tutorial https://stackoverflow.com/questions/13063201/how-to-show-the-whole-image-when-using-opencv-warpperspective
cv::Mat warpImages(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& homography, BlendType blend_type, SeamType seam_type) {
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

    // Obtener máscaras iniciales para cada imágen (píxeles con información)

    // cv::Mat mask_result1 = cv::Mat(img1.size(), CV_8UC1, cv::Scalar(255));
    // cv::Mat mask_result2 = cv::Mat(img2.size(), CV_8UC1, cv::Scalar(255));

    // cv::Mat mask_result1 = img1 > 0;
    // cv::Mat mask_result2 = img2 > 0;
    // cv::cvtColor(mask_result1, mask_result1, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(mask_result2, mask_result2, cv::COLOR_BGR2GRAY);

    std::vector<cv::Point2f> corners_result1;
    std::vector<cv::Point2f> corners_result2;

    // Tamaño de la imagen final
    cv::Size result_size = cv::Size(xmax - xmin, ymax - ymin);

    cv::Mat result1, result2;
    // cv::warpPerspective(img1, result1, translation * homography, result_size, cv::INTER_LINEAR, cv::BORDER_REFLECT);
    cv::warpPerspective(img1, result1, translation * homography, result_size);
    // cv::warpPerspective(mask_result1, mask_result1, translation * homography, result_size);
    cv::perspectiveTransform(corners_img1, corners_result1, translation * homography);

    // cv::warpPerspective(img2, result2, translation, result_size, cv::INTER_LINEAR, cv::BORDER_REFLECT);
    cv::warpPerspective(img2, result2, translation, result_size);
    // cv::warpPerspective(mask_result2, mask_result2, translation, result_size);
    cv::perspectiveTransform(corners_img2, corners_result2, translation);

    // cv::Mat mask_result1 = img1 > 0;
    // cv::Mat mask_result2 = img2 > 0;
    // cv::cvtColor(mask_result1, mask_result1, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(mask_result2, mask_result2, cv::COLOR_BGR2GRAY);

    cv::Mat mask_result1, mask_result2;

    cv::Mat result1_gray;
    cv::Mat result2_gray;

    cv::cvtColor(result1, result1_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(result2, result2_gray, cv::COLOR_BGR2GRAY);

    cv::threshold(result1_gray, mask_result1, 0, 255, cv::THRESH_BINARY);
    cv::threshold(result2_gray, mask_result2, 0, 255, cv::THRESH_BINARY);
    // cv::cvtColor(mask_result1, mask_result1, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(mask_result2, mask_result2, cv::COLOR_BGR2GRAY);

    // cv::medianBlur(mask_result1, mask_result1, 5);
    // cv::medianBlur(mask_result2, mask_result2, 5);

    cv::Mat or_mask;
    cv::bitwise_or(mask_result1, mask_result2, or_mask);

    // imshow("Mascara 1", mask_result1);
    // imshow("Mascara 2", mask_result2);
    // imshow("Mascara 3", or_mask);
    // imshow("Wrap 1", result1);
    // imshow("Wrap 2", result2);
    // cv::waitKey();

    // Guardar todo en listas

    // cv::resize(mask_result1, mask_result1, result_size, 0, 0, cv::INTER_LINEAR_EXACT);
    // cv::resize(mask_result2, mask_result2, result_size, 0, 0, cv::INTER_LINEAR_EXACT);
    // cv::resize(result1, result1, result_size, 0, 0, cv::INTER_LINEAR_EXACT);
    // cv::resize(result2, result2, result_size, 0, 0, cv::INTER_LINEAR_EXACT);

    // cv::Mat resultt;
    // resultt = result1.clone();
    // result1 = result2.clone();
    // result2 = resultt.clone();

    // resultt = mask_result1.clone();
    // mask_result1 = mask_result2.clone();
    // mask_result2 = resultt.clone();

    std::vector<UMat> images;
    images.push_back(result1.getUMat(ACCESS_RW));
    images.push_back(result2.getUMat(ACCESS_RW));

    std::vector<Point> corners;
    corners.push_back(cv::Point(0, 0));
    corners.push_back(cv::Point(0, 0));

    std::vector<UMat> masks;
    masks.push_back(mask_result1.getUMat(ACCESS_RW));
    masks.push_back(mask_result2.getUMat(ACCESS_RW));

    std::vector<Size> sizes;
    sizes.push_back(result_size);
    sizes.push_back(result_size);

    // Ajustar las máscaras con el buscador de seams
    if (seam_type != SeamType::NO) {
        std::shared_ptr<cv::detail::SeamFinder> seam_finder;
        if (seam_type == SeamType::VORONOI) {
            // cv::detail::VoronoiSeamFinder seam_finder = cv::detail::VoronoiSeamFinder();
            // seam_finder.find(images, corners, masks);
            seam_finder = std::make_shared<cv::detail::VoronoiSeamFinder>();
        } else if (seam_type == SeamType::DP_COLOR) {
            // cv::detail::DpSeamFinder seam_finder = cv::detail::DpSeamFinder(cv::detail::DpSeamFinder::COLOR);
            // seam_finder.find(images, corners, masks);
            seam_finder = std::make_shared<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
        }
        seam_finder->find(images, corners, masks);
    }

    // cv::Mat dilated_mask, seam_mask;
    // dilate(mask_result1, dilated_mask, Mat());
    // resize(dilated_mask, seam_mask, mask_result1.size(), 0, 0, INTER_LINEAR_EXACT);
    // mask_result1 = seam_mask & mask_result1;
    // dilate(mask_result2, dilated_mask, Mat());
    // resize(dilated_mask, seam_mask, mask_result2.size(), 0, 0, INTER_LINEAR_EXACT);
    // mask_result2 = seam_mask & mask_result2;

    // Mezclar las dos imágenes

    cv::Mat result;
    if (blend_type == BlendType::LINEAR) {
        cv::GaussianBlur(mask_result1, mask_result1, cv::Size(3, 3), 2);
        cv::GaussianBlur(mask_result2, mask_result2, cv::Size(3, 3), 2);
        cv::Mat mask_and;
        cv::bitwise_and(mask_result1, mask_result2, mask_and);
        cv::addWeighted(result1, 0.5, result2, 0.5, 0, result);
        cv::copyTo(result1, result, mask_result1 - mask_and);
        cv::copyTo(result2, result, mask_result2 - mask_and);

    } else if (blend_type == BlendType::MULTI_BAND || blend_type == BlendType::FEATHER) {
        cv::Mat result1_s, result2_s;
        result1.convertTo(result1_s, CV_16S);
        result2.convertTo(result2_s, CV_16S);

        std::shared_ptr<cv::detail::Blender> blender;

        if (blend_type == BlendType::MULTI_BAND) {
            float blend_strength = 5;
            Size dst_sz = cv::detail::resultRoi(corners, sizes).size();
            float blend_width = sqrt(static_cast<float>(dst_sz.area())) * blend_strength / 100.f;
            int num_bands = static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.);
            blender = std::make_shared<cv::detail::MultiBandBlender>(false, num_bands);
        } else {
            blender = std::make_shared<cv::detail::FeatherBlender>(0.0199999);
            // blender = std::make_shared<cv::detail::Blender>();
        }
        Rect roi = cv::detail::resultRoi(corners, sizes);
        blender->prepare(roi);
        blender->feed(result1_s, masks[0], corners[0]);
        blender->feed(result2_s, masks[1], corners[1]);
        cv::Mat result_blend, result_mask;
        blender->blend(result_blend, result_mask);
        result_blend.convertTo(result, CV_8U);

    } else {
        int erosion_type = 0;
        int erosion_size = 2;
        Mat element = getStructuringElement(erosion_type,
                                            Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                            Point(erosion_size, erosion_size));
        erode(mask_result2, mask_result2, element);
        result = result1.clone();
        // result1.copyTo(result, mask_result1);
        result2.copyTo(result, mask_result2);
    }

    return result;
}

void doPanorama(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& img_panorama, FeaturesType features_type, float nn_ratio, bool use_flann, BlendType blend_type, SeamType seam_type) {

    cv::Mat img1_gray, img2_gray;
    cv::cvtColor(img1, img1_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);

    // Find matches
    std::shared_ptr<Pair> pair;
    if (features_type == FeaturesType::AKAZE) {
        std::cout << "akaze" << std::endl;
        pair = std::make_shared<AKAZEPair>(img1_gray, img2_gray);
    } else if (features_type == FeaturesType::SIFT) {
        std::cout << "sift" << std::endl;
        pair = std::make_shared<SIFTPair>(img1_gray, img2_gray);

#ifdef HAVE_XFEATURES2D_NONFREE_H
    } else if (features_type == FeaturesType::SURF) {
        std::cout << "surf" << std::endl;
        pair = std::make_shared<SURFPair>(img1_gray, img2_gray);
#endif

    } else if (features_type == FeaturesType::ORB) {
        std::cout << "orb" << std::endl;
        pair = std::make_shared<ORBPair>(img1_gray, img2_gray);
    } else if (features_type == FeaturesType::HARRIS) {
        std::cout << "harris" << std::endl;
        pair = std::make_shared<HARRISPair>(img1_gray, img2_gray);
    }

    if (use_flann) {
        pair->flannMatchesNNRatio(nn_ratio);
    } else {
        pair->getMatchesApplyNNRatio(nn_ratio);
    }
    std::cout << "Núm matches: " << pair->matched1.size() << std::endl;

    // Mostrar keypoints y matches
    cv::Mat img_kpts;
    cv::drawKeypoints(img1, pair->kpts1, img_kpts);
    cv::imshow("Keypoints 1", img_kpts);
    cv::drawKeypoints(img2, pair->kpts2, img_kpts);
    cv::imshow("Keypoints 2", img_kpts);

    cv::Mat img_matches;
    std::vector<cv::DMatch> matches = pair->getMatchArray();
    cv::drawMatches(img1, pair->matched1, img2, pair->matched2, matches, img_matches);
    cv::imshow("Resultado", img_matches);

    // cv::waitKey(0);

    // Hand-made RANSAC
    Ransac robust_solver(pair->matched1, pair->matched2);
    cv::Mat homography = robust_solver.execute();

    // // Built-in RANSAC
    // std::vector<cv::Point2f> puntos_1, puntos_2;
    // for (int i = 0; i < pair->matched1.size(); i++) {
    //     puntos_1.push_back(pair->matched1[i].pt);
    //     puntos_2.push_back(pair->matched2[i].pt);
    // }
    // cv::Mat homography = cv::findHomography(puntos_1, puntos_2, cv::RANSAC);

    // img_panorama = warpImages(img1, img2, homography, blend_type, seam_type);
    img_panorama = warpImages(img1, img2, homography, blend_type, seam_type);
}
