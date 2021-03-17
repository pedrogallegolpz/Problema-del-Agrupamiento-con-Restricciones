/* -------------------------------------------------
                        READ FILES
   ------------------------------------------------- */


#ifndef __READFILES_H
#define __READFILES_H

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/*
    Asumimos que pasa el nombre del archivo y yo añado el .dat
    guardamos por referencia la estructura de Instancias leida
*/
void read_dat(const string& filename, vector<vector<double> >& data);

/*
    Asumimos que pasa el nombre del archivo y yo añado el _const_10.const y _const_20.const
    guardamos por referencia la estructura de Instancias leida
*/
void read_const(const string& filename, vector<vector<double> >& const_10, vector<vector<double> >& const_20);

#endif