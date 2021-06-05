/******************************************************
 * 
 *      @author: Pedro Gallego López
 *      @subject: Metaheurísticas
 *      @university: Universidad de Granada
 *      @date: 2021
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
#include "totex.h"
#include "greedyCOPKM.h"
#include "busqlocal.h"
#include "genetico.h"
#include "memetico.h"
#include "enfriamientosimulado.h"
#include "busqmulbas.h"
#include "busqlocreit.h"
#include "busqlocreitES.h"
#include <sstream>

using namespace std;




int main(int argc, char * argv[]) {

    

    if(argc < 2){
        cout << "Uso: " << argv[0] << " [./data/<filename> | --tex] [semilla]?" << endl << "No hay que poner el .dat en el archivo por parámetro." << endl << endl;
        return 1;
    }

    string filestring(argv[1]);

    if(filestring!="./data/bupa_set" && filestring!="./data/zoo_set" && filestring!="./data/glass_set" && filestring!="./data/prueba" && filestring!="--tex"){
        cout << "No existe ese archivo. Por favor introduzca uno de los siguientes:" << endl;
        cout << " - ./data/bupa_set" << endl << " - ./data/zoo_set" << endl << " - ./data/glass_set" << endl << " - --tex" << endl << endl;

        return 1;
    }


    // Establecemos el número de clases en función del archivo
    vector<int> clases;
    vector<string> files;
    
    if(filestring=="./data/zoo_set" || filestring=="--tex"){
        clases.push_back(7);
        files.push_back("./data/zoo_set");      
    }
    if(filestring=="./data/glass_set" || filestring=="--tex"){
        clases.push_back(7);
        files.push_back("./data/glass_set");      
    }
    if(filestring=="./data/bupa_set" || filestring=="--tex"){
        clases.push_back(16);
        files.push_back("./data/bupa_set");
    }
    if(filestring=="./data/prueba"){
        clases.push_back(4);  
        files.push_back("./data/prueba_set");            
    }

    bool hacer_tabla_tex = false;
    if(filestring=="--tex"){
        hacer_tabla_tex = true;
    }

    int seed=0;
    if(argc==3){
        stringstream ss;
        string aux(argv[2]);
        ss << aux;
        ss >> seed;
    }

    

    const int NUM_PRUEBAS = 5;
    const int TAM_POBLACION_GENETICO = 50;
    const int TAM_POBLACION_MEMETICO = 50; 


    // booleanos para ver qué algoritmos ejecutar
    bool greedy_run = true;
    bool bl_run = true;

    bool geneticos_run = false;
    bool memeticos_run = false;

    bool trayectorias_run = true;
    


    // Variables donde guardamos la salida
    // Vector de tablas
    vector<vector<vector<double>>> tablas;

    // Tablas
    vector<vector<double>> medias10_tab;
    vector<vector<double>> medias20_tab;

    vector<vector<double>> greedy10_tab;
    vector<vector<double>> greedy20_tab;

    vector<vector<double>> bl10_tab;
    vector<vector<double>> bl20_tab;

    vector<vector<double>> aggun10_tab;
    vector<vector<double>> aggun20_tab;
    vector<vector<double>> aggsf10_tab;
    vector<vector<double>> aggsf20_tab;

    vector<vector<double>> ageun10_tab;
    vector<vector<double>> ageun20_tab;
    vector<vector<double>> agesf10_tab;
    vector<vector<double>> agesf20_tab;

    vector<vector<double>> mem1010_tab;
    vector<vector<double>> mem1020_tab;
    vector<vector<double>> mem0110_tab;
    vector<vector<double>> mem0120_tab;
    vector<vector<double>> mem01best10_tab;
    vector<vector<double>> mem01best20_tab;


    vector<vector<double>> es10_tab;
    vector<vector<double>> es20_tab;

    vector<vector<double>> bmb10_tab;
    vector<vector<double>> bmb20_tab;

    vector<vector<double>> ils10_tab;
    vector<vector<double>> ils20_tab;
    vector<vector<double>> ilses10_tab;
    vector<vector<double>> ilses20_tab;

        



    // Semillas
    vector<int> semillas;




    for(int i=0; i<NUM_PRUEBAS; i++){
        // SEMILLA
        seed++;
        
        // Guardamos la semilla
        semillas.push_back(seed);

        vector<double> aux_greedy10_tab;
        vector<double> aux_greedy20_tab;

        vector<double> aux_bl10_tab;
        vector<double> aux_bl20_tab;

        vector<double> aux_aggun10_tab;
        vector<double> aux_aggun20_tab;
        vector<double> aux_aggsf10_tab;
        vector<double> aux_aggsf20_tab;

        vector<double> aux_ageun10_tab;
        vector<double> aux_ageun20_tab;
        vector<double> aux_agesf10_tab;
        vector<double> aux_agesf20_tab;

        vector<double> aux_mem1010_tab;
        vector<double> aux_mem1020_tab;
        vector<double> aux_mem0110_tab;
        vector<double> aux_mem0120_tab;
        vector<double> aux_mem01best10_tab;
        vector<double> aux_mem01best20_tab;


        vector<double> aux_es10_tab;
        vector<double> aux_es20_tab;

        vector<double> aux_bmb10_tab;
        vector<double> aux_bmb20_tab;

        vector<double> aux_ils10_tab;
        vector<double> aux_ils20_tab;
        vector<double> aux_ilses10_tab;
        vector<double> aux_ilses20_tab;


        for(int f=0; f<files.size(); f++){
            // Estructuras para la lectura
            vector<vector<double> > data;
            vector<vector<double> > const_10;
            vector<vector<double> > const_20;

            // Leemos los ficheros
            read_dat(files[f],data);//leemos el fichero.dat
            read_const(files[f],const_10,const_20);//leemos ficheros.const

            int time;

            //PAR(int num_atributos_source, int num_clases_source, vector<vector<double>> instancias_source, vector<vector<double>> restricciones_source, int seed_source, int tam_pob);
            PAR par_10 = PAR(data[0].size(), clases[f], data, const_10, seed, TAM_POBLACION_GENETICO);
            PAR par_20 = PAR(data[0].size(), clases[f], data, const_20, seed, TAM_POBLACION_GENETICO);
            

            cout << "\nSEMILLA "<< seed << "\t\t\t\t\t\tTime \tFitFunc \tInfeas \tDesviacion" << endl;

            
            ////////////////////////////////////
            //          GREEDY
            ////////////////////////////////////
            if(greedy_run){
                cout << files[f] << "_CONST_10. ";
                time = greedyCOPKM(par_10, seed,false);
                aux_greedy10_tab.push_back(par_10.infeasibility());
                aux_greedy10_tab.push_back(par_10.desviacionParticion());
                aux_greedy10_tab.push_back(par_10.fitnessFunction());
                aux_greedy10_tab.push_back(time);

            
                cout << files[f] << "_CONST_20. ";
                time = greedyCOPKM(par_20, seed,false);
                aux_greedy20_tab.push_back(par_20.infeasibility());
                aux_greedy20_tab.push_back(par_20.desviacionParticion());
                aux_greedy20_tab.push_back(par_20.fitnessFunction());
                aux_greedy20_tab.push_back(time);
            }

            ////////////////////////////////////
            //       BÚSQUEDA LOCAL
            ////////////////////////////////////
            if(bl_run){
                cout << files[f] << "_CONST_10. ";
                time = busquedaLocalPAR(par_10, seed, false);
                aux_bl10_tab.push_back(par_10.infeasibility());
                aux_bl10_tab.push_back(par_10.desviacionParticion());
                aux_bl10_tab.push_back(par_10.fitnessFunction());
                aux_bl10_tab.push_back(time);

                cout << files[f] << "_CONST_20. ";
                time = busquedaLocalPAR(par_20, seed, false);
                aux_bl20_tab.push_back(par_20.infeasibility());
                aux_bl20_tab.push_back(par_20.desviacionParticion());
                aux_bl20_tab.push_back(par_20.fitnessFunction());
                aux_bl20_tab.push_back(time);
            }


            ////////////////////////////////////
            //          GENÉTICO
            ////////////////////////////////////
            if(geneticos_run){
                cout << files[f] << "_CONST_10. ";
                time = genetico(par_10, TAM_POBLACION_GENETICO, 1, 1, seed, false,false);
                aux_ageun10_tab.push_back(par_10.infeasibility());
                aux_ageun10_tab.push_back(par_10.desviacionParticion());
                aux_ageun10_tab.push_back(par_10.fitnessFunction());
                aux_ageun10_tab.push_back(time);
                
                cout << files[f] << "_CONST_20. ";
                time = genetico(par_20, TAM_POBLACION_GENETICO, 1, 1, seed, false,false);
                aux_ageun20_tab.push_back(par_20.infeasibility());
                aux_ageun20_tab.push_back(par_20.desviacionParticion());
                aux_ageun20_tab.push_back(par_20.fitnessFunction());
                aux_ageun20_tab.push_back(time);
                

                cout << files[f] << "_CONST_10. ";
                time = genetico(par_10, TAM_POBLACION_GENETICO, 1, 2, seed, false,false);
                aux_agesf10_tab.push_back(par_10.infeasibility());
                aux_agesf10_tab.push_back(par_10.desviacionParticion());
                aux_agesf10_tab.push_back(par_10.fitnessFunction());
                aux_agesf10_tab.push_back(time);
                
                cout << files[f] << "_CONST_20. ";
                time = genetico(par_20, TAM_POBLACION_GENETICO, 1, 2, seed, false,false);
                aux_agesf20_tab.push_back(par_20.infeasibility());
                aux_agesf20_tab.push_back(par_20.desviacionParticion());
                aux_agesf20_tab.push_back(par_20.fitnessFunction());
                aux_agesf20_tab.push_back(time);


                cout << files[f] << "_CONST_10. ";
                time = genetico(par_10, TAM_POBLACION_GENETICO, 2, 1, seed, false,false);
                aux_aggun10_tab.push_back(par_10.infeasibility());
                aux_aggun10_tab.push_back(par_10.desviacionParticion());
                aux_aggun10_tab.push_back(par_10.fitnessFunction());
                aux_aggun10_tab.push_back(time);
                
                cout << files[f] << "_CONST_20. ";
                time = genetico(par_20, TAM_POBLACION_GENETICO, 2, 1, seed, false,false);
                aux_aggun20_tab.push_back(par_20.infeasibility());
                aux_aggun20_tab.push_back(par_20.desviacionParticion());
                aux_aggun20_tab.push_back(par_20.fitnessFunction());
                aux_aggun20_tab.push_back(time);


                cout << files[f] << "_CONST_10. ";
                time = genetico(par_10, TAM_POBLACION_GENETICO, 2, 2, seed, false,false);
                aux_aggsf10_tab.push_back(par_10.infeasibility());
                aux_aggsf10_tab.push_back(par_10.desviacionParticion());
                aux_aggsf10_tab.push_back(par_10.fitnessFunction());
                aux_aggsf10_tab.push_back(time);
                
                
                cout << files[f] << "_CONST_20. ";
                time = genetico(par_20, TAM_POBLACION_GENETICO, 2, 2, seed, false,false);
                aux_aggsf20_tab.push_back(par_20.infeasibility());
                aux_aggsf20_tab.push_back(par_20.desviacionParticion());
                aux_aggsf20_tab.push_back(par_20.fitnessFunction());
                aux_aggsf20_tab.push_back(time);
            }

            ////////////////////////////////////
            //          MEMÉTICO
            ////////////////////////////////////
            //memetico(PAR par, int tam, int bls, double prob, bool best, int cruce, int seed, bool mostrarEstado);
            if(memeticos_run){
                cout << endl << files[f] << "_CONST_10. ";
                time = memetico(par_10, TAM_POBLACION_MEMETICO, 10, 1.0, false, 2, seed, false,false);
                aux_mem1010_tab.push_back(par_10.infeasibility());
                aux_mem1010_tab.push_back(par_10.desviacionParticion());
                aux_mem1010_tab.push_back(par_10.fitnessFunction());
                aux_mem1010_tab.push_back(time);
                
                cout << files[f] << "_CONST_20. ";
                time = memetico(par_20, TAM_POBLACION_MEMETICO, 10, 1.0, false, 2, seed, false,false);
                aux_mem1020_tab.push_back(par_20.infeasibility());
                aux_mem1020_tab.push_back(par_20.desviacionParticion());
                aux_mem1020_tab.push_back(par_20.fitnessFunction());
                aux_mem1020_tab.push_back(time);


                cout << endl << files[f] << "_CONST_10. ";
                time = memetico(par_10, TAM_POBLACION_MEMETICO, 10, 0.1, false, 2, seed, false,false);
                aux_mem0110_tab.push_back(par_10.infeasibility());
                aux_mem0110_tab.push_back(par_10.desviacionParticion());
                aux_mem0110_tab.push_back(par_10.fitnessFunction());
                aux_mem0110_tab.push_back(time);
            
                cout << files[f] << "_CONST_20. ";
                time = memetico(par_20, TAM_POBLACION_MEMETICO, 10, 0.1, false, 2, seed, false,false);  
                aux_mem0120_tab.push_back(par_20.infeasibility());
                aux_mem0120_tab.push_back(par_20.desviacionParticion());
                aux_mem0120_tab.push_back(par_20.fitnessFunction());
                aux_mem0120_tab.push_back(time);          


                cout << endl << files[f] << "_CONST_10. ";
                time = memetico(par_10, TAM_POBLACION_MEMETICO, 10, 0.1, true, 2, seed, false,false); 
                aux_mem01best10_tab.push_back(par_10.infeasibility());
                aux_mem01best10_tab.push_back(par_10.desviacionParticion());
                aux_mem01best10_tab.push_back(par_10.fitnessFunction());
                aux_mem01best10_tab.push_back(time);           
                
                cout << files[f] << "_CONST_20. ";
                time = memetico(par_20, TAM_POBLACION_MEMETICO, 10, 0.1, true, 2, seed, false,false);    
                aux_mem01best20_tab.push_back(par_20.infeasibility());
                aux_mem01best20_tab.push_back(par_20.desviacionParticion());
                aux_mem01best20_tab.push_back(par_20.fitnessFunction());
                aux_mem01best20_tab.push_back(time);  
            }

            ////////////////////////////////////
            //          TRAYECTORIAS
            ////////////////////////////////////
            if(trayectorias_run){
                cout << endl << files[f] << "_CONST_10. ";
                time = enfriamientoSimulado(par_10, seed, false, false);
                aux_es10_tab.push_back(par_10.infeasibility());
                aux_es10_tab.push_back(par_10.desviacionParticion());
                aux_es10_tab.push_back(par_10.fitnessFunction());
                aux_es10_tab.push_back(time);
            
                cout << files[f] << "_CONST_20. ";
                time = enfriamientoSimulado(par_20, seed, false, false);
                aux_es20_tab.push_back(par_20.infeasibility());
                aux_es20_tab.push_back(par_20.desviacionParticion());
                aux_es20_tab.push_back(par_20.fitnessFunction());
                aux_es20_tab.push_back(time); 


                cout << endl << files[f] << "_CONST_10. ";
                time = busquedaMultiArranque(par_10, seed, false, false);
                aux_bmb10_tab.push_back(par_10.infeasibility());
                aux_bmb10_tab.push_back(par_10.desviacionParticion());
                aux_bmb10_tab.push_back(par_10.fitnessFunction());
                aux_bmb10_tab.push_back(time);
            
                cout << files[f] << "_CONST_20. ";
                time = busquedaMultiArranque(par_20, seed, false, false);
                aux_bmb20_tab.push_back(par_20.infeasibility());
                aux_bmb20_tab.push_back(par_20.desviacionParticion());
                aux_bmb20_tab.push_back(par_20.fitnessFunction());
                aux_bmb20_tab.push_back(time); 


                cout << endl << files[f] << "_CONST_10. ";
                time = busquedaLocalReiterada(par_10, seed, false, false);
                aux_ils10_tab.push_back(par_10.infeasibility());
                aux_ils10_tab.push_back(par_10.desviacionParticion());
                aux_ils10_tab.push_back(par_10.fitnessFunction());
                aux_ils10_tab.push_back(time);
            
                cout << files[f] << "_CONST_20. ";
                time = busquedaLocalReiterada(par_20, seed, false, false);
                aux_ils20_tab.push_back(par_20.infeasibility());
                aux_ils20_tab.push_back(par_20.desviacionParticion());
                aux_ils20_tab.push_back(par_20.fitnessFunction());
                aux_ils20_tab.push_back(time); 

                cout << endl << files[f] << "_CONST_10. ";
                time = busquedaLocalReiteradaES(par_10, seed, false, false);
                aux_ilses10_tab.push_back(par_10.infeasibility());
                aux_ilses10_tab.push_back(par_10.desviacionParticion());
                aux_ilses10_tab.push_back(par_10.fitnessFunction());
                aux_ilses10_tab.push_back(time);
            
                cout << files[f] << "_CONST_20. ";
                time = busquedaLocalReiteradaES(par_20, seed, false, false);
                aux_ilses20_tab.push_back(par_20.infeasibility());
                aux_ilses20_tab.push_back(par_20.desviacionParticion());
                aux_ilses20_tab.push_back(par_20.fitnessFunction());
                aux_ilses20_tab.push_back(time); 
            }     
        }

        // Añadimos filas a las tablas

        if(greedy_run){
            greedy10_tab.push_back(aux_greedy10_tab);
            greedy20_tab.push_back(aux_greedy20_tab);
        }
        
        if(bl_run){
            bl10_tab.push_back(aux_bl10_tab);
            bl20_tab.push_back(aux_bl20_tab);
        }

        if(geneticos_run){
            aggun10_tab.push_back(aux_aggun10_tab);
            aggun20_tab.push_back(aux_aggun20_tab);
            aggsf10_tab.push_back(aux_aggsf10_tab);
            aggsf20_tab.push_back(aux_aggsf20_tab);

            ageun10_tab.push_back(aux_ageun10_tab);
            ageun20_tab.push_back(aux_ageun20_tab);
            agesf10_tab.push_back(aux_agesf10_tab);
            agesf20_tab.push_back(aux_agesf20_tab);
        }

        if(memeticos_run){
            mem1010_tab.push_back(aux_mem1010_tab);
            mem1020_tab.push_back(aux_mem1020_tab);
            mem0110_tab.push_back(aux_mem0110_tab);
            mem0120_tab.push_back(aux_mem0120_tab);
            mem01best10_tab.push_back(aux_mem01best10_tab);
            mem01best20_tab.push_back(aux_mem01best20_tab);
        }

        if(trayectorias_run){
            es10_tab.push_back(aux_es10_tab);
            es20_tab.push_back(aux_es20_tab);  

            bmb10_tab.push_back(aux_bmb10_tab);
            bmb20_tab.push_back(aux_bmb20_tab);    

            ils10_tab.push_back(aux_ils10_tab);
            ils20_tab.push_back(aux_ils20_tab); 
            ilses10_tab.push_back(aux_ilses10_tab);
            ilses20_tab.push_back(aux_ilses20_tab);            
        }


    }

    if(greedy_run){   
        tablas.push_back(greedy10_tab);
        tablas.push_back(greedy20_tab);
    }

    if(bl_run){
        tablas.push_back(bl10_tab);
        tablas.push_back(bl20_tab);
    }

    if(geneticos_run){
        tablas.push_back(aggun10_tab);
        tablas.push_back(aggun20_tab);
        tablas.push_back(aggsf10_tab);
        tablas.push_back(aggsf20_tab);

        tablas.push_back(ageun10_tab);
        tablas.push_back(ageun20_tab);
        tablas.push_back(agesf10_tab);
        tablas.push_back(agesf20_tab);
    }

    if(memeticos_run){
        tablas.push_back(mem1010_tab);
        tablas.push_back(mem1020_tab);

        tablas.push_back(mem0110_tab);
        tablas.push_back(mem0120_tab);
        
        tablas.push_back(mem01best10_tab);
        tablas.push_back(mem01best20_tab);
    }

    if(trayectorias_run){
        tablas.push_back(es10_tab);
        tablas.push_back(es20_tab);
        
        tablas.push_back(bmb10_tab);
        tablas.push_back(bmb20_tab);
        
        tablas.push_back(ils10_tab);
        tablas.push_back(ils20_tab);
        tablas.push_back(ilses10_tab);
        tablas.push_back(ilses20_tab);
    }


    // Calculamos medias
    for(int tab=0; tab<tablas.size(); tab++){
        if(tablas[tab].size()>0){
            vector<double> fila_medias;

            for(int j=0; j<tablas[tab][0].size();j++){
                double media=0;

                for(int i=0; i<tablas[tab].size(); i++){
                    media+=tablas[tab][i][j];
                }
                media/=tablas[tab].size();

                fila_medias.push_back(media);
            }
            if(tab%2==0){
                //10%
                medias10_tab.push_back(fila_medias);
            }else{
                medias20_tab.push_back(fila_medias);
            }
            tablas[tab].push_back(fila_medias);
        }
    }

    vector<string> tab_name;
    if(greedy_run){
        tab_name.push_back("Greedy 10%");
        tab_name.push_back("Greedy 20%");
    }

    if(bl_run){
        tab_name.push_back("BL 10%");
        tab_name.push_back("BL 20%");
    }

    if(geneticos_run){
        tab_name.push_back("AGGUN 10%");
        tab_name.push_back("AGGUN 20%");
        tab_name.push_back("AGGSF 10%");
        tab_name.push_back("AGGSF 20%");

        tab_name.push_back("AGEUN 10%");
        tab_name.push_back("AGEUN 20%");
        tab_name.push_back("AGESF 10%");
        tab_name.push_back("AGESF 20%");
    }

    if(memeticos_run){
        tab_name.push_back("MEM10 10%");
        tab_name.push_back("MEM10 20%");

        tab_name.push_back("MEM01 10%");
        tab_name.push_back("MEM01 20%");

        tab_name.push_back("MEM01BEST 10%");
        tab_name.push_back("MEM01BEST 20%");
    }

    if(trayectorias_run){
        tab_name.push_back("ES 10%");
        tab_name.push_back("ES 20%");

        tab_name.push_back("BMB 10%");
        tab_name.push_back("BMB 20%");


        tab_name.push_back("ILS 10%");
        tab_name.push_back("ILS 20%");
        tab_name.push_back("ILSES 10%");
        tab_name.push_back("ILSES 20%");
    }

    if(hacer_tabla_tex){
        cout << endl << endl << "-----------TABLAS-----------"<<endl;
        for(int tab=0; tab<tablas.size(); tab++){
            cout << endl << endl << tab_name[tab] << endl;
            toTableTex(tablas[tab]);
        }

        cout << endl << endl << "MEDIAS 10%" << endl;
        toTableTex(medias10_tab);

        cout << endl << endl << "MEDIAS 20%" << endl;
        toTableTex(medias20_tab);
    }
    
}
