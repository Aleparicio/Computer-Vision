#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <list>
#include <iostream>
#include "utils.h"
#include "writer.h"
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

    string fichero = "objetos.txt";
    if (argc != 3) {
        std::cout << "Invocar como: aprender nomfich nomobj" << std::endl;
        return 0;
    }

    // Lectura del nuevo item
    std::string image_path = cv::samples::findFile(argv[1]);
    cv::Mat frame = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    frame = thresholding(frame, OTSU);
    if (frame.empty())
        return 0;

    // Lectura del fichero
    std::vector<Objeto> everyItem;
    Writer wrt(fichero);
    wrt.getItems(everyItem);

    // Adición de un nuevo ítem
    Descriptor d = getDescriptors(frame);
    Objeto elemento(argv[2]);
    elemento.set(d);
    everyItem.push_back(elemento);

    // Creación de las estadísticas
    std::vector<Objeto> grupos;
    std::vector<int> ocurrencias;
    for (int i = 0; i < everyItem.size(); ++i) {
        bool existe = false;
        for (int j = 0; j < grupos.size(); ++j) {
            if (everyItem[i].name.compare(grupos[j].name) == 0) {
                existe = true;
                grupos[j].area += everyItem[i].area;
                grupos[j].perimeter += everyItem[i].perimeter;
                grupos[j].firstHuMoment += everyItem[i].firstHuMoment;
                grupos[j].secondHuMoment += everyItem[i].secondHuMoment;
                grupos[j].thirdHuMoment += everyItem[i].thirdHuMoment;
                ocurrencias[j] += 1;
            }
        }

        if (!existe) {
            grupos.push_back(everyItem[i]);
            ocurrencias.push_back(1);
        }
    }

    // Medias
    std::vector<Descriptor> medias(grupos.size(), Descriptor());
    for (int i = 0; i < grupos.size(); ++i) {
        medias[i].area = grupos[i].area / (float)ocurrencias[i];
        medias[i].perimeter = grupos[i].perimeter / (float)ocurrencias[i];
        medias[i].firstHuMoment = grupos[i].firstHuMoment / (float)ocurrencias[i];
        medias[i].secondHuMoment = grupos[i].secondHuMoment / (float)ocurrencias[i];
        medias[i].thirdHuMoment = grupos[i].thirdHuMoment / (float)ocurrencias[i];
    }

    // Varianzas
    std::vector<Descriptor> varianzas(grupos.size(), Descriptor());
    for (int i = 0; i < grupos.size(); ++i) {
        for (int j = 0; j < everyItem.size(); ++j) {
            if (everyItem[j].name.compare(grupos[i].name) == 0) {
                varianzas[i].area += pow((everyItem[j].area - medias[i].area), 2);
                varianzas[i].perimeter += pow((everyItem[j].perimeter - medias[i].perimeter), 2);
                varianzas[i].firstHuMoment += pow((everyItem[j].firstHuMoment - medias[i].firstHuMoment), 2);
                varianzas[i].secondHuMoment += pow((everyItem[j].secondHuMoment - medias[i].secondHuMoment), 2);
                varianzas[i].thirdHuMoment += pow((everyItem[j].thirdHuMoment - medias[i].thirdHuMoment), 2);
            }
        }
    }

    for (int i = 0; i < varianzas.size(); ++i) {
        if (ocurrencias[i] == 1) {
            varianzas[i] = Descriptor();
        } else {
            varianzas[i].area /= (ocurrencias[i] - 1);
            varianzas[i].perimeter /= (ocurrencias[i] - 1);
            varianzas[i].firstHuMoment /= (ocurrencias[i] - 1);
            varianzas[i].secondHuMoment /= (ocurrencias[i] - 1);
            varianzas[i].thirdHuMoment /= (ocurrencias[i] - 1);
        }
    }

    wrt.writeMetrics(everyItem, grupos, medias, varianzas);

    return 0;
}
