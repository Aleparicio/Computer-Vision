// #include <opencv2/core.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>

// #include <time.h>
// #include <iomanip>
// #include <list>
// #include <iostream>
// #include <fstream>

// #include "utils.h"
// #include "writer.h"

// using namespace std;
// using namespace cv;

// int main(int argc, char* argv[]) {

//     string file = "objetos.txt";
//     if (argc != 2) {
//         std::cout << "Invocar como: reconocer nomfich" << std::endl;
//         return 0;
//     }

//     // Lectura del nuevo item
//     std::string image_path = cv::samples::findFile(argv[1]);
//     cv::Mat frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
//     if (frame.empty())
//         return 0;

//     Writer wrt(file);
//     std::vector<Object> medias, varianzas;
//     wrt.getMetrics(medias, varianzas);

//     for (int i = 0; i < medias.size(); ++i) {
//         cout << medias[i].name << "\t" << medias[i].perimeter << "\t" << medias[i].firstHuMoment
//              << "\t" << medias[i].secondHuMoment << "\t" << medias[i].thirdHuMoment << "\n";
//         cout << varianzas[i].name << "\t" << varianzas[i].perimeter << "\t" << varianzas[i].firstHuMoment
//              << "\t" << varianzas[i].secondHuMoment << "\t" << varianzas[i].thirdHuMoment << "\n";
//     }
// }
