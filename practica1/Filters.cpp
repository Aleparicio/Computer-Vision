#include "Filters.h"

// Contrast

void Contrast::apply(cv::Mat& img) const {
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    img = gain * img;
    img = img + bias;
}

void Contrast::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(180);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("Gain");
        cvui::trackbar(settings.width() - 20, &gain, 5.0f, 150.0f);
        cvui::space(20);
        cvui::text("Bias");
        cvui::trackbar(settings.width() - 20, &bias, 80.0f, 300.0f);
    }
    settings.end();
}

// Histogram Equalization

void HistogramEqualization::apply(cv::Mat& image) const {
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(image, image);
}

void HistogramEqualization::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(100);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("No settings!");
    }
    settings.end();
}

// CLAHE

void CLAHE::apply(cv::Mat& image) const {
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clipLimit);
    clahe->apply(image, image);
}

void CLAHE::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(100);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("Clip Limit");
        cvui::trackbar(settings.width() - 20, &clipLimit, 0.0f, 25.0f);
    }
    settings.end();
}

// Posterize

int Posterization::reduce(int value, int numColors) const {
    // Size of each interval
    int size = 255 / numColors;
    return ((value / size)) * size;
}

void Posterization::apply(cv::Mat& image) const {
    for (int i = 0; i < image.rows; i++) {
        uchar* data = image.ptr<uchar>(i);
        for (int j = 0; j < image.cols * image.channels(); j++) {
            data[j] = reduce(data[j], numColors);
            // *data ++= *data/div*div + div/2;
        }
    }
}

// void Posterization::apply(cv::Mat& image) const {
//     for (int i = 0; i < image.rows; i++) {
//         uchar* data = image.ptr<uchar>(i);
//         for (int j = 0; j < image.cols * image.channels(); j++) {
//             // data[j] = data[j]/div*div + div/2
//             *data++ = *data / numColors * numColors + numColors / 2;
//         }
//     }
// }

void Posterization::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(100);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("Number of colors");
        cvui::trackbar(settings.width() - 20, &numColors, 1, 25, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE);
    }
    settings.end();
}

// Alien

void Alien::apply(cv::Mat& img) const {
}

void Alien::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(350);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("R");
        cvui::trackbar(settings.width() - 20, &R, 0, 255, 1, "%.0Lf");
        cvui::space(20);
        cvui::text("G");
        cvui::trackbar(settings.width() - 20, &G, 0, 255, 1, "%.0Lf");
        cvui::space(20);
        cvui::text("B");
        cvui::trackbar(settings.width() - 20, &B, 0, 255, 1, "%.0Lf");
        cvui::space(20);
        cvui::text("Alpha");
        cvui::trackbar(settings.width() - 20, &alpha, 0, 255, 1, "%.0Lf");
    }
    settings.end();
}

// Distortion

void Distortion::apply(cv::Mat& img) const {
}

void Distortion::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(180);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("k1");
        cvui::trackbar(settings.width() - 20, &k1, 5.0f, 150.0f);
        cvui::space(20);
        cvui::text("k2");
        cvui::trackbar(settings.width() - 20, &k2, 80.0f, 300.0f);
    }
    settings.end();
}
