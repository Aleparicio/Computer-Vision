#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

const int ALPHA_SLIDER_MAX = 100;

// int alpha;
// int alphaSlider = 0;

// static void onTrackbar(int, void*) {
//     // alpha = (double)alphaSlider / ALPHA_SLIDER_MAX;
//     alpha = 1 + alphaSlider;
//     std::cout << "alpha: " << alpha << std::endl;
//     std::cout << "alphaSlider: " << alphaSlider << std::endl;
// }


int main() {
    cv::VideoCapture cap;

    if (!cap.open(0)) {
        std::cout << "Failed to connect to cam" << std::endl;
        return 0;
    }

    cv::namedWindow("Gaussian Blur", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Gaussian Blur", 1000, 0);

    // Versión con callback
    // char trackbarName[50];
    // sprintf(trackbarName, "Alpha x %d", ALPHA_SLIDER_MAX);
    // cv::createTrackbar(trackbarName, "Gaussian Blur", &alphaSlider, ALPHA_SLIDER_MAX, onTrackbar);
    // // Los valores están entre 0 y ALPHA_SLIDER_MAX (el mínimo siempre es 0)
    // // El valor numérico de la trackbar está en alphaSlider
    // // onTrackbar es la función de callback

    // onTrackbar(alphaSlider, 0);


    // También hay cv::String

    int val = 0;
    cv::createTrackbar("Blur", "Gaussian Blur", &val, ALPHA_SLIDER_MAX, NULL);

    while (true) {
        cv::Mat frame;

        cap >> frame;
        if (frame.empty()) // end of video stream
            break;

        // Procesar el frame
        if (val > 0) {
            cv::GaussianBlur(frame, frame, cv::Size(0, 0), ((double)val)/10); // aplicar filtro gaussiano
        }

        std::string label = "amount: " + std::to_string(val) + "%";
        cv::putText(frame, label, cv::Point(20, 30), cv::FONT_HERSHEY_PLAIN, 2.0, CV_RGB(0,255,0), 2.0);

        cv::imshow("Gaussian Blur", frame);
        // if(cv::waitKey(10) == 27) // stop capturing when key is pressed
        // if(cv::waitKey(10) != -1) // stop capturing when key is pressed
        if(cv::waitKey(10) >= 0) // stop capturing when key is pressed
            break;
    }

    // cv::destroyAllWindows();

    // the camera will be closed automatically upon exit
    // wtr.release();
    // cap.close();
    return 0;
}
