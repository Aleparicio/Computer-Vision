#pragma once

#include "descriptors.hpp"

class Writer {

    std::string file;

  public:
    Writer(std::string _file) : file(_file) {}
    void getItems(std::vector<Object>& items);
    void getMetrics(std::vector<Object>& medias, std::vector<Object>& varianzas);
    void writeMetrics(std::vector<Object>& everyItem, std::vector<Object>& grupos,
                      std::vector<Descriptors>& medias, std::vector<Descriptors>& varianzas);
};
