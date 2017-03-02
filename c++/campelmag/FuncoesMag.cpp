
#include <iostream>
#include <cmath>
#include "FuncoesMag.h"

using namespace std;
// Brms()
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