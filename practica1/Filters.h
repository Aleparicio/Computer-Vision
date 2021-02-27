#pragma once

#include <opencv2/opencv.hpp>

#include "cvui.h"
#include "EnhancedWindow.h"

// enum filtro { CONTRASTE,
//               ALIEN,
//               POSTER,
//               DISTORSION,
//               EQHISTO,
//               CLAHE };

class Filter {
  public:
    // inline const static std::string name = "Filter";
    virtual void apply(cv::Mat& img) const = 0;
    virtual void showSettings(EnhancedWindow& settings, cv::Mat& frame) = 0;
};

class Contrast : public Filter {
  public:
    inline const static std::string name = "Contrast";
    Contrast(int _gain, int _bias) : gain(_gain), bias(_bias) {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;

  private:
    const static int MAX_GAIN = 100;
    const static int MAX_BIAS = 100;
    float gain;
    float bias;
};

class HistogramEqualization : public Filter {
  public:
    inline const static std::string name = "Histogram Equalization";
    HistogramEqualization() {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;
};

// Adaptive histogram equalization
class CLAHE : public Filter {
  public:
    inline const static std::string name = "CLAHE Equalization";
    CLAHE(int _clipLimit) : clipLimit(_clipLimit) {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;

  private:
    const static int MAX_CLIP_LIMIT = 100;
    float clipLimit;
};

class Posterization : public Filter {
  public:
    inline const static std::string name = "Posterization";
    Posterization(int _numColors) : numColors(_numColors) {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;

  private:
    const static int MAX_NUM_COLORS = 100;
    int numColors;
    int reduce(int value, int numColors) const;
};

class Alien : public Filter {
  public:
    inline const static std::string name = "Alien";
    Alien(int _R, int _G, int _B, int _alpha) : R(_R), G(_G), B(_B), alpha(_alpha) {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;

  private:
    int R, G, B, alpha;
};

// Barrel and Pincushion distortion correction
class Distortion : public Filter {
  public:
    inline const static std::string name = "Distortion Correction";
    Distortion(int _k1, int _k2) : k1(_k1), k2(_k2) {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;

  private:
    float k1, k2;
};

// Thresholding distorsion correction
class Thresholding : public Filter {
  public:
    inline const static std::string name = "Thresholding";
    Thresholding(int _thresh) : thresh(_thresh), inverse(false) {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;

  private:
    float thresh;
    bool inverse;
};

// Twirl (whirlpool) effect
class Twirl : public Filter {
  public:
    inline const static std::string name = "Twirl";
    Twirl(int _x, int _y, int _radius, int _twist)
        : x(_x), y(_y), radius(_radius), twist(_twist) {}
    void apply(cv::Mat& img) const override;
    void showSettings(EnhancedWindow& settings, cv::Mat& frame) override;

  private:
    float x, y, radius, twist;
};
