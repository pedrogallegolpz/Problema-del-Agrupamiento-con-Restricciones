/******************************************************
 * 
 *      @author: Pedro Gallego López
 *      @subject: Metaheurísticas. Práctica 1
 *      @university: Universidad de Granada
 *      @date: marzo 2021
 * 
 * ***************************************************/


/* -------------------------------------------------
                    MAIN FILE
   ------------------------------------------------- */

#include <iostream>
#include <vector>
#include <string>
#include "random.h"
#include "par.h"
#include "readfiles.h"

using namespace std;


int main(int argc, char * argv[]) {

    string filestring(argv[1]);

    if(argc != 2){
        cout << "Uso: " << argv[0] << " ./data/<filename> " << endl << "No hay que poner el .dat en el archivo por parámetro." << endl << endl;
        return 1;
    }else if(filestring!="./data/bupa_set" && filestring!="./data/zoo_set" && filestring!="./data/glass_set"){
        cout << "No existe ese archivo. Por favor introduzca uno de los siguientes:" << endl;
        cout << " - ./data/bupa_set" << endl << " - ./data/zoo_set" << endl << " - ./data/glass_set" << endl << endl;

        return 1;
    }

    // Estructuras para la lectura
    vector<vector<double> > data;
    vector<vector<double> > const_10;
    vector<vector<double> > const_20;

    // Estructuras PAR
    PAR par_10;
    PAR par_20;

    // Leemos los ficheros
    read_dat(argv[1],data);//leemos el fichero.dat
    read_const(argv[1],const_10,const_20);//leemos ficheros.const

    // Inificalizamos las estructuras info con la informacion leida
    par_10.setInstancias(data); 
    par_10.setNumInstancias(data.size());
    par_10.setNumAtributos(data[0].size());
    par_10.setRestricciones(const_10);

    par_20.setInstancias(data); 
    par_20.setNumInstancias(data.size());
    par_20.setNumAtributos(data[0].size());
    par_20.setRestricciones(const_20);

    
}
