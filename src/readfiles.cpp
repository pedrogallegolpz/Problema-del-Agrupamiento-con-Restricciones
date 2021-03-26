#include <iostream>
#include <vector>
#include <math.h>  
#include "../include/readfiles.h"


using namespace std;

/*
    Asumimos que pasa el nombre del archivo y yo añado el .dat
    guardamos por referencia la estructura de Instancias leida
*/
void read_dat(const string& filename, vector<vector<double> >& data){

    cout << "--------------------------------------------------------------------------" << endl;
    cout << "Reading: " << filename+".dat" << endl;
    cout << "--------------------------------------------------------------------------" << endl << endl;

    ifstream file(filename+".dat"); 
    string line;

    int i=0;
    while (getline(file, line)) {
        istringstream s(line);
        string field;

        data.push_back(vector<double>());

        while (getline(s, field, ',')) {
            if (s.peek() != '\n')
                data[i].push_back(stod(field));
        }

        i++;
    
    }

}

/*
    Asumimos que pasa el nombre del archivo y yo añado el _const_10.const y _const_20.const
    guardamos por referencia la estructura de Instancias leida
*/
void read_const(const string& filename, vector<vector<double> >& const_10, vector<vector<double> >& const_20){

    cout << "--------------------------------------------------------------------------" << endl;
    cout << "Reading: " << filename+"_const_10.const ," << filename+"_const_20.const" << endl;
    cout << "--------------------------------------------------------------------------" << endl << endl;

    ifstream file_10(filename+"_const_10.const"); 
    ifstream file_20(filename+"_const_20.const"); 
    string line;
    
    int i=0;
    while (getline(file_10, line)) {
        istringstream s(line);
        string field;

        const_10.push_back(vector<double>());

        while (getline(s, field, ',')) {
            if (s.peek() != '\n')
                const_10[i].push_back(stod(field));
        }

        i++;
    
    }

    i=0;
    while (getline(file_20, line)) {
        istringstream s(line);
        string field;

        const_20.push_back(vector<double>());

        while (getline(s, field, ',')) {
            if (s.peek() != '\n')
                const_20[i].push_back(stod(field));
        }

        i++;
    
    }

}