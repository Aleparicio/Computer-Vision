#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>

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

// Devuelve nº inliers del modelo model con un epsilon de error.
// Es decir, devuelve cuántas rectas  de lines intersectan con la 
// circunferencia de centro model y radio epsilon
int checkInliers(std::vector<cv::Vec4f> lines, cv::Vec2f model, float epsilon);

// Votación y obtención del punto de fuga mediante un algoritmo de RANSAC
bool votingProcess(std::vector<cv::Vec4f>& lines, std::vector<float> angulos, cv::Vec2i& pt);