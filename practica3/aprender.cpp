#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>

#include <fstream>

#include "descriptors.hpp"
#include "mahalanobis_classifier.hpp"

// Leer muestras de un fichero
//
// Formato de las muestras:
// clase fichero descriptores
// ...
void read_samples(const std::string& file, std::vector<Descriptors>& X,
                  std::vector<std::string>& Y, std::vector<std::string>& files) {
    // std::cout << "leer fichero" << std::endl;
    std::string sample;
    Descriptors descs;

    std::ifstream is(file);

    // std::cout << file << std::endl;

    if (is) {
        std::string line;
        while (std::getline(is, line)) {
            std::istringstream ss(line);
            // std::cout << "Entrado" << std::endl;
            // Read label
            ss >> sample;
            // std::cout << "Label " << sample << std::endl;
            Y.push_back(sample);
            // Read file
            ss >> sample;
            // std::cout << "File " << sample << std::endl;
            files.push_back(sample);
            // Read descriptors
            ss >> descs;
            // std::cout << "Descs " << descs << std::endl;
            X.push_back(descs);
            // std::cout << std::endl;
        }
    } else {
        std::cerr << "No existe el fichero " << file << std::endl;
    }
    is.close();
    // std::cout << "fin leer fichero" << std::endl;
}

// Escribir las muestras en un fichero
//
// Formato de las muestras:
// clase fichero descriptores
// ...
void write_samples(const std::string& file, const std::vector<Descriptors>& X,
                   const std::vector<std::string>& Y, const std::vector<std::string>& files) {
    std::ofstream os(file);
    for (int i = 0; i < X.size(); ++i) {
        os << Y[i] << " " << files[i] << " " << X[i] << std::endl;
    }
}

int main(int argc, char* argv[]) {
    const std::string model_file = "objetos.txt";
    const std::string samples_file = "muestras.txt";

    if (argc != 3) {
        std::cout << "Invocar como: aprender nomfich nomobj" << std::endl;
        return 0;
    }

    // Lectura de las muestras del fichero
    std::vector<Descriptors> X;     // descriptores
    std::vector<std::string> Y;     // etiquetas
    std::vector<std::string> files; // ficheros de cada muestra

    read_samples(samples_file, X, Y, files);

    // Lectura de la nueva muestra de entreamiento
    std::string image_path = cv::samples::findFile(argv[1]);
    cv::Mat frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (frame.empty())
        return 0;

    // Calcular los descriptores de la muestra
    thresholding(frame, frame, OTSU);
    Descriptors d = descriptors(frame);

    // std::cout << "Descriptores" << d << std::endl;

    // Añadir la muestra al conjunto de datos
    X.push_back(d);
    Y.push_back(argv[2]);
    files.push_back(argv[1]);

    // for (int i = 0; i < Y.size(); i++) {
    //     std::cout << Y[i] << std::endl;
    // }

    // Entrenar modelo
    MahalanobisClassifier mc;
    mc.train(X, Y, A_PRIORI_ESTIMATION, 10);

    write_samples(samples_file, X, Y, files);
    mc.save_model(model_file);

    return 0;
}
