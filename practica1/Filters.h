#pragma once

#include <opencv2/opencv.hpp>

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
};

class Contrast : public Filter {
  public:
    inline const static std::string name = "Contrast";
    Contrast(std::string winname);
    Contrast(int _gain, int _bias) : gain(_gain), bias(_bias) {}
    void apply(cv::Mat& img) const override;

  private:
    const static int MAX_GAIN = 100;
    const static int MAX_BIAS = 100;
    int gain;
    int bias;
};

class HistogramEqualization : public Filter {
  public:
    inline const static std::string name = "Histogram Equalization";
    HistogramEqualization(std::string winname);
    void apply(cv::Mat& img) const;
};

// Adaptive histogram equalization
class CLAHE : public Filter {
  public:
    inline const static std::string name = "CLAHE Equalization";
    CLAHE(std::string winname);
    CLAHE(int _clipLimit) : clipLimit(_clipLimit) {}
    void apply(cv::Mat& img) const;

  private:
    const static int MAX_CLIP_LIMIT = 100;
    int clipLimit;
};

class Posterization : public Filter {
  public:
    inline const static std::string name = "Posterization";
    Posterization(std::string winnanme);
    Posterization(int _numColors) : numColors(_numColors) {}
    void apply(cv::Mat& img) const;

  private:
    const static int MAX_NUM_COLORS = 100;
    int numColors;
    int reduce(int value, int numColors) const;
};

class Alien : public Filter {
  public:
    inline const static std::string name = "Alien";
    Alien(std::string winname);
    Alien(int _R, int _G, int _B, int _alpha) : R(_R), G(_G), B(_B), alpha(_alpha) {}
    void apply(cv::Mat& img) const;

  private:
    int R, G, B, alpha;
};

// Barrel and Pincushion distorsion correction
class Distortion : public Filter {
  public:
    inline const static std::string name = "Distortion Correction";
    Distortion(std::string winname);
    Distortion(int _k1, int _k2) : k1(_k1), k2(_k2) {}
    void apply(cv::Mat& img) const;

  private:
    int k1, k2;
};
