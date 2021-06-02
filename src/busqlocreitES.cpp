#include <iostream>
#include <vector>
#include <math.h>  
#include <cmath>
#include <chrono>
#include "../include/random.h"
#include "../include/busqlocreitES.h"
#include "../include/enfriamientosimulado.h"

using namespace std;

/*
    Ejecuta el algoritmo de Búsqueda Local Reiterada en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos

        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo
*/
int busquedaLocalReiteradaES(PAR &par, int seed, bool mostrarEstado, bool mostrarEvolucionFitness){
    Set_random(seed);

    // Limpiamos el problema
    par.clear();

    int evaluaciones_max_ff = 10000;  

    // Evolución funcion objetivo
    vector<double> inicios_fit = par.getPeoresFitnessTrayectoria();
    vector<double> finales_fit = par.getMejoresFitnessTrayectoria();

    // Inicializamos constantes
    int size = 0.1*par.getNumInstancias();
    double mu = 0.3;     // Se admiten soluciones de hasta 'mu' tanto por ciento de empeoramiento
    double phi = 0.3;    // Se aceptan las soluciones admitidas con probabilidad 'phi'
    int max_vecinos = 10*par.getNumInstancias(); // max_vecinos generados en cada iteracion
    int max_exitos = 0.1*max_vecinos;
    
    int valoraciones_funcion_objetivo = 100000;  

    
    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la asignación de clústers
    if(!par.crearSolucionAleatoria()){
        cout << "Error al asignar instancias aleatoriamente en el algoritmo de enfriamiento simulado." << endl;
    }

    // Generamos las temperaturas
    double Ti = (mu*par.fitnessFunction())/(-log(phi));
    double Tf = 0.001;

    // Nos aseguramos de que Ti sea menor que Tf
    if(Ti<=Tf){
        Tf=Ti*Tf;
    }
    
    int M = 10000/max_vecinos;
    vector<double> temperaturas = generarTemperaturas( Ti, Tf, M);


    // Inicializamos aleatoriamente la asignación de clústers
    if(!par.crearSolucionAleatoria()){
        cout << "Error al asignar instancias aleatoriamente en el algoritmo de enfriamiento simulado." << endl;
    }
    inicios_fit.push_back(par.getFuncionObjetivo());

    // Iniciamos búsqueda
    int iteraciones=0;
    bool para =false;
    while(par.getIterationsFF()<evaluaciones_max_ff && iteraciones<M+1 && para==false){
        para = par.enfriamientoSimulado(temperaturas[iteraciones], max_vecinos, max_exitos);
        iteraciones++;
    }
    finales_fit.push_back(par.getFuncionObjetivo());


    // Comenzamos el bucle con soluciones obtenidas anteriormente
    for(int i=1; i<10; i++){
        par.setIterationsFF(0);

        // Mutamos
        par.mutacionILS(size);
        inicios_fit.push_back(par.getFuncionObjetivo());

        // Iniciamos búsqueda
        int iteraciones=0;
        bool para =false;
        while(par.getIterationsFF()<evaluaciones_max_ff*(i+1) && iteraciones<M+1 && para==false){
            para = par.enfriamientoSimulado(temperaturas[iteraciones], max_vecinos, max_exitos);
            iteraciones++;
        }
        finales_fit.push_back(par.getFuncionObjetivo());

    }        

    par.simularMejorSolucion();

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    string name = "ILSES";
    
    cout << "Problema "+name+" (seed " << seed<< "):   ";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion();
    cout << endl;
    
    if(mostrarEstado){
        par.mostrarEstado(); 
    }

    if(mostrarEvolucionFitness){
        cout << endl << "Enfriamiento Simulado Mejores " << endl << "ES_mejores=[";
        for(int i=0; i<inicios_fit.size()-1;i++){
            cout << inicios_fit[i] << ", ";
        }
        cout << inicios_fit[inicios_fit.size()-1] <<"]\n";


        cout << endl << endl << "Enfriamiento Simulado Peores " << endl << "ES_mejores=[";
        for(int i=0; i<finales_fit.size()-1;i++){
            cout << finales_fit[i] << ", ";
        }
        cout << finales_fit[finales_fit.size()-1] <<"]\n";
    }
    
    return elapsed.count();
}
