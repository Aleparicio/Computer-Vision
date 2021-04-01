#pragma once

#include <unordered_map>
#include <map>
#include <vector>
#include <set>

#include "descriptors.hpp"

enum VarianceType {
    CLASSIC,
    A_PRIORI_ESTIMATION
};

struct ClassStats {
    int N; // número de muestras
    Descriptors mean;
    Descriptors variance;

    ClassStats() {}
    ClassStats(int _N, Descriptors _mean, Descriptors _variance)
        : N(_N), mean(_mean), variance(_variance) {}
};

class MahalanobisClassifier {
  public:
    // Distribución chi cuadrado con alfa 0.05 para grados de libertad 0-10.
    const float chi_square_05[11] = {0, 3.84, 5.99, 7.81, 9.49, 11.07, 12.59, 14.07, 15.51, 16.92, 18.31};

    // Estadísticas de cada clase
    //
    // Forma de acceder a ellas:
    // stats[class].stat.descriptor
    std::unordered_map<std::string, ClassStats> stats;

    // Guardar modelo en un fichero
    //
    // Formato del modelo:
    // clase N medias_descriptores varianzas_descriptores
    // ...
    void save_model(const std::string& file);

    // Leer modelo de un fichero
    void load_model(const std::string& file);

    // Entrenar el modelo creando las estadísticas de los descriptores para cada clase
    void train(const std::vector<Descriptors>& X, const std::vector<std::string>& Y, VarianceType vtype = CLASSIC, float a_priori_percent = 1);

    // Predice la clase de un objeto dados sus descriptores
    //
    // Si el objeto pasa el test de Mahalanobis con una sola clase, el sistema declara que se trata de esa clase
    // Si pasa el test con más de un objeto, declarará que tiene dudas entre las clases
    // Si no pasa el test con ninguna clase, declarará que el objeto es desconocido
    //
    // La distancia de Mahalanobis sigue una distribución Chi cuadrado con <número de descriptores> grados de libertad
    // Si la distancia de la muestra con una clase es menor que Chi^2_0.05(<número descriptores>), pertenece
    // a la clase con un 95% de probabilidad
    std::set<std::pair<float, std::string>> predict(const Descriptors& x) const;
};
