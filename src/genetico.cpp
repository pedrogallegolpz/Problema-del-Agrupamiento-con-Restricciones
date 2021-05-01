#include <iostream>
#include <vector>
#include <math.h>  
#include <chrono>
#include "../include/random.h"
#include "../include/genetico.h"

using namespace std;

/*
    Ejecuta el algoritmo Genético entero sobre un problema PAR

        -tam:   tamaño de la población
        -tipo:  1 si es estacionario, 2 generacional
        -cruce: 1 si es uniforme, 2 segmento fijo
        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
*/
void genetico(PAR par, int tam, int tipo, int cruce, int seed, bool mostrarEstado){
    // Asignamos semilla aleatoria
    Set_random(seed);

    // Asignamos el tamaño de la población
    par.setTamPoblacion(tam);
    
    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la población
    if(!par.crearPoblacionAleatoria()){
        cout << "Error al crear población aleatoria en el algoritmo genético Local en PAR." << endl;
    }
    
    int tam_pob;
    if(tipo==1){
        tam_pob=2;
    }else{
        tam_pob=par.getTamPoblacion();
    }
    int valoraciones_funcion_objetivo=100000;    
    int iteraciones = valoraciones_funcion_objetivo/tam_pob;
    
    // Buscamos mejores vecinos hasta llegar al mejor
    for(int i=0; i<iteraciones; i++){
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
}
