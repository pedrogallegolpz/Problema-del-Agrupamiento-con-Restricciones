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
    vector<vector<double>> instancias;
    vector<vector<double>> centroides;  // Contiene los $num_clases centroides. Uno de cada cluster
    vector<vector<vector<double>>> clusters;
    
    //Restricciones
    vector<vector<double>> restricciones;
    vector<vector<int>> ML;             // Restricciones Must-Link (ML). La instancia ML[i,0] tiene que tener relación con ML[i,1]
    vector<vector<int>> CL;             // Restricciones Must-Link (ML). La instancia ML[i,0] no puede tener relación con ML[i,1]
                                        // NOTA en la posición M[i] hay dos índices que hacen referencia directa a instancias (No a indices)

    // Seguiremos el orden según estos índices
    vector<int> indices;                // Nos dice cómo recorremos las instancias

    /*
        Creamos los vectores ML y CL
    */
    void generarRestricciones();

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
        Función para calcular el centroide de un conjunto ci
    */
    vector<double> calcularCentroide(vector<vector<double>> ci);


    /*
        Calcula la distancia media intra-cluster de un cluster ci
    */
    double distanciaIntraCluster(vector<vector<double>> ci);

    /*
        Calcula la desviación general de la partición C={c1​, c2​,...,cn} definida
        como la media de las desviaciones intra-cluster de cada grupo o cluster
    */
    double desviacionParticiom(vector<vector<vector<double>>> C);

    /*
        Definimos ℎ𝐶(∙) como la función que dada una instancia xi
        devuelve la etiqueta j asociada al cluster cj al que xi pertenece
        según la partición C. getClusterFromInstance hace esta función
    */ 
    int getClusterFromInstance(int i);

    /*
        Calcula el número de restricciones violadas
    */
    int infeasibility();

    /*
        Asigna instancia al cluster más cercano y que cumpla las restricciones
    */
    void asignarInstanciasAClustersCercanos();




    //////////////////////////////////
    //    Setters and Getters
    //////////////////////////////////

    int getNumInstancias() const{
        return num_instancias;
    }
    void setNumInstancias(int ninstancias){
        num_instancias = ninstancias;
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

    vector<vector<vector<double>>> getClusters() const{
        return clusters;
    }
    void setClusters(vector<vector<vector<double>>> new_clusters){
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