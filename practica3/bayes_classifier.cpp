#include <iostream>
#include <fstream>

#include "bayes_classifier.hpp"
#include "descriptors.hpp"

// Leer modelo de un fichero
//
// Formato del modelo:
// clase media_descriptores media_varianza
// ...
void BayesClassifier::load_model(std::string file) {
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
// clase media_descriptores media_varianza
// ...
void BayesClassifier::save_model(std::string file) {
    // Creación de los datos del nuevo fichero
    std::ofstream os(file);
    for (const auto& entry : stats) {
        os << entry.first << entry.second.mean << entry.second.variance << std::endl;
    }
}

// Entrenar el modelo creando las estadísticas de los descriptores
void BayesClassifier::train(std::vector<Descriptors> X, std::vector<std::string> Y) {
    //     std::vector<Object>
    //         grupos;
    //     std::vector<int> ocurrencias;
    //     for (int i = 0; i < everyItem.size(); ++i) {
    //         bool existe = false;
    //         for (int j = 0; j < grupos.size(); ++j) {
    //             if (everyItem[i].name.compare(grupos[j].name) == 0) {
    //                 existe = true;
    //                 grupos[j].descs.area += everyItem[i].area;
    //                 grupos[j].descs.perimeter += everyItem[i].perimeter;
    //                 grupos[j].descs.firstHuMoment += everyItem[i].firstHuMoment;
    //                 grupos[j].secondHuMoment += everyItem[i].secondHuMoment;
    //                 grupos[j].thirdHuMoment += everyItem[i].thirdHuMoment;
    //                 ocurrencias[j] += 1;
    //             }
    //         }
    //         if (!existe) {
    //             grupos.push_back(everyItem[i]);
    //             ocurrencias.push_back(1);
    //         }
    //     }

    //     // Medias
    //     std::vector<Descriptor> medias(grupos.size(), Descriptor());
    //     for (int i = 0; i < grupos.size(); ++i) {
    //         medias[i].area = grupos[i].area / (float)ocurrencias[i];
    //         medias[i].perimeter = grupos[i].perimeter / (float)ocurrencias[i];
    //         medias[i].firstHuMoment = grupos[i].firstHuMoment / (float)ocurrencias[i];
    //         medias[i].secondHuMoment = grupos[i].secondHuMoment / (float)ocurrencias[i];
    //         medias[i].thirdHuMoment = grupos[i].thirdHuMoment / (float)ocurrencias[i];
    //     }

    //     // Varianzas
    //     std::vector<Descriptor> varianzas(grupos.size(), Descriptor());
    //     for (int i = 0; i < grupos.size(); ++i) {
    //         for (int j = 0; j < everyItem.size(); ++j) {
    //             if (everyItem[j].name.compare(grupos[i].name) == 0) {
    //                 varianzas[i].area += pow((everyItem[j].area - medias[i].area), 2);
    //                 varianzas[i].perimeter += pow((everyItem[j].perimeter - medias[i].perimeter), 2);
    //                 varianzas[i].firstHuMoment += pow((everyItem[j].firstHuMoment - medias[i].firstHuMoment), 2);
    //                 varianzas[i].secondHuMoment += pow((everyItem[j].secondHuMoment - medias[i].secondHuMoment), 2);
    //                 varianzas[i].thirdHuMoment += pow((everyItem[j].thirdHuMoment - medias[i].thirdHuMoment), 2);
    //             }
    //         }
    //     }

    //     for (int i = 0; i < varianzas.size(); ++i) {
    //         if (ocurrencias[i] == 1) {
    //             varianzas[i] = Descriptor();
    //         } else {
    //             varianzas[i].area /= (ocurrencias[i] - 1);
    //             varianzas[i].perimeter /= (ocurrencias[i] - 1);
    //             varianzas[i].firstHuMoment /= (ocurrencias[i] - 1);
    //             varianzas[i].secondHuMoment /= (ocurrencias[i] - 1);
    //             varianzas[i].thirdHuMoment /= (ocurrencias[i] - 1);
    //         }
    //     }
}

std::vector<std::string> BayesClassifier::predict(Descriptors x) {
    // Devuelve un vector con las posibles clases

    // Si el objeto pasa el test de Mahalanobis con una sola clase, el sistema declara que se trata de esa clase

    // Si pasa el test con más de un objeto, declarará que tiene dudas entre las clases

    // Si no pasa el test con ninguna clase, declarará que el objeto es desconocido
}
