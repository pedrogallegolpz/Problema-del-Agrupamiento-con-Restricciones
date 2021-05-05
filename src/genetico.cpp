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

    par.clear();
    // Asignamos el tamaño de la población
    par.setTamPoblacion(tam);

    int tam_pob;
    if(tipo==1){
        tam_pob=2;
    }else{
        tam_pob=par.getTamPoblacion();
    }
    int valoraciones_funcion_objetivo=100000;    
    
    // Ponemos las iteraciones a 0
    par.setIterationsFF(0);

    // Evolución funcion objetivo
    vector<double> fobjetivo_evol;

    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la población
    if(!par.crearPoblacionAleatoria()){
        cout << "Error al crear población aleatoria en el algoritmo genético Local en PAR." << endl;
    }
    
    // Si queremos mostrar la evolución de la función fitness
    if(mostrarEvolucionFitness){
        vector<vector<double>> p=par.getPoblacion();
        fobjetivo_evol.push_back(p[0][p[0].size()-1]);
    }

    // Buscamos mejores vecinos hasta llegar al mejor
    while(par.getIterationsFF()<valoraciones_funcion_objetivo){
        par.runEpoch(tipo,cruce,0,0,false);
        if(mostrarEvolucionFitness){
            vector<vector<double>> p=par.getPoblacion();
            fobjetivo_evol.push_back(p[0][p[0].size()-1]);
        }
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
        c="U";
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
        cout << endl << "EVOLUCIÓN "+name << endl << name+"=[";
        for(int i=0; i<fobjetivo_evol.size()-1;i++){
            cout << fobjetivo_evol[i] << ", ";
        }
        cout << fobjetivo_evol[fobjetivo_evol.size()-1] <<"]\n";
    }

    return elapsed.count();
}
