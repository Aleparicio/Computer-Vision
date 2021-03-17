#include "vanishing_points.hpp"

// Obtiene el máximo de una matriz
float max(cv::Mat dst){
    float m = dst.at<float>(0,0);
    for(int i = 0; i < dst.rows; i++){
        for(int j = 0; j < dst.cols; j++){
                if(dst.at<float>(i,j) > m)
                    m = dst.at<float>(i,j);
        }
    }
    return m;
}

// Obtiene el mínimo de una matriz
float min(cv::Mat dst){
    float m = dst.at<float>(0,0);
    for(int i = 0; i < dst.rows; i++){
        for(int j = 0; j < dst.cols; j++){
                if(dst.at<float>(i,j) < m)
                    m = dst.at<float>(i,j);
        }
    }
    return m;
}

void filterLinesAndVote(std::vector<cv::Vec2f>& lines, cv::Mat& img, 
                        std::vector<cv::Vec4f>& flines, cv::Vec2i& pt){

    flines.clear();
    cv::Vec2f pt1, pt2;
    const float angleEpsRad = 10 *2*CV_PI/360;
    std::vector<float> angulos;

    // Filtrado de líneas
    for(int i = 0; i < lines.size(); ++i){
        // Las líneas verticales se descartan
        if(abs(lines[i][1]) > angleEpsRad && abs(lines[i][1] - CV_PI / 2) > angleEpsRad){
            transformLine(lines[i], pt1, pt2);
            cv::Vec4f aux(pt1[0], pt1[1], pt2[0], pt2[1]);
            flines.push_back(aux);
            angulos.push_back(lines[i][1]);
        }
    }
    votingProcess(flines, angulos, pt);
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
    cv::drawMarker(cdst, punto_fuga, cv::Scalar(0,255,0), 0, 50);
}

// Obtiene los puntos de fuga
cv::Mat getVanishingPoints(cv::Mat frame){

    cv::Mat frame_gray, grad_x, grad_y, dst;
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    CannyGradient(frame_gray, grad_x, grad_y, 1.4, 9);
    
    cv::Mat abs_grad_x, abs_grad_y;
    //Para mostrar
    //grad_x.convertTo(abs_grad_x, CV_8U, 0.5, 128);
    //grad_y.convertTo(abs_grad_y, CV_8U, 0.5, 128);
    cv::Canny(grad_x, grad_y, dst, 30, 110, true);

    cv::Mat cdst;
    cv::Vec2i punto_fuga(0,0);
    cv::cvtColor(dst, cdst, cv::COLOR_GRAY2BGR);

    // Standard Hough Line Transform
    std::vector<cv::Vec2f> lines;                    
    std::vector<cv::Vec4f> flines; 
    cv::HoughLines(dst, lines, 1, CV_PI / 180, 50, 0, 0, 0.0, CV_PI / 2.0); // Run the detection
    filterLinesAndVote(lines, dst, flines, punto_fuga);
    draw2Dlines(cdst, flines, punto_fuga);
    return cdst;
}