#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <iostream>
#include <complex>

using namespace std;

// chamada para complexo
typedef std::complex<double> cx;

//Tensão da Linha (V)
constexpr double V {765e3};

//Potência Transmitida (W)
constexpr double P {2.2e9};
constexpr double Ir {P/V};

//Angulos das fases (graus)
const Eigen::Vector3d AngulosABC {120,0,-120};

//Corrente com a defasagem
const Eigen::Vector3cd* Itemp {Iri(1120,AngulosABC)};
const Eigen::Vector3cd I {*Itemp};

//Caracteristicas Mecânicas
const vector<double> LinhaDeMedicao {-80.0, 80.0, 0.5, 1.5}; //[Xmin Xmax Passo Altura]
constexpr double M {0.87};
constexpr double Hmax {25.0}; //#[m]
constexpr double FLECHA {8.63}; //#[m]
constexpr double EspacSubCond {0.457}; // # [m]
constexpr double Dcabo {0.02959};
constexpr double Ncond {4.0};
constexpr double h0 {Hmax-FLECHA-EspacSubCond/2.0};
const Eigen::Vector3d H {h0, h0, h0}; //#Altura mínima do cabo no feixe
const Eigen::Vector3d D {-14.34, 0.0, 14.34};

//Passando por referencia, para facilitar a leitura
const double& minimo {((LinhaDeMedicao[1]-LinhaDeMedicao[0])/LinhaDeMedicao[2])+1};
const double& passo {LinhaDeMedicao[0]};
const double& maximo {LinhaDeMedicao[0]+LinhaDeMedicao[2]*((LinhaDeMedicao[1]-LinhaDeMedicao[0])/LinhaDeMedicao[2])};




#endif