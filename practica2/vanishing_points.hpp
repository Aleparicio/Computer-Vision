#ifndef vanish
#define vanish

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "canny_operator.hpp"
#include <iostream>
#include <list>

// Filtra las líneas obtenidas por la trasnformada de Hough,
// las restantes votan para encontrar el punto de fuga, un voto por línea
// Las líneas eliminadas son las verticales y aquellas cuya intersección
// con el horizonte está fuera de la imagen.
void filterLinesAndVote(std::vector<cv::Vec2f>& lines, cv::Mat& img, 
                         std::vector<cv::Vec4f>& flines, cv::Vec2i& pt);

// Transforma la recta line de una representación de origen + ángulo
// (theta + ro) a una representación de dos puntos
void transformLine(cv::Vec2f line, cv::Vec2f& p1, cv::Vec2f& p2);

// Finds the intersection of two lines, or returns false.
// Input : A = (1, 1), B = (4, 4)
//         C = (1, 8), D = (2, 4)
//         Output : The intersection of the given lines 
//         AB and CD is: (2.4, 2.4)
// Líneas representada por ax + by = c
// Encontrar X -- > (a1b2 – a2b1) x = c1b2 – c2b1.
// https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
// Ssi devuelve cierto, res contiene el puno de intersección
bool intersection(cv::Vec2f a, cv::Vec2f b, cv::Vec2f c, cv::Vec2f d, cv::Vec2f& res);

// Dibuja las líneas sobre la imagen, almacenadas como x1 y1 x2 y2.
// Además, dibuja el punto de fuga
void draw2Dlines(cv::Mat& cdst, std::vector<cv::Vec4f>& lines, cv::Point2i punto_fuga);

// Obtiene los puntos de fuga de la iamge
cv::Mat getVanishingPoints(cv::Mat frame);

#endif