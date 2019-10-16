#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <typeinfo>
#include <stdlib.h>

using namespace std;

int main() {

    ifstream archivo;
    ofstream archivoGuardado;
    string nombreArchivo = "archivoguardado.txt";
    int cont = 0;

    archivoGuardado.open(nombreArchivo.c_str(),ios::app);

    if(archivoGuardado.fail()){ //Si ha ocurrido algun error
		cout<<"No se pudo abrir el archivo";
		exit(1);
	}
    archivo.open("archivo.txt");

    //Los 2 datos que me importan guardar.
    int offsetNota;
    char tipo;
    string a;

    //Mientras no sea el final del archivo.
    while (!archivo.eof()) {

      archivo >> a;
      stringstream ss(a);
      string linea;

      for (int i = 0; i < 3; i++) {

          getline (ss, linea, ',');

          if (i == 0) {

            //Aux para comprar en el IF
            int aux = stoi(linea);
            if (aux == 64) {
                char c = 'd';
                tipo = c;
            }
            else if (aux == 192) {
                char c = 'f';
                tipo = c;
            }
            else if (aux == 320) {
                char c = 'j';
                tipo = c;
            }
            else if (aux == 448) {
                char c = 'k';
                tipo = c;
            }
          }
          else if (i == 2) {
            offsetNota = stoi(linea);
          }
      }
      cont++;
      cout<<"Linea "<< cont <<endl;
      archivoGuardado << offsetNota;
      archivoGuardado << "," <<tipo << endl;
    }
    archivo.close();
    archivoGuardado.close();
    cout << "Proceso terminado";
    return 0;
}
