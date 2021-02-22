#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <iomanip>

void rotulos(){
    cout << "             PRUEBAS PARA EL MODULO TIPO PERSONA" << endl;
	cout << "             ===================================" << endl << endl;
	cout << "    Seleccione la operacion (0 para terminar):" << endl;
	cout << "		1.- Definir persona: " << endl;
	cout << "		2.- Fecha nacimiento: " << endl;
	cout << "		3.- Es mujer " << endl << endl;
}

int main() {

    cv::VideoCapture cap;
    if (!cap.open(0)) {
        std::cout << "Failed to connect to cam" << std::endl;
        return 0;
    }


	int operacion;
    rotulos();

	cout << "Operacion (necesario definir una persona): " << flush;
	cin >> operacion;
	while(operacion != 0){
		if (operacion == 1){
			int d, m , aa;
			cout << "Inserte dia mes y anyo: " << flush;
			cin >> d >> m >> aa;
			cout << "¿Es mujer? (true o false): " << flush;
			bool esMujerD;
			cin >> boolalpha >> esMujerD;
		    p = definirPersona(d,m,aa,esMujerD);
			int fecha=nacido(p);
			cout << fecha << endl;
			if(esMujer(p)){
				cout << "Es mujer." << endl << endl;
			}
			else{
				cout << "No es mujer" << endl << endl;
			}
		}
		if (operacion == 2){
			int fechan = nacido(p);
			cout << "Esta persona ha nacido el dia (aaaammdd): " << fechan << endl << endl;
		}
		if (operacion == 3){
			bool loes = esMujer(p);
			if(loes){
				cout << "Esta persona es una mujer." << endl << endl;
			}
			else{
				cout << "Esta persona no es una mujer." << endl << endl;
			}
		}
		rotulos();
		cout << "Operacion (necesario definir una persona): " << flush;
		cin >> operacion;
	}
    return 0;
    while (true) {
        cv::Mat frame;

        cap >> frame;
        if (frame.empty()) // end of video stream
            break;

        // Procesar el frame
        if (val > 0) {
            cv::GaussianBlur(frame, frame, cv::Size(0, 0), ((double)val)/10); // aplicar filtro gaussiano
        }

        std::string label = "amount: " + std::to_string(val) + "%";
        cv::putText(frame, label, cv::Point(20, 30), cv::FONT_HERSHEY_PLAIN, 2.0, CV_RGB(0,255,0), 2.0);

        cv::imshow("Gaussian Blur", frame);
        // if(cv::waitKey(10) == 27) // stop capturing when key is pressed
        // if(cv::waitKey(10) != -1) // stop capturing when key is pressed
        if(cv::waitKey(10) >= 0) // stop capturing when key is pressed
            break;
    }

    // cv::destroyAllWindows();

    // the camera will be closed automatically upon exit
    // wtr.release();
    // cap.close();
    return 0;
}
