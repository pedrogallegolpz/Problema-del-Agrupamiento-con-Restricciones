
#include <iostream>
#include <vector>
#include "../include/prueba.h"

using namespace std;


void insertar(vector<double> &poblacion, double elemento){
    if(poblacion.empty()){
        poblacion.push_back(elemento);   
    }else{
        double movimiento = ((float)(poblacion.size()-1))/2;
        double elemento_medio = movimiento;
        bool insertado = false;

        // Insertamos de forma ordenada
        while(!insertado){
            int it = elemento_medio;
            int iteraciones=0;
            if(movimiento<1){
                // Comparamos las funciones objetivo
                if(elemento < poblacion[it]){
                    for(int i=it; i>=0 && !insertado; i--){
                        if(elemento>=poblacion[i]){
                            poblacion.insert(poblacion.begin()+i+1, elemento);
                            insertado=true;
                        }
                        iteraciones++;
                    }
                    // Si no se ha insertado es porque es el más pequeño
                    if(!insertado){
                        poblacion.insert(poblacion.begin(),elemento);
                        insertado=true;
                    }
                }else if(elemento> poblacion[it]){
                    for(int i=it; i<poblacion.size() && !insertado; i++){
                        if(elemento<=poblacion[i]){
                            poblacion.insert(poblacion.begin()+i, elemento);
                            insertado=true;
                        }
                        iteraciones++;
                    } 
                    // Si no se ha insertado es porque es el más grande
                    if(!insertado){
                        poblacion.insert(poblacion.end(),elemento);
                        insertado=true;
                    }               
                }else{
                    poblacion.insert(poblacion.begin()+it,elemento);
                    insertado=true;
                } 
                cout<< "Iteraciones: " << iteraciones << endl;
            }else{
                movimiento = movimiento/2;  // Lo que nos vamos a desplazar para encontrar la posición

                // Comparamos las funciones objetivo
                if(elemento < poblacion[it]){
                    elemento_medio -= movimiento;
                }else if(elemento> poblacion[it]){
                    elemento_medio += movimiento;
                }else{
                    poblacion.insert(poblacion.begin()+it,elemento);
                    insertado=true;
                } 
               
            }
        }      
    }
    
}