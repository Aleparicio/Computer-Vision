#include "writer.h"

void Writer::getItems(std::vector<objeto>& items){

    items.clear();
    std::string item;
    std::ifstream file(fichero);

    if(file){
        file >> item;
                
        while(item.compare("Estadísticas") != 0){
                    
            objeto obj(item);
            file >> item;
            obj.perimeter = stof(item);
            file >> item;
            obj.area = stof(item);
            file >> item;
            obj.firstHuMoment = stof(item);
            file >> item;
            obj.secondHuMoment = stof(item);
            file >> item;
            obj.thirdHuMoment = stof(item);
            file >> item;

            items.push_back(obj);

        }
    } else {
        std::cerr << "No existe el fichero." << std::endl;
    }  

    file.close();
}


void Writer::writeMetrics(std::vector<objeto>& everyItem, std::vector<objeto>& grupos,
                         std::vector<descriptores>& medias, std::vector<descriptores>& varianzas ){


    // Creación de los datos del nuevo fichero
    std::ofstream newFile(fichero);
    for(int j = 0; j < everyItem.size(); ++j){
        newFile << everyItem[j].name << "\t" << everyItem[j].perimeter << "\t" << everyItem[j].area 
                << "\t" << everyItem[j].firstHuMoment << "\t" << everyItem[j].secondHuMoment << "\t"
                << everyItem[j].firstHuMoment << "\n";
    }

    newFile << "Estadísticas" << "\n";

    for(int j = 0; j < grupos.size(); ++j){
        newFile << grupos[j].name << std::setw(20) << "\tMedias:\t" << medias[j].perimeter << "\t" 
                << medias[j].area << "\t" << medias[j].fHuMoment << "\t" << medias[j].secHuMoment << "\t"
                << medias[j].thirdHuMoment << "\n";
        newFile << grupos[j].name << std::setw(20) << "\tVarianzas:\t" << varianzas[j].perimeter << "\t" 
                << varianzas[j].area << "\t" << varianzas[j].fHuMoment << "\t" << varianzas[j].secHuMoment << "\t"
                << varianzas[j].thirdHuMoment << "\n";
    }
}