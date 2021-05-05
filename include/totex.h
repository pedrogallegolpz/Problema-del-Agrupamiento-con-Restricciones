/* -------------------------------------------------
                CONVERT INFO TO TEX
   ------------------------------------------------- */


#ifndef __TOTEX_H
#define __TOTEX_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

/*
    Sacamos una tabla latex sin título, sin etiqueta y sin pie de tabla.
*/
void toTableTex(vector<vector<double>> &tabla);

#endif