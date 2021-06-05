#include <iostream>
#include <vector>
#include <math.h>  
#include <cmath>
#include <chrono>
#include "../include/random.h"
#include "../include/busqmulbas.h"

using namespace std;

/*
    Ejecuta el algoritmo de Búsqueda Multiarranque Básica en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos

        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo
*/
int busquedaMultiArranque(PAR &par, int seed, bool mostrarEstado, bool mostrarEvolucionFitness){
    // Asignamos semilla aleatoria
    Set_random(seed);

    // Limpiamos el problema
    par.clear();

    int evaluaciones_max_ff = 10000;  

    // Evolución funcion objetivo
    vector<double> inicios_fit = par.getPeoresFitnessTrayectoria();
    vector<double> finales_fit = par.getMejoresFitnessTrayectoria();

    // Var de pruebas
    double total=0;
    int it=0;

    auto begin = chrono::high_resolution_clock::now();
    for(int i=0; i<10; i++){
        par.setIterationsFF(0);

        // Inicializamos aleatoriamente la asignación de clústers
        par.crearSolucionAleatoria();
        inicios_fit.push_back(par.getFuncionObjetivo());

        // Iniciamos búsqueda
        bool continua = true;
        while(continua){
            continua = par.buscarPrimerVecinoMejor(evaluaciones_max_ff);
            it++;
        }
        finales_fit.push_back(par.getFuncionObjetivo());
    }        

    par.simularMejorSolucion();

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    string name = "BMB";
    

    cout << "Problema "+name+" (seed " << seed<< "):   ";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion() << "\t IteracionesBL: "<< it;
    cout << endl;
    
    if(mostrarEstado){
        par.mostrarEstado(); 
    }

    if(mostrarEvolucionFitness){
        cout << endl << endl << "Enfriamiento Simulado Peores " << endl << "BMB_peores=[";
        for(int i=0; i<inicios_fit.size()-1;i++){
            cout << inicios_fit[i] << ", ";
        }
        cout << inicios_fit[inicios_fit.size()-1] <<"]\n";

        cout << endl << "Enfriamiento Simulado Mejores " << endl << "BMB_mejores=[";
        for(int i=0; i<finales_fit.size()-1;i++){
            cout << finales_fit[i] << ", ";
        }
        cout << finales_fit[finales_fit.size()-1] <<"]\n";
    }
    
    return elapsed.count();
}
