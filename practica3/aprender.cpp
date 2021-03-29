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
    std::string sample;
    Descriptors descs;

    std::ifstream is(file);

    if (is) {
        std::string line;
        while (std::getline(is, line)) {
            std::istringstream ss(line);
            // Read label
            ss >> sample;
            Y.push_back(sample);
            // Read file
            ss >> sample;
            files.push_back(sample);
            // Read descriptors
            ss >> descs;
            X.push_back(descs);
        }
    } else {
        std::cerr << "No existe el fichero " << file << std::endl;
    }
    is.close();
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
    cv::Mat frame;
    std::string image_path;
    try {
        image_path = cv::samples::findFile(argv[1]);
        frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    } catch (const std::exception& e) {
        std::cout << "No se ha podido abrir la imagen " << argv[1] << std::endl;
        return 1;
    }

    if (frame.empty()) {
        std::cout << "La imagen está vacía" << argv[1] << std::endl;
        return 1;
    }

    std::cout << "Aprendiendo imagen: " << image_path << std::endl;

    // Calcular los descriptores de la muestra
    thresholding(frame, frame, OTSU);
    Descriptors d = descriptors(frame);

    // Añadir la muestra al conjunto de datos
    X.push_back(d);
    Y.push_back(argv[2]);
    files.push_back(argv[1]);

    // Entrenar modelo
    MahalanobisClassifier mc;
    mc.train(X, Y, A_PRIORI_ESTIMATION, 10);

    write_samples(samples_file, X, Y, files);
    mc.save_model(model_file);

    return 0;
}
