/*  -------------------------------------------------------------------------
            DEFINICIÓN DEL PROBLEMA DEL AGRUPAMIENTO CON RESTRICCIONES 
    -------------------------------------------------------------------------   */

#ifndef __PAR_H
#define __PAR_H

#include <iostream>
#include <vector>
#include "../include/random.h" 

using namespace std;

class PAR{
private:
    double GREATER_DIST;
    int SEED;

    int num_instancias;
    int num_atributos;
    int num_clases;

    bool necesidad_actualizar_centroides;       // Nos dirá si hace falta actualizar los centroides

    double funcion_objetivo;                    // En esta variable se guardará el valor actual de la función objetivo cuando haga falta
    
    int iterations_ff;                          // Cuenta las veces que se usa la función objetivo de manera útil

    /*
        Todas las variables vector que hacen referencia a las instancias
        tienen el mismo orden con las instancias. Es decir, en la posición
        i-ésima del vector instancias, cluster, restricciones o incluso el
        valor i en una posición de ML o CL, hacen referencia a la misma
        instancia.

        El vector de índices solo se usará para calcular las distancias a
        los centroides, para hacer operaciones de modificación.
    */
    vector<vector<double>> instancias;
    vector<vector<double>> centroides;          // Contiene los $num_clases centroides. Uno de cada cluster
    vector<vector<int>> clusters;               // En la posición i contiene los índices de las instancias que pertenecen al cluster i
    vector<int> inst_belong;                    // El entero de la posición i nos dice que la instancia i pertenece al cluster inst_belong[i]  
    
    //Restricciones
    vector<vector<double>> restricciones;
    vector<vector<int>> ML;                     // Restricciones Must-Link (ML). La instancia ML[i,0] tiene que tener relación con ML[i,1]
    vector<vector<int>> CL;                     // Restricciones Must-Link (ML). La instancia ML[i,0] no puede tener relación con ML[i,1]
                                                // NOTA en la posición M[i] hay dos índices que hacen referencia directa a instancias (No a indices)

    // Atributos genéticos y meméticos
    const double PROB_CRUCE = 0.7;              // Probabilidad de que de cruce en una época genética
    const double PROB_MUT_GEN = 0.1;            // Probabilidad de mutación genética
    int TAM_POBLACION;                          // Nos dice el tamaño de la población
    int epoca;                                  // Época poblacional por la que vamos
    vector<vector<double>> poblacion;           // Vector de 50 vectores soluciones. Estas soluciones son de tipo double, pero aún así las 
                                                //  instancias son enteros. Su penúltima componente será la época en la que se generó. Su 
                                                //  última componente si que es un double haciendo referencia al  valor de su función objetivo.

    // Seguiremos el orden según estos índices
    vector<int> indices;                        // Nos dice cómo recorremos las instancias

    /*
        Creamos los vectores ML y CL
    */
    void generarRestricciones();

    
    /*
        Pone los clusters a -1
    */
    void resetClusters();

    /*
        Añade a un elemento a 'poblacion' ordenada según la función
        objetivo de cada solución (posición 0 la mejor). Si 'pob'
        tiene ya TAM_POBLACION elementos entonces borramos el peor.
    */
    void insertPoblacion(vector<double> &elemento, vector<vector<double>> &pob);

public:
    /*
        Pone el num_clases a 0, por lo tanto no hay nada que hacer.
    */
    PAR();

    /*
        Inicializa el número de clases y los datos del problema.
    */
    PAR(int num_atributos_source, int num_clases_source, vector<vector<double>> instancias_source, vector<vector<double>> restricciones_source, int seed_source, int tam_pob);

    /*
        Constructor copia
    */
    PAR(const PAR &par_source);  

    /*
        Limpia todos los cálculos hechos
    */
    bool clear();  

    /*
        Barajar indices
    */
    void shuffleInstances();

    /*
        Calcula la distancia euclídea entre dos puntos de Rn
    */
    double distanciaEntreDosPuntos(vector<double> p1, vector<double> p2);

    /*
        Función para calcular el centroide de un cluster[ci]
    */
    vector<double> calcularCentroide(int ci);

    /*
        Actualiza los centroides
    */
    bool updateCentroides();

    /*
        Calcula el número de restricciones violadas con una instancia concreta
        antes de ser asignada
            -imstancia  índice directamente sobre el vector de instancias
            -clúster    cluster al que se pretende asignar
    */
    int restriccionesConInstancia(int instancia, int cluster);

    /*
        Calcula el incremento de restricciones violadas al cambiar una instancia
        de clúster
            -instancia  índice directamente sobre el vector de instancias
            -c1         cluster actual   
            -c2         cluster al que se pretende asignar
    */
    int incrementoInfeasibility(int instancia, int c1, int c2);


    /*
        Crea solución aleatoria. Devuelve el éxito de la operación
    */
    bool asignarInstanciasAleatoriamente(vector<int> &solucion);



    /*
        Simula una solución. Actualiza clústers, centroides y función objetivo
    */
    bool simularSolucion(vector<double> &solucion);


    ////////////////////////////////////////////////////////////////
    //
    //      Métodos principales para las distintas Metaheurísticas
    //
    ////////////////////////////////////////////////////////////////


    /////////////////////
    //  GREEDY
    /////////////////////
    /*
        USADO PARA LA METAHEURÍSTICA GREEDY COPKM PARA CADA ITERACIÓN

        Asigna cada instancia a su clúster más cercano teniendo que respetar
        las restricciones fuertes y teniendo en cuenta de que cada instancia
        tiene que escoger el clúster con el que incumpla menos restricciones
        débiles.

        Devuelve un bool que nos dice si el método ha provocado algún cambio
        en los centroides.
    */
    bool asignarInstanciasAClustersCercanos();

    


    /////////////////////
    //  BÚSQUEDA LOCAL
    /////////////////////
    /*
        USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL EL PRIMERO MEJOR

        Asigna clústers a las instancia de forma aleatoria respetando
        las restricciones fuertes del problema.

        Devuelve true si ha sido exitosa la operación. 
        En otro caso devuelve false.
    */
    bool crearSolucionAleatoria();

    /*
        USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL EL PRIMERO MEJOR

        Nos devuelve verdadero si cambia el cluster. Solo hace el cambio si mejora
        la solución al problema.
    */
    bool cambioClusterMejor(int instancia, int cluster);

    /*
        USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL EL PRIMERO MEJOR

        Nos devuelve True en caso de haber encontrado un vecino mejor. En otro caso
        estamos ante la solución óptima y nos dará False
    */
    bool buscarPrimerVecinoMejor();


    /*
        USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL SUAVE Y MEMÉTICO 

        Actúa sobre la solución pasada por parámetro. Se permiten #fallos en el algoritmo

        Nos devuelve True en caso de haber encontrado un vecino mejor. En otro caso
        estamos ante la solución óptima y nos dará False
    */
    bool busquedaLocalSuave(vector<double> &solucion, int fallos);

    


    ////////////////////////////
    //  GENÉTICOS Y MEMÉTICOS
    ////////////////////////////
    /*
        USADO PARA LA METAHEURÍSTICA GENÉTICA

        Crea un conjunto de TAM_POBLACION soluciones y se guardan en poblacion

        Devuelve true si ha sido exitosa la operación. 
        En otro caso devuelve false.
    */
    bool crearPoblacionAleatoria();

    /*
        USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

        Lleva a cabo el operador de cruce uniforme. Se pasan por referencia los dos padres
        y el vector resultante.

        Devuelve true si ha sido exitosa la operación.
        En otro caso devuelve false.
    */
    bool operadorCruceUniforme(vector<double> &padre1, vector<double> &padre2, vector<double> &hijo);

    /*
        USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

        Lleva a cabo el operador de cruce por segmento fijo. Se pasan por referencia los dos
        padres y el vector resultante.

        Devuelve true si ha sido exitosa la operación.
        En otro caso devuelve false.
    */
    bool operadorCruceSegmentoFijo(vector<double> &padre1, vector<double> &padre2, vector<double> &hijo);

    /*
        USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

        Lleva a cabo el operador de mutación. Se pasan por referencia la población.
        Además se pasará el número de genes a modificar.

        Devuelve true si ha sido exitosa la operación.
        En otro caso devuelve false.
    */
    bool operadorMutacion(vector<vector<double>> &pob, double ngenes);

    /*
        USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

        Se contemplan las metaheurísticas genéticas tanto estacionarias como generacional
        y los operadores de cruce 'uniforme' y 'segmento fijo'. Ejecuta una época completa
        sobre la población

        params:
            -tipo:      entero que determina si el algoritmo es estacionario(1) o generacional(2)
            -cruce:     entero que determina el tipo de cruce: 'uniforme'(1) o 'segmento fijo'(2)   
            -bls:       entero que determina que cada #bls épocas se haga una BúsquedaLocalSuave
                        si es 0 se ejecutará una época conforme a un algoritmo genético. Si es >0
                        se ejecutará una época conforme a un algoritmo memético.
            -prob:      probabilidad de que se modifique un cromosoma (bls>0)
            -best:      booleano: se cogen solo los prob*tam mejores si true (bls>0 and prob!=1)
        Nos devuelve True si se ha realizado con éxito el proceso de búsqueda de solución.
        Nos devuelve False en caso contrario.
    */
    bool runEpoch(int tipo, int cruce, int bls, double prob, bool best);


    /*
        USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

        Hace como función de mostrar los resultados de la evolución genética de cualquiera de las
        metaheurísticas genéticas. Se coge la mejor solución de la población y la establece como
        solución al problema representado por la clase.

        RECOMENDABLE: Ejecutar antes varias épocas con 'runGenetico'. Si no se ha ejecutado ninguna
        se ejecutará por defecto una época de tipo estacionario y segmento uniforme.

        Nos devuelve True si se ha realizado con éxito el proceso de búsqueda de solución.
        Nos devuelve False en caso contrario.
    */
    bool finishEpochs();
    

    
    /////////////////////////////////////////////////////////////
    //
    //    Métodos que nos dicen cómo de buena es la solución
    //
    /////////////////////////////////////////////////////////////

    /*
        Calcula la distancia media intra-cluster de un cluster[ci]
    */
    double distanciaIntraCluster(int ci);

    /*
        Calcula la desviación general de la partición C={c1​, c2​,...,cn} definida
        como la media de las desviaciones intra-cluster de cada grupo o cluster
    */
    double desviacionParticion();

    /*
        Calcula el número de restricciones violadas
    */
    int infeasibility();

    /*
        Función objetivo. Será la función que buscaremos minimizar y nos dirá 
        cómo de buena es nuestra solución actual
    */
    double  fitnessFunction();


    //////////////////////////////////
    //    Mostrar Info por Pantalla
    //////////////////////////////////
    /*
        Nos muestra el estado del problema actual para cada clúster
            -Nº de instancias que pertenecen al clúster
            -Centroide
            -Distancia intra clúster
        Además del problema general como es:
            -Infeasibility
            -Distancia media intra clúster del problema
            -Función objetivo
    */
    void mostrarEstado();

    /*
        Nos muestra un punto con sus coordenadas
    */
    void mostrarPunto(vector<double> p);
    



    //////////////////////////////////
    //    Setters and Getters
    //////////////////////////////////


    double getGreaterDist() const{
        return GREATER_DIST;
    }
    void setGreaterDist(int dist){
        GREATER_DIST = dist;
    }

    double getSeed() const{
        return SEED;
    }
    void setSeed(int seed){
        SEED = seed;
    }

    int getNumInstancias() const{
        return num_instancias;
    }
    void setNumInstancias(int ninstancias){
        num_instancias = ninstancias;

        // Asignamos ningún clúster a todos
        if(!inst_belong.empty()){
            inst_belong.clear();
        }
        for(int i=0; i<num_instancias; i++){
            inst_belong.push_back(-1);
        }

    }
    
    int getNumAtributos() const{
        return num_atributos;
    }
    void setNumAtributos(int natributos){
        num_atributos = natributos;
        if(num_clases>0){       // Podemos inicializar los centroides
            centroides.clear();
            centroides.resize(num_clases);
            for(int c=0; c<num_clases; c++){
                centroides[c].resize(num_atributos);
            }
            necesidad_actualizar_centroides=true;
        }
    }

    int getNumClases() const{
        return num_clases;
    }
    void setNumClases(int nclases){
        num_clases = nclases;

        clusters.clear();
        clusters.resize(num_clases);

        if(num_atributos>0){       // Podemos inicializar los centroides
            centroides.clear();
            centroides.resize(num_clases);
            for(int c=0; c<num_clases; c++){
                centroides[c].resize(num_atributos);
            }
            necesidad_actualizar_centroides=true;
        }
    }


    vector<vector<double>> getInstancias() const{
        return instancias;
    }
    void setInstancias(vector<vector<double>> new_instancias){
        // Inicializamos todas las variables que dependen de las instancias
        instancias = new_instancias;
        num_instancias = new_instancias.size();

        if(!inst_belong.empty()){
            inst_belong.clear();
        }
        for(int i=0; i<num_instancias; i++){
           inst_belong.push_back(-1);
        }

        ShuffleIndices(num_instancias);

        // Calculamos la distancia más grande
        GREATER_DIST = 0;
        for(int i=0; i<instancias.size(); i++){
            for(int j=i+1; j<instancias.size(); j++){
                double dist_actual = distanciaEntreDosPuntos(instancias[i], instancias[j]);
                if(dist_actual > GREATER_DIST){
                    GREATER_DIST = dist_actual;
                }
            }
        }

    }

    vector<vector<double>> getCentroides() const{
        return centroides;
    }; 
    void setCentroides(vector<vector<double>> new_centroides){
        bool correct_size=true;
        if(new_centroides.size() == num_clases){
            for(int c =0; c<num_clases; c++){
                if(new_centroides[c].size() != num_atributos){
                    correct_size = false;
                    cout << "ERROR. Tamaño de los centroides erróneo. El número de atributos de los centroides no coincide con el de las instancias." << endl;
                }
            }
            if(correct_size){
                centroides = new_centroides;
            }
        }else{
            cout << "ERROR. Tamaño de los centroides erróneo. Hay distinto número de centroides que de clases." << endl;
        }
    }

    vector<int> getInstBelong() const{
        return inst_belong;
    }
    void getInstBelong(vector<int> new_inst_belong){
        if(new_inst_belong.size()==num_instancias){
            inst_belong = new_inst_belong;
        }else{
            cout << "ERROR. Inst_belong asignado de distinto tamaño con respecto el número de instancias." << endl;
        }
    }
    
    vector<vector<int>> getClusters() const{
        return clusters;
    }
    void setClusters(vector<vector<int>> new_clusters){
        if(new_clusters.size() == num_clases){
            clusters = new_clusters;
        }else{
            cout << "ERROR. Se ha intentado asignar un número de clústers distintos al número de clases real." << endl;
        }
    }
    
    vector<vector<double>> getRestricciones() const{
        return restricciones;
    }
    void setRestricciones(vector<vector<double>> new_restricciones){
        restricciones = new_restricciones;
        generarRestricciones();
    }

    vector<int> getIndices() const{
        return indices;
    }
    void setIndices(vector<int> ind){
        if(ind.size() == num_instancias){
            indices = ind;
        }else{
            cout << "ERROR. Los índices que se han intentado asignar no contiene el mismo número de índices que instancias el problema." << endl;
        }
    }

    int getIterationsFF() const{
        return iterations_ff;
    }
    void setIterationsFF(int it){
        iterations_ff = it;
    }

    int getTamPoblacion() const{
        return TAM_POBLACION;
    }
    void setTamPoblacion(int tam){
        TAM_POBLACION = tam;
    }

    int getEpoca() const{
        return epoca;
    }
    void setEpoca(int t){
        epoca = t;
    }

    vector<vector<double>> getPoblacion() const{
        return poblacion;
    }
    void setPoblacion(vector<vector<double>> pob){
        poblacion = pob;
    }

    double getFuncionObjetivo() const{
        return funcion_objetivo;
    }
    void setFuncionObjetivo(double fo){
        funcion_objetivo=fo;
    }


};
    
#endif