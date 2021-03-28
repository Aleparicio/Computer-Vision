#include <iostream>
#include <fstream>

#include "mahalanobis_classifier.hpp"
#include "descriptors.hpp"

// Leer modelo de un fichero
//
// Formato del modelo:
// clase N media_descriptores media_varianza
// ...
void MahalanobisClassifier::load_model(const std::string& file) {
    std::string name;
    Descriptors mean;
    Descriptors variance;

    std::ifstream is(file);

    if (is) {
        std::string line;
        while (std::getline(is, line)) {
            std::istringstream ss(line);
            // Leer clase
            ss >> name;
            // Leer medias
            ss >> mean;
            // Leer varianzas
            ss >> variance;
            stats[name] = ClassStats(mean, variance);
        }
    } else {
        std::cerr << "No existe el fichero " << file << std::endl;
    }
    is.close();
}

// Guardar modelo en un fichero
//
// Formato del modelo:
// clase N media_descriptores media_varianza
// ...
void MahalanobisClassifier::save_model(const std::string& file) {
    // Creación de los datos del nuevo fichero
    std::ofstream os(file);
    for (const auto& entry : stats) {
        os << entry.first << "  " << entry.second.N << "  " << entry.second.mean << "  " << entry.second.variance << std::endl;
    }
}

// Entrenar el modelo creando las estadísticas de los descriptores
void MahalanobisClassifier::train(const std::vector<Descriptors>& X, const std::vector<std::string>& Y) {
    // Obtener los nombres de todas las clases
    std::vector<std::string> classes = Y; // Crear copia del vector
    std::sort(classes.begin(), classes.end());
    // Eliminar elementos duplicados
    std::vector<std::string>::iterator it;
    it = std::unique(classes.begin(), classes.end());
    classes.resize(std::distance(classes.begin(), it));

    // Ordenar las muestras de entrenamiento por clases
    std::unordered_map<std::string, std::vector<Descriptors>> samples_dict;
    for (int i = 0; i < X.size(); i++) {
        samples_dict[Y[i]].push_back(X[i]);
    }

    // Número de muestras por clase
    for (auto& c : classes) {
        stats[c].N = samples_dict[c].size();
    }

    // Calcular medias
    for (auto& c : classes) {
        // Sumar los descriptores de todas las muestras
        Descriptors sum;
        for (int i = 0; i < stats[c].N; i++) {
            // Sumar valores de los descriptores
            for (int k = 0; k < sum.data.size(); k++) {
                sum.data[k] += samples_dict[c][i].data[k];
            }
        }
        // Dividir todos los descriptores por el número de muestras
        for (int k = 0; k < sum.data.size(); k++) {
            stats[c].mean.data[k] = sum.data[k] / stats[c].N;
        }
    }

    // Calcular varianzas
    for (auto& c : classes) {
        // Sumar las distancias al cuadrado de las muestras con la media
        Descriptors squares_sum;
        for (int i = 0; i < stats[c].N; i++) {
            for (int k = 0; k < squares_sum.data.size(); k++) {
                squares_sum.data[k] += pow(samples_dict[c][i].data[k] - stats[c].mean.data[k], 2);
            }
        }
        // Dividir todos los descriptores por el número de muestras - 1
        if (stats[c].N != 1) {
            for (int k = 0; k < squares_sum.data.size(); k++) {
                stats[c].variance.data[k] = squares_sum.data[k] / (stats[c].N - 1);
            }
        }
    }
}

std::vector<std::string> MahalanobisClassifier::predict(const Descriptors& x) {
    // Devuelve un vector con las posibles clases

    // Si el objeto pasa el test de Mahalanobis con una sola clase, el sistema declara que se trata de esa clase

    // Si pasa el test con más de un objeto, declarará que tiene dudas entre las clases

    // Si no pasa el test con ninguna clase, declarará que el objeto es desconocido
}
