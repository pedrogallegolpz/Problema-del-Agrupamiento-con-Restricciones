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
#include "greedyCOPKM.h"

using namespace std;


int main(int argc, char * argv[]) {

    

    if(argc != 2){
        cout << "Uso: " << argv[0] << " ./data/<filename> " << endl << "No hay que poner el .dat en el archivo por parámetro." << endl << endl;
        return 1;
    }

    string filestring(argv[1]);

    if(filestring!="./data/bupa_set" && filestring!="./data/zoo_set" && filestring!="./data/glass_set"){
        cout << "No existe ese archivo. Por favor introduzca uno de los siguientes:" << endl;
        cout << " - ./data/bupa_set" << endl << " - ./data/zoo_set" << endl << " - ./data/glass_set" << endl << endl;

        return 1;
    }


    // Establecemos el número de clases en función del archivo
    int clases=0;
    if(filestring=="./data/bupa_set"){
        clases=16;
    }else if(filestring=="./data/zoo_set"){
        clases=7;
    }else if(filestring=="./data/glass_set"){
        clases=7;
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
    par_10.setNumClases(clases);


    par_20.setInstancias(data); 
    par_20.setNumInstancias(data.size());
    par_20.setNumAtributos(data[0].size());
    par_20.setRestricciones(const_20);
    par_20.setNumClases(clases);

    cout << "Ejecutamos el problema PAR_CONST_10 con COPKM" << endl;
    greedyCOPKM(par_10);

    
    
}


// Código complementario
//Calculamos maximos y minimos del fichero 
    // RESULTADOS DE EJECUTAR EL CÓDIGO DE ABAJO CON CADA ARCHIVO:
    //      Zoo: máximos={1,1,1,...(16)...,1}, mínimos={0,0,0,...(16)...,0}
    //      Glass: máximos={1,1,1,...(9)...,1}, mínimos={0,0,0,...(9)...,0}
    //      Bupa: máximos={1,1,1,1,1}, mínimos={0,0,0,0,0}
    /*   
    cout << "Los elementos más grandes por posición de " << filestring << " son: [";
    
    for(int j=0; j<data[0].size(); j++){
        double max=-1000000;
        for(int i=0; i<data.size(); i++){
            if(data[i][j]>max){
                max=data[i][j];
            }   
        }
        cout << max << ",";
    }
    cout << "]" <<endl <<endl;

    cout << "Los elementos más pequeños por posición de " << filestring << " son: [";
    
    for(int j=0; j<data[0].size(); j++){
        double min=1000000;
        for(int i=0; i<data.size(); i++){
            if(data[i][j]<min){
                min=data[i][j];
            }   
        }
        cout << min << ",";
    }
    cout << "]" <<endl <<endl;
    */
