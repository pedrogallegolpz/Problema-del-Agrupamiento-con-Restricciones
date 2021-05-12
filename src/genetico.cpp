#include <iostream>
#include <vector>
#include <math.h>  
#include <chrono>
#include "../include/random.h"
#include "../include/genetico.h"

using namespace std;

/*
    Ejecuta el algoritmo Genético entero sobre un problema PAR

        -tam:                       tamaño de la población
        -tipo:                      1 si es estacionario, 2 generacional
        -cruce:                     1 si es uniforme, 2 segmento fijo
        -mostrarEstado:             muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos*/
int genetico(PAR &par, int tam, int tipo, int cruce, int seed, bool mostrarEstado, bool mostrarEvolucionFitness){
    // Asignamos semilla aleatoria
    Set_random(seed);

    // Limpiamos el problema
    par.clear();

    // Asignamos el tamaño de la población
    par.setTamPoblacion(tam);
    // Ponemos las iteraciones a 0
    par.setIterationsFF(0);

    int valoraciones_funcion_objetivo=100000;    
    
   
    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la población
    if(!par.crearPoblacionAleatoria()){
        cout << "Error al crear población aleatoria en el algoritmo genético Local en PAR." << endl;
    }
    
    // Buscamos mejores vecinos hasta llegar al mejor
    while(par.getIterationsFF()<valoraciones_funcion_objetivo){
        par.runEpoch(tipo,cruce,0,0,false);
    }
    
    
    // Finalizamos el algoritmo y nos quedamos con el mejor
    par.finishEpochs();
        
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    string t;
    string c;
    if(tipo == 1){
        t="E";
    }else{
        t="G";
    }

    if(cruce==1){
        c="UN";
    }else{
        c="SF";
    }

    string name = "AG"+t+c;

    cout << "Problema "+name+" (seed " << seed<< "):   ";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion() << endl;
    
    if(mostrarEstado){
        par.mostrarEstado(); 
    }

    if(mostrarEvolucionFitness){
        // Evolución funcion objetivo
        vector<double> recorrido_fun_objetivo = par.getRecorridoFunObjetivo();

        cout << endl << "EVOLUCIÓN "+name << endl << name+"=[";
        for(int i=0; i<recorrido_fun_objetivo.size()-1;i++){
            cout << recorrido_fun_objetivo[i] << ", ";
        }
        cout << recorrido_fun_objetivo[recorrido_fun_objetivo.size()-1] <<"]\n";
    }

    return elapsed.count();
}
