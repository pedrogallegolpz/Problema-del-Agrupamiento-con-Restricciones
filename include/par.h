/*  -------------------------------------------------
        FUNCIONES AUXILIARES DE LOS CLUSTERS 
    -------------------------------------------------   */

#ifndef __PAR_H
#define __PAR_H

#include <vector>

using namespace std;

class PAR{
private:
    int num_instancias;
    int num_atributos;
    int num_clases;

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

public:
    /*
        Pone el num_clases a 0, por lo tanto no hay nada que hacer.
    */
    PAR();

    /*
        Inicializa el número de clases y los datos del problema.
    */
    PAR(int num_instancias_source, int num_atributos_source, int num_clases_source, vector<vector<double>> instancias_source);

    /*
        Constructor copia
    */
    PAR(const PAR &par_source);    

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
        Actualiza los centroides
    */
    bool updateCentroides();
    
    
    /*
        Asigna cada instancia a su clúster más cercano teniendo que respetar
        las restricciones fuertes y teniendo en cuenta de que cada instancia
        tiene que escoger el clúster con el que incumpla menos restricciones
        débiles.

        Devuelve un bool que nos dice si el método ha provocado algún cambio
        en los centroides.
    */
    bool asignarInstanciasAClustersCercanos();



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
    */
    void mostrarEstado();

    /*
        Nos muestra un punto con sus coordenadas
    */
    void mostrarPunto(vector<double> p);
    



    //////////////////////////////////
    //    Setters and Getters
    //////////////////////////////////

    int getNumInstancias() const{
        return num_instancias;
    }
    void setNumInstancias(int ninstancias){
        num_instancias = ninstancias;

        // Asignamos ningún clúster a todos
        inst_belong.clear();
        for(int i=0; i<num_instancias; i++){
            inst_belong.push_back(-1);
        }
    }
    
    int getNumAtributos() const{
        return num_atributos;
    }
    void setNumAtributos(int natributos){
        num_atributos = natributos;
    }

    int getNumClases() const{
        return num_clases;
    }
    void setNumClases(int nclases){
        num_clases = nclases;
        centroides.resize(num_clases);
        clusters.resize(num_clases);
    }

    vector<vector<double>> getInstancias() const{
        return instancias;
    }
    void setInstancias(vector<vector<double>> new_instancias){
        instancias = new_instancias;
    }

    vector<vector<double>> getCentroides() const{
        return centroides;
    }; 
    void setCentroides(vector<vector<double>> new_centroides){
        centroides = new_centroides;
    }

    vector<int> getInstBelong() const{
        return inst_belong;
    }
    void getInstBelong(vector<int> new_inst_belong){
        inst_belong = new_inst_belong;
    }
    
    vector<vector<int>> getClusters() const{
        return clusters;
    }
    void setClusters(vector<vector<int>> new_clusters){
        clusters = new_clusters;
    }
    
    vector<vector<double>> getRestricciones() const{
        return restricciones;
    }
    void setRestricciones(vector<vector<double>> new_restricciones){
        restricciones = new_restricciones;
    }

    vector<int> getIndices() const{
        return indices;
    }
    void setIndices(vector<int> ind){
        indices = ind;
    }
    


};
    
#endif