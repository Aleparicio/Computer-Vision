#include <opencv2/opencv.hpp>

#include "Panorama.h"
#include <string>

enum class Mode {
    IMAGES,
    LIVE_KEY,
    LIVE_AUTO
};

struct Args {
    Mode mode = Mode::IMAGES;
    FeaturesType features = FeaturesType::AKAZE;
    BlendType blend = BlendType::NO;
    SeamType seam = SeamType::NO;
    bool use_flann = false;
    float nn_ratio = 0.8;
    int capture_interval = 2000; // milisegundos
    std::vector<std::string> images_files;
};

static void printUsage(char** argv) {
    std::cout << "Uso: " << argv[0]
              << " [--mode MODO] [opciones...]\n\n"
                 "Modos (por defecto images):\n"
                 "  images [imagenes...]\n"
                 "      Componer todas las imágenes que se pasen\n"
                 "  live_key\n"
                 "    Panorama en directo, capturando una imagen cada vez que se pulse una tecla\n"
                 "  live_auto [time]\n"
                 "    Panorama en directo, capturando una imagen cada time segundos. Por defecto 2 segundos\n"
                 "Opciones:\n"
                 "  --features {harris, orb, sift, surf, akaze}\n"
                 "    Tipo de features al hacer matching. Por defecto akaze\n"
                 "  --blend {no, linear, multiband, feather}\n"
                 "    Método para unir las imágenes. Por defecto no\n"
                 "  --seam {no, voronoi, dp_color}\n"
                 "    Tipo de estimación de uniones entre imágenes. Por defecto no\n"
                 "  --use_flann\n"
                 "    Usar FLANN para comparar los descriptores. Por defecto no se usa\n"
                 "  --nn_ratio\n"
                 "    Ratio al segundo vecino. Por defecto 0.8\n"
                 "\nEjemplos:\n"
              << argv[0] << " --mode images ../../images/BuildingScene/building{4,3,5,2,1}.JPG --blend multiband\n"
              << argv[0] << " --mode live_key --blend multiband --seam voronoi\n"
              << argv[0] << " --mode live_auto 5 --features sift\n";
}

static bool parseArgs(int argc, char** argv, Args& args) {
    if (argc == 1) {
        printUsage(argv);
        return false;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--help") {
            printUsage(argv);
            return false;
        } else if (arg == "--mode") {
            std::string mode(argv[i + 1]);
            i++;
            if (mode == "images") {
                args.mode = Mode::IMAGES;
            } else if (mode == "live_key") {
                args.mode = Mode::LIVE_KEY;
            } else if (mode == "live_auto") {
                args.mode = Mode::LIVE_AUTO;
                std::string time(argv[i + 1]);
                if (time.substr(0, 2) != "--") {
                    args.capture_interval = std::stoi(time) * 1000;
                    i++;
                }
            } else {
                std::cout << "Modo desconocido: " << mode << std::endl;
                return false;
            }
        } else if (arg == "--features") {
            std::string features(argv[i + 1]);
            i++;
            if (features == "harris") {
                args.features = FeaturesType::HARRIS;
            } else if (features == "orb") {
                args.features = FeaturesType::ORB;
            } else if (features == "sift") {
                args.features = FeaturesType::SIFT;
#ifdef HAVE_XFEATURES2D_NONFREE_H
            } else if (features == "surf") {
                args.features = FeaturesType::SURF;
#endif
            } else if (features == "akaze") {
                args.features = FeaturesType::AKAZE;
            } else {
                std::cout << "Features desconocidas: " << features << std::endl;
                return false;
            }
        } else if (arg == "--blend") {
            std::string blend(argv[i + 1]);
            i++;
            if (blend == "no") {
                args.blend = BlendType::NO;
            } else if (blend == "linear") {
                args.blend = BlendType::LINEAR;
            } else if (blend == "multiband") {
                args.blend = BlendType::MULTI_BAND;
            } else if (blend == "feather") {
                args.blend = BlendType::FEATHER;
            } else {
                std::cout << "Blend desconocido: " << blend << std::endl;
                return false;
            }
        } else if (arg == "--seam") {
            std::string seam(argv[i + 1]);
            i++;
            if (seam == "no") {
                args.seam = SeamType::NO;
            } else if (seam == "voronoi") {
                args.seam = SeamType::VORONOI;
            } else if (seam == "dp_color") {
                args.seam = SeamType::DP_COLOR;
            } else {
                std::cout << "Seam desconocido: " << seam << std::endl;
                return false;
            }
        } else if (arg == "--use_flann") {
            args.use_flann = true;
        } else if (arg == "--nn_ratio") {
            args.nn_ratio = std::stof(std::string(argv[i + 1]));
            i++;
        } else if (arg.substr(0, 2) != "--") {
            args.images_files.push_back(arg);
        } else {
            std::cout << "Argumento desconocido: " << arg << std::endl;
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    std::string output_name = "panorama.jpg";

    // int screen_width = 4096, screen_height = 2160;
    // int screen_width = 1920, screen_height = 1080;
    int screen_width = 1920, screen_height = 1050;

    cv::namedWindow("Matches", cv::WINDOW_NORMAL);
    cv::resizeWindow("Matches", cv::Size(screen_width, screen_height / 2));
    cv::moveWindow("Matches", 0, screen_height / 2 + 40);

    cv::namedWindow("Panorama", cv::WINDOW_NORMAL);
    cv::resizeWindow("Panorama", cv::Size(screen_width, screen_height / 2));
    cv::moveWindow("Panorama", 0, 0);

    // cv::waitKey(0);

    Args args;
    if (!parseArgs(argc, argv, args)) {
        return 1;
    }

    // cv::Mat intrinsic, distCoeffs;
    // calibrate_camera(intrinsic, distCoeffs);
    // cv::Mat undistorted_img1, img1 = cv::imread("../../images/chessboard/left01.jpg", cv::IMREAD_GRAYSCALE);
    // undistort_image(img1, undistorted_img1, intrinsic, distCoeffs);

    // cv::imshow("dsa", undistorted_img1);
    // cv::waitKey();

    if (args.mode == Mode::IMAGES) {
        // Abrir imágenes
        std::vector<cv::Mat> orig_images;
        for (std::string file : args.images_files) {
            cv::Mat img = cv::imread(file, cv::IMREAD_COLOR);
            if (img.empty()) {
                std::cout << "No se puede abrir la imagen: " << file << std::endl;
            } else {
                orig_images.push_back(img);
            }
        }

        if (orig_images.size() < 2) {
            std::cout << "No hay suficientes imágenes" << std::endl;
            return 1;
        }

        // Realizar panorama
        cv::Mat panorama = orig_images[0];
        for (int i = 1; i < orig_images.size(); i++) {
            doPanorama(orig_images[i], panorama, panorama, args.features, args.nn_ratio, args.use_flann, args.blend, args.seam);
            cv::imshow("Panorama", panorama);

            cv::resizeWindow("Panorama", cv::Size(screen_width, screen_height / 2));
            cv::moveWindow("Panorama", 0, 0);

            cv::resizeWindow("Matches", cv::Size(screen_width, screen_height / 2));
            cv::moveWindow("Matches", 0, screen_height / 2 + 40);

            int k = cv::waitKey(0);

            if (k == 27) {
                break;
            } else if (k == 's') {
                std::string name = imwriteSafe(output_name, panorama);
                std::cout << "Guardada imagen: " << name << std::endl;
                cv::waitKey(0);
            }
        }
    } else {
        // Capturar desde la cámara
        cv::VideoCapture cap;
        cap.open(0);
        if (!cap.isOpened()) {
            std::cout << "Failed to connect to camera." << std::endl;
            return 1;
        }
        std::cout << "Se tomarán imágenes en vivo." << std::endl;

        cv::Mat frame, panorama, new_panorama;
        cap >> frame;

        frame = frame(Rect(300, 0, frame.cols - 600, frame.rows - 90));

        // cv::resize(frame, frame, cv::Size(), 0.5, 0.5);
        std::cout << frame.rows << " " << frame.cols << std::endl;
        panorama = frame;

        cv::imshow("Panorama", panorama);

        while (true) {
            cv::resizeWindow("Panorama", cv::Size(screen_width, screen_height / 2));
            cv::moveWindow("Panorama", 0, 0);

            cv::resizeWindow("Matches", cv::Size(screen_width, screen_height / 2));
            cv::moveWindow("Matches", 0, screen_height / 2 + 40);

            int k;
            if (args.mode == Mode::LIVE_AUTO) {
                k = cv::waitKey(args.capture_interval);
            } else {
                k = cv::waitKey(0);
            }

            if (k == 27) {
                break;
            } else if (k == 's') {
                std::string name = imwriteSafe(output_name, panorama);
                std::cout << "Guardada imagen: " << name << std::endl;
                cv::waitKey(0);
            }

            // Capturar nueva imagen
            cap >> frame;
            if (frame.empty()) // end of video stream
                break;
            // cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

            frame = frame(Rect(300, 0, frame.cols - 600, frame.rows - 90));

            // Crear panorama
            doPanorama(frame, panorama, new_panorama, args.features, args.nn_ratio, args.use_flann, args.blend, args.seam);
            panorama = new_panorama.clone();

            cv::imshow("Panorama", panorama);
        }

        cap.release();
    }
}
