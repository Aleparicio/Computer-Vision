#pragma once

#include <unordered_map>
#include <vector>

#include "descriptors.hpp"

struct ClassStats {
    int N; // número de muestras
    Descriptors mean;
    Descriptors variance;

    ClassStats() {}
    ClassStats(Descriptors _mean, Descriptors _variance) : mean(_mean), variance(_variance) {}
};

class BayesClassifier {
  public:
    // Estadísticas de cada clase
    //
    // Forma de acceder a ellas:
    // stats[class].stat.descriptor
    std::unordered_map<std::string, ClassStats> stats;

    void save_model(const std::string& file);
    void load_model(const std::string& file);

    void train(const std::vector<Descriptors>& X, const std::vector<std::string>& Y);
    std::vector<std::string> predict(const Descriptors& x);
};
