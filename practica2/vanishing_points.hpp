#ifndef vanish
#define vanish

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "ransac.hpp"
#include "canny_operator.hpp"
#include <iostream>
#include <list>

// Filtra las líneas obtenidas por la trasnformada de Hough,
// las restantes votan para encontrar el punto de fuga
// Las líneas eliminadas son las verticales y horizontales con un
// ángulo a modo de epsilon
void filterLinesAndVote(std::vector<cv::Vec2f>& lines, cv::Mat& img, 
                         std::vector<cv::Vec4f>& flines, cv::Vec2i& pt);

// Transforma la recta line de una representación de origen + ángulo
// (theta + ro) a una representación de dos puntos
void transformLine(cv::Vec2f line, cv::Vec2f& p1, cv::Vec2f& p2);

// Dibuja las líneas sobre la imagen, almacenadas como x1 y1 x2 y2.
// Además, dibuja el punto de fuga
void draw2Dlines(cv::Mat& cdst, std::vector<cv::Vec4f>& lines, cv::Point2i punto_fuga);

// Obtiene los puntos de fuga de la iamge
cv::Mat getVanishingPoints(cv::Mat frame);

#endif