#include <iostream>
#include <fstream>

#include <map>
#include <set>

#include "mahalanobis_classifier.hpp"
#include "descriptors.hpp"

void MahalanobisClassifier::load_model(const std::string& file) {
    std::string name;
    int N;
    Descriptors mean;
    Descriptors variance;

    std::ifstream is(file);

    if (is) {
        std::string line;
        while (std::getline(is, line)) {
            std::istringstream ss(line);
            // Leer clase
            ss >> name;
            // Leer número de muestras
            ss >> N;
            // Leer medias
            ss >> mean;
            // Leer varianzas
            ss >> variance;
            stats[name] = ClassStats(N, mean, variance);
        }
    } else {
        std::cerr << "No existe el fichero " << file << std::endl;
    }
    is.close();
}

void MahalanobisClassifier::save_model(const std::string& file) {
    // Creación de los datos del nuevo fichero
    std::ofstream os(file);
    for (const auto& entry : stats) {
        os << entry.first << "  " << entry.second.N << "  " << entry.second.mean << "  " << entry.second.variance << std::endl;
    }
}

void MahalanobisClassifier::train(const std::vector<Descriptors>& X, const std::vector<std::string>& Y, VarianceType vtype, float a_priori_percent) {
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
    if (vtype == CLASSIC) {
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
    } else if (vtype == A_PRIORI_ESTIMATION) {
        // estimador a priori: cuadrado de a_priori_percent% de la media del descriptor
        for (auto& c : classes) {
            // Sumar las distancias al cuadrado de las muestras con la media
            Descriptors squares_sum;
            for (int i = 0; i < stats[c].N; i++) {
                for (int k = 0; k < squares_sum.data.size(); k++) {
                    squares_sum.data[k] += pow(samples_dict[c][i].data[k] - stats[c].mean.data[k], 2);
                }
            }
            // Sumar estimador y suma de distancias al cuadrado y dividir por número de muestras
            for (int k = 0; k < squares_sum.data.size(); k++) {
                float variance_estimator = pow((a_priori_percent / 100) * stats[c].mean.data[k], 2);
                stats[c].variance.data[k] = (variance_estimator + squares_sum.data[k]) / stats[c].N;
            }
        }
    }
}

std::set<std::pair<float, std::string>> MahalanobisClassifier::predict(const Descriptors& x) const {
    // Calcular distancia de Mahalanobis del objeto x con cada clase
    std::set<std::pair<float, std::string>> mahalanobis_distances;

    // Los elementos del set se van ordenado por el float de la pareja
    // en orden decreciente automáticamente
    for (auto& entry : stats) {
        float total_distance = 0;
        for (int i = 0; i < x.data.size(); i++) {
            float distance = pow(x.data[i] - entry.second.mean.data[i], 2) / entry.second.variance.data[i];
            total_distance += distance;
        }
        mahalanobis_distances.emplace(total_distance, entry.first);
    }

    std::cout << "Distancias de Mahalanobis:" << std::endl;
    for (auto& entry : mahalanobis_distances) {
        std::cout << "  * " << entry.second << ": " << entry.first << std::endl;
    }

    // Eliminar clases que no pasan el test
    auto it = mahalanobis_distances.begin();
    while (it != mahalanobis_distances.end()) {
        if (it->first > chi_square_05[x.data.size()]) {
            it = mahalanobis_distances.erase(it);
        } else {
            it++;
        }
    }

    return mahalanobis_distances;
}
