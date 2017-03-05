
#include <iostream>
//#include <cmath>
#include <vector>
#include <complex>

#include <eigen3/Eigen/Dense>
#include <boost/math/constants/constants.hpp>

#include "FuncoesMag.h"

using namespace std;

// cahamada para complexo
typedef std::complex<double> cx;


constexpr double PI {boost::math::constants::pi<double>()};

/*
* Retorna um vetor com o campo magnético resultante nos pontos da faixa de passagem
* [SBr SBx SBh B] - Campo em mG (1 mili Gauss = 0.1 micro Tesla)
* Brms(I,H,D,P)
* I = Corrente que passa pela linha. [IA IB IC]
* H = Vetor com as posições verticais dos cabos. [A B C]
* D = Vetor com as posições horizontais relativas dos cabos [A 0 C]
* P = Vetor com as caracteristicas da linha de medição [-X X Passo Altura]
*/
Eigen::MatrixXcd* Brms(const Eigen::Vector3cd& I, 
											 const vector<double>& H, 
											 const vector<double>& D, 
											 const vector<double>& P){
	
	//Inicializa o ponteiro de saida
	Eigen::MatrixXcd* ptrBrms = new Eigen::MatrixXcd;
	
	//Inicializa o ponteiro Xm
	const vector<double>* ptrXm = new vector<double>;
	
	// Divisoes no eixo x -> collect(inicio, passo, fim);
	ptrXm = collect(P[1],P[3],P[2]);
	
	// Altura maxima
	const double& Hm {P[4]};
	
	//Ponteiros Matrizes complexas
	Eigen::MatrixXcd* ptrBkr = new Eigen::MatrixXcd(ptrXm->size(),3);
	Eigen::MatrixXcd* ptrBkx = new Eigen::MatrixXcd(ptrXm->size(),3);
  Eigen::MatrixXcd* ptrBkh = new Eigen::MatrixXcd(ptrXm->size(),3);
	
	for(unsigned short int j=0; j < 3; ++j){
      for(unsigned short int i=0; i < ptrXm->size(); ++i){
				
        (*ptrBkr)(i,j) = ((2e-7)*I[j])/sqrt( pow(cx(ptrXm->at(i)-D[j]),2) + pow(Hm-H[j],2) );
        (*ptrBkx)(i,j) = ((2e-7)*I[j]*(ptrXm->at(i)-D[j]))/sqrt( pow(cx(ptrXm->at(i)-D[j]), 2) + pow(Hm-H[j], 2) );
        (*ptrBkh)(i,j) = ((2e-7)*I[j]*(ptrXm->at(i)-H[j]))/sqrt( pow(cx(ptrXm->at(i)-D[j]), 2) + pow(Hm-H[j], 2) );

      }
    }
	
	
	delete ptrXm;
	delete ptrBkr;
	delete ptrBkx;
	delete ptrBkh;
	
	return ptrBrms;
}
											
											
											
											
// CalcEkv()

/*
* Calcula a tensão crítica de pico (Equação de Peek)
* CoronaVc(M,Delta, Dcond, DistCond) [kV]
*
* M = Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
* Delta = Coeficiente que depende da altura e altitude
* Dcond = Diâmetro do condutor [m]
* DistCond = Distância entre condutores [m]
*/
double CoronaVc(const double& M,
								const double& Delta,
								const double& Dcond,
								const double& DistCond){
									
	double A0 {0.0};
	double A1 {0.0};
											 
	A0 = 2.43*M*Delta*Dcond*1000; 
  A1 = log10((2*DistCond*1000)/(Dcond*1000));
	
	return A0*A1;										
}


/*
* Calcula o delta crítico
* CoronaVc(M,delta, Dcond, DistCond)
*
* H = Altitude da instalação [m]
* t = temperatura média anual [°C]
*/
double CoronaDeltaCrit(const double& H, const double& Tc){
	cout << PI << endl;
	return (0.386*(760-0.086*H))/(273+Tc);
}


/*
* Calcula as perdas devido ao Efeito Corona
* CoronaPerdas(Delta,F,Dcond, DistCond,V,Vc) [kW/km]
*
* Delta = Delta Crítico
* F = Frequência da rede [Hz]
* Dcond = Diâmetro do condutor [m]
* DistCond = Distância entre os condutores [m]
* V = Tensão da rede pico-a-pico [V]
* Vc = Tensão Crítica da linha [V]
*/
double CoronaPerdas(const double& Delta, 
									  const double& FHz,
										const double& Dcond,
										const double& DistCond,
										const double& Vpp,
										const double& Vc){
	
	double A0, A1, A2;
	
  A0 = (3.44/Delta)*FHz;
  A1 = sqrt( (Dcond*1000) / ( 2*(DistCond*1000) ) );
  A2 = (Vpp-Vc)*(Vpp-Vc)*0.001;

  return A0*A1*A2;
}


/*
* Calcula o campo elétrico crítico
* CoronaEc(M,Delta,Dcond) [kV/cm]
*
* M = Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
* Delta = Delta Crítico
* F = Frequência da rede [Hz]
* Dcond = Diâmetro do condutor [m]
*
*/
double CoronaEC(const double& M,
								const double& Delta,
								const double& Dcond){

	double A0, A1;
	
  A0 = M*(21.1e3)*(Delta);
  A1 = (1 + (sqrt(1/0.301)/(sqrt(Delta*Dcond*1000)) ));

  return A0*A1;
}


/*
* Calcula o diâmetro do feixe
* db(S,N)
* S = Espaçamento entre os condutores
* N = Número de condutores
*/
double db(const double& S, const double& N){
	return S/sin(PI/N);
}


/*
* Calcula o feixe equivalente
*   deq(db, D, N)
* db = Diâmetro do feixe
* D  = Diâmetro do cabo
* N  = Número de condutores
*/
double deq(const double& db0, const double& D, const double& N){
	return db0*( pow(N*D/db0,1.0/N) );
}


/*
* Retorna a profundidade da imagem do condutor para o campo Magnético
* [Delta] - Profundidade em metros
* ProfImg(R,P,F)
* R = Resistividade ρ do solo. [Ω]
* P = Permeabilidade μ do solo. [H/m] Típico = 4π 10^-7 H/m
* F = Frequência da rede [Hz]
*/
double ProfImg(const double& R, const double& P, const double& FHz){
	return sqrt(R/(PI*P*FHz));
}
