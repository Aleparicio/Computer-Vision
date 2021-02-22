#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Filters.h"
#include <iostream>
#include <iomanip>

using namespace std;

const std::string window_name = "Efecto";

void rotulos(){
    cout << endl << endl << " SELECCIONE EL FILTRO A EMPLEAR" << endl;
	cout << " ===================================================" << endl;
	cout << "   Filtro a aplicar (0 para terminar):" << endl;
	cout << "   1.- Contraste y/o equalización del histograma" << endl;
	cout << "   2.- Alien " << endl;
	cout << "   3.- Póster " << endl;
    cout << "   4.- Distorsión " << endl;
    cout << "Para terminar la aplicación de un filtro pulsar cualquier tecla." << endl;
	cout << "Seleccionar filtro: " << flush;
}

void rotulosEqContraste(){
    cout << endl << endl << " SELECCIONAR SUBTIPO (0 para terminar)" << endl;
	cout << " =================================================" << endl;
	cout << "   1 - Ampliación contraste" << endl;
	cout << "   2 - Equalización Histograma " << endl;
	cout << "   3 - CLAHE " << endl;
    cout << "Para terminar la aplicación de un filtro pulsar cualquier tecla." << endl;
	cout << "Seleccionar filtro: " << flush;
}

int procesarVideo(const filtro filter, float arg1Filter = 1.0, float arg2Filter = 0.0) {

    cv::VideoCapture cap;
    if (!cap.open(0)) {
        std::cout << "Failed to connect to camera." << std::endl;
        return 0;
    }

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    // cv::createTrackbar("Blur", "Gaussian Blur", &val, ALPHA_SLIDER_MAX, NULL);

    while (true) {

        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) // end of video stream
            break;

        if (filter == CONTRASTE) { 
            contrastAdjustment(frame, arg1Filter, arg2Filter);
        } else if (filter == EQHISTO) {
            equalizeHist(frame);
        } else if (filter == CLAHE) {
            equalizeCLAHE(frame, arg1Filter);
        } else if (filter == ALIEN) {
            //cv::imshow(window_name, frame);
        } else if (filter == POSTER) {
            //cv::imshow(window_name, frame);
        } else if (filter == DISTORSION) {
            //cv::imshow(window_name, frame);
        }

        cv::imshow(window_name, frame);
        if(cv::waitKey(10) >= 0) // stop capturing when key is pressed
            break;
    }

    cv::destroyAllWindows();
    return 1;
}

int main() {

	int operacion;
    rotulos();

	std::cin >> operacion;
	while(operacion != 0){

		if (operacion == 1){
			// Invocar Contraste y/o equalización del histograma
            rotulosEqContraste();
            std::cin >> operacion;
            if(operacion == 1){ 
                cout << "Introduce ganancia: ";
                float ganancia = 0.0, sesgo = 0.0;
                std::cin >> ganancia;
                cout << "Introduce sesgo: ";
                std::cin >> sesgo;
                procesarVideo(CONTRASTE, ganancia, sesgo); 
            }
            else if(operacion == 2){ 
                procesarVideo(EQHISTO); 
            }
            else if(operacion == 3){ 
                cout << "Introduce clip limit: ";
                float clipLimit = 0.0;
                std::cin >> clipLimit;
                procesarVideo(CLAHE, clipLimit); 
            }
            else { break; }
		}
		if (operacion == 2){
            // Invocar Alien
            procesarVideo(ALIEN);
		}
		if (operacion == 3){
			// Invocar póster
            procesarVideo(POSTER);
		}
        if (operacion == 4){
			// Invocar Distorsión
            procesarVideo(DISTORSION);
		}

        rotulos();
		std::cin >> operacion;
	}
    return 0;
}
