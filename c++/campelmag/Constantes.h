#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <iostream>

using namespace std;

//Tensão da Linha (V)
constexpr double V {765e3};

//Potência Transmitida (W)
constexpr double P {2.2e9};
constexpr double Ir {P/V};

//Angulos das fases (graus)
const vector<double> AngulosABC {120,0,-120};

//Corrente com a defasagem
//Itemp = Iri(1120,AngulosABC);
//IA = complex(Itemp[1,1],Itemp[1,2]);
//IB = complex(Itemp[2,1],Itemp[2,2]);
//IC = complex(Itemp[3,1],Itemp[3,2]);

//I = [IA IB IC];

//Caracteristicas Mecânicas
const vector<double> LinhaDeMedicao {-80.0, 80.0, 0.5, 1.5}; //[Xmin Xmax Passo Altura]
constexpr double Hmax {25.0}; //#[m]
constexpr double FLECHA {8.63}; //#[m]
constexpr double EspacSubCond {0.457}; // # [m]
constexpr double Dcabo {0.02959};
constexpr double Ncond {4.0};
constexpr double h0 {Hmax-FLECHA-EspacSubCond/2.0};
const vector<double> H {h0, h0, h0}; //#Altura mínima do cabo no feixe
const vector<double> D {-14.34, 0.0, 14.34};



#endif