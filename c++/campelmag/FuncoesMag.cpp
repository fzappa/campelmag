#include <iostream>
#include <vector>
#include <complex>

#define EIGEN_USE_MKL_ALL
#include <eigen3/Eigen/Dense>

// Para definicoes de constantes do boost
#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/codata_constants.hpp>
#include <boost/math/constants/constants.hpp>

#include "FuncoesMag.hpp"
#include "ParserXML.hpp"

using namespace std;

// chamada para complexo
typedef std::complex<double> cx;

// Constantes do BOOST
// http://www.boost.org/doc/libs/1_60_0/doc/html/boost_units/Reference.html
// http://www.boost.org/doc/libs/1_46_0/libs/math/doc/sf_and_dist/html/math_toolkit/toolkit/internals1/constants.html
constexpr double PI {boost::math::constants::pi<double>()};
const double EPSILON {boost::units::si::constants::codata::epsilon_0 / boost::units::si::farad * boost::units::si::meter};



/**
* Retorna um vetor com o campo magnético resultante nos pontos da faixa de passagem
* [SBr SBx SBh B] - Campo em mG (1 mili Gauss = 0.1 micro Tesla)
* Brms(I,H,D,P)
* I = Corrente que passa pela linha. [IA IB IC]
* H = Vetor com as posições verticais dos cabos. [A B C]
* D = Vetor com as posições horizontais relativas dos cabos [A 0 C]
* P = Vetor com as caracteristicas da linha de medição [-X X Passo Altura]
*
* EPRI AC Transmission Line Reference Book
**/
Eigen::MatrixXcd* Brms(const conf& dados){

	// Reescrevendo para trabalhar com o conf
	const Eigen::Vector3d AngulosABC {dados.angabc[0],dados.angabc[1],dados.angabc[2]};
	const Eigen::Vector3cd* I {Iri(dados.corrente,AngulosABC)};

	const Eigen::Vector3d& H{dados.hmin,dados.hmin,dados.hmin};
	const Eigen::Vector3d& D{dados.pxfeixes[0],dados.pxfeixes[1],dados.pxfeixes[2]};
	const vector<double>& P{dados.linhamed[0], dados.linhamed[1], dados.linhamed[2], dados.linhamed[3]};

	//Passando por referencia, para facilitar a leitura
  const double& max {P[0]+P[2]*((P[1]-P[0])/P[2])};
  const double& min {((P[1]-P[0])/P[2])+1};
  const double& passo {P[0]};

	//Inicializa o ponteiro Xm
	Eigen::VectorXd* ptrXm = new Eigen::VectorXd;
	*ptrXm = Eigen::VectorXd::LinSpaced(min,passo,max);

	// Altura maxima
	const double& Hm {P[3]};

	//Ponteiros Matrizes complexas
	Eigen::MatrixXcd* ptrBkr = new Eigen::MatrixXcd(ptrXm->size(),3);
	Eigen::MatrixXcd* ptrBkx = new Eigen::MatrixXcd(ptrXm->size(),3);
  Eigen::MatrixXcd* ptrBkh = new Eigen::MatrixXcd(ptrXm->size(),3);

	Eigen::MatrixXcd* ptrSaida = new Eigen::MatrixXcd(ptrXm->size(),4);

	// Calcula o campo nas componentes
	for(unsigned short int i=0; i < 3; ++i){
		// Valores em Tesla
		ptrBkr->col(i) = ( (2e-7)*(*I)[i] ) /( ( ptrXm->array() - D[i] ).square() + (Hm - H[i])*(Hm - H[i]) ).sqrt();
        ptrBkx->col(i) = ( (2e-7)*(*I)[i]*(ptrXm->array() - D[i])) / ( ( ptrXm->array() - D[i] ).square() + (Hm-H[i])*(Hm-H[i]) ).sqrt();
        ptrBkh->col(i) = ( (2e-7)*(*I)[i]*(Hm - H[i]) )/( (ptrXm->array() - D[i]).square() + (Hm - H[i])*(Hm - H[i]) ).sqrt();
	}

	// Converte os valore e armazena na forma [SBr SBx SBh Bmag]
	ptrSaida->col(0) = ptrBkr->rowwise().sum();
    ptrSaida->col(1) = ptrBkx->rowwise().sum();
    ptrSaida->col(2) = ptrBkh->rowwise().sum();
    ptrSaida->col(3) = (ptrSaida->col(1).array().real().square() + ptrSaida->col(2).array().real().square()).sqrt();

	// Converte para micro Tesla
	*ptrSaida *= 1e6;

	// Limpa os ponteiros utilizados da memoria
	delete I;
	delete ptrXm;
	delete ptrBkr;
	delete ptrBkx;
	delete ptrBkh;

	return ptrSaida;
}

/**
* Calcula o campo elétrico medido em um perfil
* CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, P)
*
* H = Altura dos condutores - [HA HB HC] [m]
* D = Distância entre os condutores - [DA DB DC] [m]
* EspacSubCond = Espaçamento entre os subcondutores [m]
* Ncond = Numero de condutores por feixe
* Dcabo = Diâmetro do cabo do feixe. [m]
* V = Tensão da linha [V]
* AngulosABC = Defasagem angular entre as fases - [AA AB AC] [graus]
* P = Perfil de medição - [Xmin Xmax Passo Altura] [m]
**/
Eigen::MatrixXcd* CalcEkv(const conf& dados){

	const Eigen::Vector3d& H {dados.hmin, dados.hmin, dados.hmin};
	const Eigen::Vector3d& D {dados.pxfeixes[0], dados.pxfeixes[1], dados.pxfeixes[2]};
	const double& EspacSubCond {dados.espacsubcond};
	const double& Ncond {dados.ncond};
	const double& Dcabo {dados.diamcabo};
	const double& V {dados.tensao};
	const Eigen::Vector3d& AngABC {dados.angabc[0], dados.angabc[1], dados.angabc[2]};
	const std::vector<double>& P {dados.linhamed[0], dados.linhamed[1], dados.linhamed[2], dados.linhamed[3]};

	const double& DB {db(EspacSubCond,Ncond)};
	const double& DEQ {deq(DB,Dcabo,Ncond)};

	const Eigen::MatrixXd* ptrMp {Pkk(H,DEQ)}; // Matriz propria
	const Eigen::MatrixXd* ptrMm {Pkl( *Skll(D,H),*Skl(D,H) )}; // Matriz mutua

	const Eigen::MatrixXd* ptrP {MatrizP(*ptrMp, *ptrMm)}; // Matriz de Potenciais

	const Eigen::Vector3cd* ptrTensaoVri {Vri(V,AngABC)};
	Eigen::MatrixXd* ptrC = new Eigen::MatrixXd;; // Matriz de cargas
	*ptrC = (*ptrP).inverse();

	Eigen::Vector3cd* ptrVQ = new Eigen::Vector3cd;
	*ptrVQ = (*ptrC)*(*ptrTensaoVri);

	//https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
	// Resolve o sistema linear ao inves de calcular a inversa
	//*ptrVQ = (*ptrP).colPivHouseholderQr().solve((*ptrTensaoVri).real());

	Eigen::MatrixXcd* ptrCalcEkv = new Eigen::MatrixXcd;
	ptrCalcEkv = Erms(H,D,P,*ptrVQ);

	// Divide os valores
	*ptrCalcEkv /= 1000.0;

	delete ptrMp;
	delete ptrMm;
	delete ptrP;
	delete ptrTensaoVri;
	delete ptrC;
	delete ptrVQ;

	return ptrCalcEkv;
}





/**
* Calcula a tensão crítica de pico (Equação de Peek)
* CoronaVc(M,Delta, Dcond, DistCond) [kV]
*
* M = Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
* Delta = Coeficiente que depende da altura e altitude
* Dcond = Diâmetro do condutor [m]
* DistCond = Distância entre condutores [m]
**/
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


/**
* Calcula o delta crítico
* CoronaVc(M,delta, Dcond, DistCond)
*
* H = Altitude da instalação [m]
* t = temperatura média anual [°C]
**/
double CoronaDeltaCrit(const double& H, const double& Tc){
	return (0.386*(760-0.086*H))/(273+Tc);
}


/**
* Calcula as perdas devido ao Efeito Corona
* CoronaPerdas(Delta,F,Dcond, DistCond,V,Vc) [kW/km]
*
* Delta = Delta Crítico
* F = Frequência da rede [Hz]
* Dcond = Diâmetro do condutor [m]
* DistCond = Distância entre os condutores [m]
* V = Tensão da rede pico-a-pico [V]
* Vc = Tensão Crítica da linha [V]
**/
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


/**
* Calcula o campo elétrico crítico
* CoronaEc(M,Delta,Dcond) [kV/cm]
*
* M = Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
* Delta = Delta Crítico
* F = Frequência da rede [Hz]
* Dcond = Diâmetro do condutor [m]
*
**/
double CoronaEC(const double& M,
								const double& Delta,
								const double& Dcond){

	double A0, A1;

  A0 = M*(21.1e3)*(Delta);
  A1 = (1 + (sqrt(1/0.301)/(sqrt(Delta*Dcond*1000)) ));

  return A0*A1;
}


/**
* Calcula o diâmetro do feixe
* db(S,N)
* S = Espaçamento entre os condutores
* N = Número de condutores
**/
double db(const double& S, const double& N){
	return S/sin(PI/N);
}


/**
* Calcula o feixe equivalente
*   deq(db, D, N)
* db = Diâmetro do feixe
* D  = Diâmetro do cabo
* N  = Número de condutores
**/
double deq(const double& db0, const double& D, const double& N){
	return db0*( pow(N*D/db0,1.0/N) );
}


/**
* Retorna um vetor com os campos eletricos correspondentes aos pontos [SEx SEy Erms];
* Erms(P,H,D,Q)
* H = Vetor com as posições verticais dos cabos. [A B C]
* D = Vetor com as posições horizontais relativas dos cabos [A 0 C]
* P = Vetor com as caracteristicas da linha de medição [-X X Passo Altura]
* Q = Matriz com as cargas
**/
Eigen::MatrixXcd* Erms(const Eigen::Vector3d& H,
											 const Eigen::Vector3d& D,
											 const std::vector<double>& P,
											 const Eigen::Vector3cd& VQ){

	//Passando por referencia, para facilitar a leitura
  const double& max {P[0]+P[2]*((P[1]-P[0])/P[2])};
  const double& min {((P[1]-P[0])/P[2])+1};
  const double& passo {P[0]};

	//Inicializa o ponteiro Xm
	Eigen::VectorXd* ptrXm = new Eigen::VectorXd;
	*ptrXm = Eigen::VectorXd::LinSpaced(min,passo,max);

		//Inicializa ponteiros Ex e Ey (complexas) 2D
	Eigen::MatrixXcd* ptrEx = new Eigen::MatrixXcd(ptrXm->size(),3); // Campo em x
	Eigen::MatrixXcd* ptrEy = new Eigen::MatrixXcd(ptrXm->size(),3); // Campo em y

	//Inicializa ponteiros dos termos da equacao (reais)
	Eigen::VectorXd* ptrT1x = new Eigen::VectorXd(ptrXm->size()); // Campo em x
	Eigen::VectorXd* ptrT2x = new Eigen::VectorXd(ptrXm->size()); // Campo em x
	Eigen::VectorXd* ptrT1y = new Eigen::VectorXd(ptrXm->size()); // Campo em x
	Eigen::VectorXd* ptrT2y = new Eigen::VectorXd(ptrXm->size()); // Campo em x

	Eigen::VectorXd* ptrDPML = new Eigen::VectorXd(ptrXm->size()); // #Distância entre o ponto de medição e a linha

	const double Hm = P[3]; // Altura do ponto de medicao
	cx* C = new cx;

	for(unsigned short int i=0; i < 3; ++i){

		*ptrDPML = ptrXm->array() - D[i];
		*C = VQ[i]/(2*PI*EPSILON);

		*ptrT1x = ptrDPML->array()/(ptrDPML->array().square() + (H[i]-Hm)*(H[i]-Hm));
		*ptrT2x = ptrDPML->array()/(ptrDPML->array().square() + (H[i]+Hm)*(H[i]+Hm));

		*ptrT1y = (Hm-H[i])/(ptrDPML->array().square() + (H[i]-Hm)*(H[i]-Hm));
		*ptrT2y = (Hm+H[i])/(ptrDPML->array().square() + (H[i]+Hm)*(H[i]+Hm));

		(*ptrEx).col(i) = (*C)*((*ptrT1x) - (*ptrT2x));
		(*ptrEy).col(i) = (*C)*((*ptrT1y) - (*ptrT2y));

	}


	// Inicializa o ponteiro de saida
	Eigen::MatrixXcd* ptrErms = new Eigen::MatrixXcd;
	*ptrErms = Eigen::MatrixXcd::Zero(ptrXm->size(),3);
	ptrErms->col(0) = ptrEx->rowwise().sum();
	ptrErms->col(1) = ptrEy->rowwise().sum();

	ptrErms->col(2) = ( ptrErms->col(0).array().real().square() + ptrErms->col(0).array().imag().square() +
											ptrErms->col(1).array().real().square() + ptrErms->col(1).array().imag().square() ).sqrt();

	//Remove os ponteiros desnecessarios da memoria
	delete ptrXm;
	delete ptrEx;
	delete ptrEy;
	delete ptrT1x;
	delete ptrT2x;
	delete ptrT1y;
	delete ptrT2y;
	delete ptrDPML;
	delete C;

	return ptrErms;
}


Eigen::Vector3cd* Iri(const double& I, const Eigen::Vector3d& AngABC){

	Eigen::Vector3cd* ptrIri = new Eigen::Vector3cd;

	ptrIri->real() = (((PI/180.0)*AngABC).array().cos())*(I/sqrt(3));
	ptrIri->imag() = (((PI/180.0)*AngABC).array().sin())*(I/sqrt(3));

	return ptrIri;
}


/**
* Retorna a Matriz de Coeficientes de Potenciais de Maxwell (3,3)
* MatrizP(Mp, Mm)
* Mp = Matriz própria de coeficientes
* Mm = Matriz mútua de coeficientes
**/
Eigen::MatrixXd* MatrizP(const Eigen::MatrixXd& MP, const Eigen::MatrixXd& MM){

	Eigen::MatrixXd* ptrMatrizP = new Eigen::MatrixXd(3,3);

	*ptrMatrizP = MM;
  (*ptrMatrizP)(0,0) = MP(0,0);
  (*ptrMatrizP)(1,1) = MP(1,1);
  (*ptrMatrizP)(2,2) = MP(2,2);

  return ptrMatrizP;
}


/*
* Calcula o coeficiente de potencial próprio
* do condutor k.
*   Pkk(H, deq)
* H = Altura dos condutores - [HA HB HC] [m]
* deq0  = Diâmetro equivalente do condutor
* */
Eigen::MatrixXd* Pkk(const Eigen::Vector3d& H, const double& deq0){

	Eigen::MatrixXd* ptrPkk = new Eigen::MatrixXd(3,3);
	Eigen::Vector3d* ptrM = new Eigen::Vector3d;

	*ptrPkk = Eigen::MatrixXd::Zero(3,3);
  *ptrM = (1/(2*PI*EPSILON))*((4*H/deq0).array().log());
  (*ptrPkk)(0,0) = (*ptrM)(0);
  (*ptrPkk)(1,1) = (*ptrM)(1);
  (*ptrPkk)(2,2) = (*ptrM)(2);

	delete ptrM;

  return ptrPkk;

}


/**
* Calcula o coeficiente de potencial mútuo
* entre os condutores k e l.
*   Pkl(Skll, Skl)
* Skll = Distância entre o condutor k e a
*        imagem do condutor l.
* Skl = Distância entre o condutor k e l
* */
Eigen::MatrixXd* Pkl(const Eigen::MatrixXd& Skll0, const Eigen::MatrixXd& Skl0){

	Eigen::MatrixXd* ptrPkl = new Eigen::MatrixXd(3,3);

	*ptrPkl = (1/(2*PI*EPSILON))*((Skll0.array()/Skl0.array()).array().log());

	return ptrPkl;
}



/**
* Retorna a profundidade da imagem do condutor para o campo Magnético
* [Delta] - Profundidade em metros
* ProfImg(R,P,F)
* R = Resistividade ρ do solo. [Ω]
* P = Permeabilidade μ do solo. [H/m] Típico = 4π 10^-7 H/m
* F = Frequência da rede [Hz]
**/
double ProfImg(const double& R, const double& P, const double& FHz){
	return sqrt(R/(PI*P*FHz));
}




/**
* Calcula a distância entre os condutores k e l.
* Skl(D,H)
* D = Matriz com a posição relativa das fases.
* H = Matriz com a altura relativa das fases
* D = [-14  0  14] (exemplo)
* H = [ 28  28 28]
* */
Eigen::MatrixXd* Skl(const Eigen::Vector3d& D, const Eigen::Vector3d& H){

	Eigen::MatrixXd* ptrSkl = new Eigen::MatrixXd(3,3);

	(*ptrSkl)(0,0) = 0.0;
	(*ptrSkl)(1,0) = sqrt((D[0]-D[1])*(D[0]-D[1]) + (H[0]-H[1])*(H[0]-H[1]));
	(*ptrSkl)(2,0) = sqrt((D[0]-D[2])*(D[0]-D[2]) + (H[0]-H[2])*(H[0]-H[2]));

	(*ptrSkl)(0,1) = sqrt( (D[1]-D[0])*(D[1]-D[0]) + (H[1]-H[0])*(H[1]-H[0]) );
	(*ptrSkl)(1,1) = 0.0;
	(*ptrSkl)(2,1) = sqrt((D[1]-D[2])*(D[1]-D[2]) + (H[1]-H[2])*(H[1]-H[2]));

	(*ptrSkl)(0,2) = sqrt((D[2]-D[0])*(D[2]-D[0]) + (H[2]-H[0])*(H[2]-H[0]));
	(*ptrSkl)(1,2) = sqrt((D[2]-D[1])*(D[2]-D[1]) + (H[2]-H[1])*(H[2]-H[1]));
	(*ptrSkl)(2,2) = 0.0;

	return ptrSkl;
}


/**
* Calcula a distância entre o condutor k e a
* imagem do condutor l.
* Skll(D,H)
* D = Matriz com a posição relativa das fases.
*
* D = [-14  0  14] (exemplo)
* H = [ 28  28 28]
* */
Eigen::MatrixXd* Skll(const Eigen::Vector3d& D, const Eigen::Vector3d& H){

	Eigen::MatrixXd* ptrSkll = new Eigen::MatrixXd(3,3);

	(*ptrSkll)(0,0) = 2*H[0];
	(*ptrSkll)(1,0) = sqrt( (D[0]-D[1])*(D[0]-D[1]) + (2*H[0])*(2*H[0]));
	(*ptrSkll)(2,0) = sqrt( (D[0]-D[2])*(D[0]-D[2]) + (2*H[0])*(2*H[0]));

	(*ptrSkll)(0,1) = sqrt( (D[1]-D[0])*(D[1]-D[0]) + (2*H[1])*(2*H[1]));
	(*ptrSkll)(1,1) = 2*H[1];
	(*ptrSkll)(2,1) = sqrt( (D[1]-D[2])*(D[1]-D[2]) + (2*H[1])*(2*H[1]));

	(*ptrSkll)(0,2) = sqrt( (D[2]-D[0])*(D[2]-D[0]) + (2*H[2])*(2*H[2]));
	(*ptrSkll)(1,2) = sqrt( (D[2]-D[1])*(D[2]-D[1]) + (2*H[2])*(2*H[2]) );
	(*ptrSkll)(2,2) = 2*H[2];



	return ptrSkll;

}


/**
* Retorna a Matriz com as tensões(2,3)
* Vri(V, Angulos)
* V = Tensão de fase
* Angulos = Angulos de defasegem das fases. [A B C]
*
* */
Eigen::Vector3cd* Vri(const double& V, const Eigen::Vector3d& AngABC){

	Eigen::Vector3cd* ptrVri = new Eigen::Vector3cd;

	ptrVri->real() = (((PI/180.0)*AngABC).array().cos())*(V/sqrt(3));
	ptrVri->imag() = (((PI/180.0)*AngABC).array().sin())*(V/sqrt(3));

	return ptrVri;

}