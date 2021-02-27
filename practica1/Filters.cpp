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
        cvui::trackbar(settings.width() - 20, &gain, 0.1f, 20.0f);
        cvui::space(20);
        cvui::text("Bias");
        cvui::trackbar(settings.width() - 20, &bias, 0.0f, 255.0f);
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

void Posterization::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(100);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("Number of colors per channel");
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

// Scale value between min and max
float scale(float value, float min, float max) {
    // if value [0,1]
    // return (max - min) * value + min;
    return (value - min) / (max - min);
}

// Distortion
void Distortion::apply(cv::Mat& img) const {

    cv::Mat ximg = cv::Mat::zeros(img.rows, img.cols, CV_32FC1);
    cv::Mat yimg = cv::Mat::zeros(img.rows, img.cols, CV_32FC1);

    float cx = img.cols / 2;
    float cy = img.rows / 2;
    int h = img.rows;
    int w = img.cols;

    float gk1 = (float)k1 / 1000000.f;
    float gk2 = (float)k2 / 1000000000.f;

    float* bufferx = (float*)(&ximg)->data;
    float* buffery = (float*)(&yimg)->data;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float r2 = (x - cx) * (x - cx) + (y - cy) * (y - cy);
            *bufferx = (x - cx) * (1 + gk1 * r2 + gk2 * r2 * r2);
            *buffery = (y - cy) * (1 + gk1 * r2 + gk2 * r2 * r2);
            ++buffery;
            ++bufferx;
        }
    }

    ximg = ximg + (cx);
    yimg = yimg + (cy);

    // cv::Mat aux = img.clone();
    // cv::remap(aux, img, ximg, yimg, cv::INTER_LINEAR);
    cv::remap(img, img, ximg, yimg, cv::INTER_LINEAR);
}

void Distortion::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(180);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("k1 will be divided by 1000000");
        cvui::text("k2 will be divided by 1000000000");
        cvui::text("k1");
        cvui::trackbar(settings.width() - 20, &k1, -3.f, 3.f);
        cvui::space(20);
        cvui::text("k2");
        cvui::trackbar(settings.width() - 20, &k2, -1.f, 1.f);
    }
    settings.end();
}

// Thresholding
void Thresholding::apply(cv::Mat& img) const {
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    if (inverse)
        cv::threshold(img, img, thresh, 255, cv::THRESH_BINARY_INV);
    else
        cv::threshold(img, img, thresh, 255, cv::THRESH_BINARY);
}

void Thresholding::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    settings.setHeight(180);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("Threshold value");
        cvui::trackbar(settings.width() - 20, &thresh, 0.f, 255.f);
        cvui::space(20);
        cvui::checkbox("Inverse", &inverse);
    }
    settings.end();
}

// Twirl
void Twirl::apply(cv::Mat& img) const {
    cv::Mat source = img.clone();

    const float width = (float)img.rows;
    const float height = (float)img.cols;

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {

            // Perform all the calculations repect to the twirl center
            float xp = (float)j - x;
            float yp = (float)i - y;

            // Angle between the pixel and the X axis
            float angle = atan2f(yp, xp);
            // Distance between the pixel and the center
            float distance = sqrtf((xp * xp) + (yp * yp));

            float amount = 1.0f - (distance / radius);
            if (amount > 0.0f) {
                angle += (twist / 10.0f) * amount * M_PI * 2.0f;

                float xr = sinf(angle) * distance + y;
                float yr = cosf(angle) * distance + x;

                int k = (int)std::min(width - 1, std::max(0.0f, xr));
                int m = (int)std::min(height - 1, std::max(0.0f, yr));

                uchar* src = source.ptr<uchar>(k, m);
                uchar* out = img.ptr<uchar>(i, j);

                out[0] = src[0];
                out[1] = src[1];
                out[2] = src[2];
            }
        }
    }
}

void Twirl::showSettings(EnhancedWindow& settings, cv::Mat& frame) {
    // Mouse interaction
    if (cvui::mouse(cvui::RIGHT_BUTTON, cvui::DOWN)) {
        x = cvui::mouse().x;
        y = cvui::mouse().y;
    }

    settings.setHeight(200);
    settings.begin(frame);
    if (!settings.isMinimized()) {
        cvui::text("Threshold value");
        cvui::trackbar(settings.width() - 20, &radius, 0.f, 1000.f);
        cvui::space(20);
        cvui::text("Threshold value");
        cvui::trackbar(settings.width() - 20, &twist, 0.f, 25.f);
        cvui::space(20);
        cvui::text("Right mouse click to set the center!");
    }
    settings.end();
}
