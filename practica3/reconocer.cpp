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

    cv::imshow("Imagen", frame);

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
        std::cout << "Objeto " << i + 1 << std::endl;

        // Calcular descriptores del objeto
        Descriptors x = descriptors(components[i]);

        cv::imshow("Componentes " + std::to_string(i + 1), components[i]);
        // drawConnectedComponents(components[i], connected);
        // cv::imshow("Componentes conexas " + std::to_string(i), connected);
        drawContours(components[i], contours);
        cv::imshow("Contornos " + std::to_string(i + 1), contours);

        // Predecir la clase
        auto possible_classes = mc.predict(x);

        std::cout << "Predicción " << i + 1 << ": ";
        switch (possible_classes.size()) {
        case 0:
            std::cout << "Objeto desconocido" << std::endl;
            break;
        case 1:
            std::cout << possible_classes.begin()->second << std::endl;
            break;
        default:
            std::cout << "Dudas entre las clases ";
            for (auto& c : possible_classes) {
                std::cout << " " << c.second;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    cv::waitKey();
}
