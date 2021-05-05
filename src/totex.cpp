#include <iostream>
#include <vector>
#include <math.h>  
#include "../include/readfiles.h"

/*
    Sacamos una tabla latex sin título, sin etiqueta y sin pie de tabla.
*/
void toTableTex(vector<vector<double>> &tabla){
    cout << endl << "\\begin{table}[htp]" << endl << "\\centering" << endl << "\\resizebox{16cm}{!} {" << endl << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|c|c|}"<< endl;
    cout << "\\hline" << endl << endl << "\\multicolumn{1}{|c|}{} & \\multicolumn{4}{|c|}{\\textbf{Zoo}} & \\multicolumn{4}{|c|}{\\textbf{Glass}} & \\multicolumn{4}{|c|}{\\textbf{Bupa}} \\\\ \\hline" << endl << endl;
    cout << "& \\textit{Tasa\\_Inf} & \\textit{Dist-Intra} & \\textit{Agr} & \\textit{T(ms)} & \\textit{Tasa\\_Inf} & \\textit{Dist-Intra} & \\textit{Agr} & \\textit{T(ms)} &\\textit{Tasa\\_Inf} & \\textit{Dist-Intra} & \\textit{Agr} & \\textit{T(ms)} \\\\ \\hline" << endl << endl;

    for(int i = 0; i<tabla.size(); i++){
        if(i!=tabla.size()-1){
            cout << "\\textbf{Ejecución "<< i+1<< "} & ";
            for(int j=0; j<tabla[i].size(); j++){
                if(j!=tabla[i].size()-1){
                    cout << tabla[i][j] << " & ";
                }else{
                    cout << tabla[i][j] << " \\\\ \\hline" << endl << endl;
                }
            } 
        }else{
             cout << "\\textbf{Media} & ";
            for(int j=0; j<tabla[i].size(); j++){
                if(j!=tabla[i].size()-1){
                    cout << tabla[i][j] << " & ";
                }else{
                    cout << tabla[i][j] << " \\\\ \\hline" << endl << endl;
                }
            } 
        }
    }

    cout << "\\end{tabular}\n}" << endl;
    cout << "\\caption{}" << endl;
    cout << "\\label{}" << endl;
    cout << "\\end{table}" << endl;
}


using namespace std;
