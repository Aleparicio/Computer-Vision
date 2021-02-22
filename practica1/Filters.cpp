#include "Filters.h"

void contrastAdjustment(cv::Mat& image, float gain, float sesgo){
    image = gain*image;
    image = image+sesgo;
}

//void equalizeImage(cv::Mat& image){

//}

