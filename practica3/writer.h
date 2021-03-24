#include <string>
#include "utils.h"
#include <fstream>
#include <iostream>

#ifndef WRITER_H
#define WRITER_H

struct objeto {

    std::string name;
    float perimeter = 0.0;
    float area = 0.0;
    float firstHuMoment = 0.0;
    float secondHuMoment = 0.0;
    float thirdHuMoment = 0.0;

    objeto(std::string nombre) : name(nombre) {}

    void set(descriptores d){
        perimeter = d.perimeter;
        area = d.area;
        firstHuMoment = d.fHuMoment;
        secondHuMoment = d.secHuMoment;
        thirdHuMoment = d.thirdHuMoment;
    }
};

class Writer {

    std::string fichero;
    
    public:

        Writer(std::string file) : fichero(file) {}
        void getItems(std::vector<objeto>& items);
        void writeMetrics(std::vector<objeto>& everyItem, std::vector<objeto>& grupos,
                          std::vector<descriptores>& medias, std::vector<descriptores>& varianzas);

};


#endif