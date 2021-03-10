#include "vanishing_points.hpp"


// Votación y obtención del punto de fuga
void votingProcess(cv::Mat img, std::vector<cv::Vec2f>& valid_intersections, cv::Vec2i& pt){
    
    // Votación
    std::vector<int> pixels(img.cols, 0);
    for(int i = 0; i < valid_intersections.size(); ++i)
        pixels[cvRound(valid_intersections[i][0])] += 1;

    // Obtención del punto de fuga
    int nmax = 0;
    pt[1] = img.rows / 2.0;
    for(int i = 0; i < img.cols; ++i){
        if(pixels[i] > nmax){
            nmax = pixels[i];
            pt[0] = i;
        }
    }
}


// Filtra las líneas obtenidas por la trasnformada de Hough,
// las restantes votan para encontrar el punto de fuga, un voto por línea
// Las líneas eliminadas son las verticales y aquellas cuya intersección
// con el horizonte está fuera de la imagen.
void filterLinesAndVote(std::vector<cv::Vec2f>& lines, cv::Mat& img, 
                        std::vector<cv::Vec4f>& flines, cv::Vec2i& pt){

    flines.clear();
    cv::Vec2f pt1, pt2, res;
    const int angleVertical = 10;
    std::vector<cv::Vec2f> valid_intersections;

    // Obtención de la recta que pasa por el centro de la imagen
    cv::Vec2f img1(0.0, img.rows / 2.0);
    cv::Vec2f img2(img.cols, img.rows / 2.0);

    // Filtrado de líneas
    for(int i = 0; i < lines.size(); ++i){
        // Las líneas verticales se descartan
        if(abs(lines[i][1]) > angleVertical * 2*CV_PI/360){
            transformLine(lines[i], pt1, pt2);
            // Las líneas que no intersectan, se descartan
            if(intersection(pt1, pt2, img1, img2, res)) {
                // Si la intersección cae fuera de la imagen se elimina
                if(res[0] >= 0. && res[0] < img.cols && res[1] >= 0. && res[1] < img.rows){
                    cv::Vec4f aux(pt1[0], pt1[1], pt2[0], pt2[1]);
                    flines.push_back(aux);
                    valid_intersections.push_back(res);
                }
            }
        }
    }
    votingProcess(img, valid_intersections, pt);
}


// Transforma la recta line de una representación de origen + ángulo
// (theta + ro) a una representación de dos puntos
void transformLine(cv::Vec2f line, cv::Vec2f& p1, cv::Vec2f& p2){
        float rho = line[0], theta = line[1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        p1[0] = cvRound(x0 + 1000 * (-b));
        p1[1] = cvRound(y0 + 1000 * (a));
        p2[0] = cvRound(x0 - 1000 * (-b));
        p2[1] = cvRound(y0 - 1000 * (a));
}


// Finds the intersection of two lines, or returns false.
// Input : A = (1, 1), B = (4, 4)
//         C = (1, 8), D = (2, 4)
//         Output : The intersection of the given lines 
//         AB and CD is: (2.4, 2.4)
// Líneas representada por ax + by = c
// Encontrar X -- > (a1b2 – a2b1) x = c1b2 – c2b1.
// https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
// Ssi devuelve cierto, res contiene el puno de intersección
bool intersection(cv::Vec2f a, cv::Vec2f b, cv::Vec2f c, cv::Vec2f d, cv::Vec2f& res) {

    // Línea ab
    float a1 = b[1] - a[1];
    float b1 = a[0] - b[0];
    float c1 = a1*a[0] + b1*a[1];

    // Línea cd
    float a2 = d[1] - c[1];
    float b2 = c[0] - d[0];
    float c2 = a2*c[0] + b2*c[1];

    float det = a1*b2 - a2*b1;
    if(abs(det) > 10e-6){
        res[0] = (b2*c1 - b1*c2) / det; 
        res[1] = (a1*c2 - a2*c1) / det; 
        return true;
    } else {
        return false;
    }
}

// Dibuja las líneas sobre la imagen, almacenadas como x1 y1 x2 y2.
// Además, dibuja el punto de fuga
void draw2Dlines(cv::Mat& cdst, std::vector<cv::Vec4f>& lines, cv::Point2i punto_fuga){
    for (size_t i = 0; i < lines.size(); i++) {
        cv::Point pt1, pt2;
        pt1.x = lines[i][0];
        pt1.y = lines[i][1];
        pt2.x = lines[i][2];
        pt2.y = lines[i][3];
        cv::line(cdst, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }
    cv::drawMarker(cdst, punto_fuga, cv::Scalar(0,255,0));
}