#include "ransac.hpp"


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
    if(abs(det) > 10e-4){
        res[0] = (b2*c1 - b1*c2) / det; 
        res[1] = (a1*c2 - a2*c1) / det; 
        return true;
    } else {
        return false;
    }

}


// Devuelve nº inliers del modelo model con un epsilon de error.
// Es decir, devuelve cuántas rectas  de lines intersectan con la 
// circunferencia de centro model y radio epsilon
int checkInliers(std::vector<cv::Vec4f> lines, cv::Vec2f model, float epsilon){

    int inliers = 0;
    for(int i = 0; i < lines.size(); ++i){
        
        if(abs(lines[i][0] - lines[i][2]) > 10e-5){
            // m = (y2-y1) / (x2-x1)
            float m = (lines[i][1] - lines[i][3]) / (lines[i][0] - lines[i][2]);
            // y - mx = b
            float b = lines[i][1] - m*lines[i][0];
            // y = mx + b
            // A = m, B = -1, C = b
            float dist = abs(m*model[0] + (-1)*model[1] + b) / sqrtf(m*m + 1);
        
            if(dist <= epsilon)
                inliers++;
        }
    }
    return inliers;
}


// Votación y obtención del punto de fuga mediante un algoritmo de RANSAC
bool votingProcess(std::vector<cv::Vec4f>& lines, 
                   std::vector<float> angulos, cv::Vec2i& pt){
    
    srand(time(NULL));
    
    // El número de intentos está fijo, podría dejarse en función 
    // de valores como el porcentaje de inliers y/o la seguridad
    // deseada en obtener el mejor modelo.
    int numIntentos = 40;
    float epsilonPxs = 20;
    float epsilonGrados = 12;

    cv::Vec2f model, best_model;
    int best_score = -1;

    for(int i = 0; i < numIntentos && lines.size() > 1; ++i){

        // Elige las líneas
        int l1 = rand() % lines.size();
        int l2 = rand() % lines.size();

        cv::Vec2f l1_1(lines[l1][0], lines[l1][1]);
        cv::Vec2f l1_2(lines[l1][2], lines[l1][3]);
        cv::Vec2f l2_1(lines[l2][0], lines[l2][1]);
        cv::Vec2f l2_2(lines[l2][2], lines[l2][3]);

        // Calcula la intersección y si el ángulo les permite ser candidatas
        float angleDiff = abs(angulos[l1] - angulos[l2]) / (2*CV_PI) * 360;
        if(intersection(l1_1, l1_2, l2_1, l2_2, model) && angleDiff > epsilonGrados){

            int score = checkInliers(lines, model, epsilonPxs);
            if(score > best_score){
                best_score = score;
                best_model = model;
            }
        }
    }

    if(best_score < 1)
        return false;
    else{
        pt = best_model;
        return true;
    }
}
