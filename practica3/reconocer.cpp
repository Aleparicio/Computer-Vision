#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <vector>

#include "descriptors.hpp"
#include "mahalanobis_classifier.hpp"

int main(int argc, char* argv[]) {
    const std::string model_file = "objetos.txt";

    if (argc != 2) {
        std::cout << "Invocar como: reconocer nomfich" << std::endl;
        return 0;
    }

    // Lectura de la imagen a clasificar
    std::string image_path = cv::samples::findFile(argv[1]);
    cv::Mat frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (frame.empty())
        return 0;

    // Cargar modelo
    MahalanobisClassifier mc;
    mc.load_model(model_file);

    // Obtener objetos de la imagen
    cv::Mat thresholded, connected, contours;
    thresholding(frame, thresholded, OTSU);
    std::vector<cv::Mat> components;
    getConnectedComponents(thresholded, components);

    // Para cada objeto
    for (int i = 0; i < components.size(); i++) {
        // Calcular descriptores del objeto
        Descriptors x = descriptors(frame);

        cv::imshow("Componentes " + std::to_string(i), components[i]);
        // drawConnectedComponents(components[i], connected);
        // cv::imshow("Componentes conexas " + std::to_string(i), connected);
        drawContours(components[i], contours);
        cv::imshow("Contornos " + std::to_string(i), contours);

        // Predecir la clase
        std::vector<std::string> possible_classes = mc.predict(x);

        std::cout << "Predicción " << i << ": ";
        switch (possible_classes.size()) {
        case 0:
            std::cout << "Objeto desconocido" << std::endl;
            break;
        case 1:
            std::cout << "Es: " << possible_classes[0] << std::endl;
            break;
        default:
            std::cout << "Dudas entre las clases:";
            for (auto& c : possible_classes) {
                std::cout << " " << c;
            }
            std::cout << std::endl;
        }
        cv::waitKey();
    }
}
