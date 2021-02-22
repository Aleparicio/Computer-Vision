#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <iomanip>

const int TRACKBAR_MAX = 100;

// void onTrackbar(int value, void* userdata) {
//     std::cout << value << std::endl;

//     double* blur = (double*)userdata;

//     // double blur = *((double*)userdata);

//     if (value == 0) {
//         *blur = 0;
//     }
//     else {
//         *blur = ((double)value) / 10;
//     }
// }


struct TrackbarData {
    double blur;
    std::string label;
};

void onTrackbar(int value, void* userdata) {
    TrackbarData* td = (TrackbarData*)userdata;

    if (value == 0) {
        td->blur = 0;
    }
    else {
        td->blur = ((double)value) / 10;
    }

    // td->label = "amount: " + std::to_string(value);
    std::stringstream sstm;
    sstm << "amount: " << std::fixed << std::setprecision(1) << td->blur;
    td->label = sstm.str();
}


int main() {
    cv::VideoCapture cap;

    if (!cap.open(0)) {
        std::cout << "Failed to connect to cam" << std::endl;
        return 0;
    }

    cv::namedWindow("Gaussian Blur", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Gaussian Blur", 100, 100);

    int value = 0;
    TrackbarData td;

    td.blur = 0;
    td.label = "Adjust the slider to blur the image";

    cv::createTrackbar("Blur", "Gaussian Blur", &value, TRACKBAR_MAX, onTrackbar, &td);
    // Los valores están entre 0 y ALPHA_SLIDER_MAX (el mínimo siempre es 0)
    // El valor numérico de la trackbar está en alphaSlider
    // onTrackbar es la función de callback

    while (true) {
        cv::Mat frame;

        cap >> frame;
        if (frame.empty()) // end of video stream
            break;

        // Procesar el frame
        if (td.blur > 0) {
            cv::GaussianBlur(frame, frame, cv::Size(0, 0), td.blur); // aplicar filtro gaussiano
        }

        cv::putText(frame, td.label, cv::Point(40, 40), cv::FONT_HERSHEY_PLAIN, 2.0, CV_RGB(0,255,0), 2.0);

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
