#ifndef FUNCOESMAG_H
#define FUNCOESMAG_H

#include <vector>
#include <eigen3/Eigen/Dense>
#include "ParserXML.h"

//Campo magnetico resultante
Eigen::MatrixXcd* Brms(const conf& dados);

//Campo eletrico resultante
Eigen::MatrixXcd* CalcEkv(const conf& dados);

// Tensao critica de corona
double CoronaVc(const double& M,
								const double& Delta,
								const double& Dcond,
								const double& DistCond);


// Delta critico de corona
double CoronaDeltaCrit(const double& H, const double& Tc);


// Perdas corona
double CoronaPerdas(const double& Delta,
									  const double& FHz,
										const double& Dcond,
										const double& DistCond,
										const double& Vpp,
										const double& Vc);

// Campo eletrico critico corona
double CoronaEC(const double& M,
								const double& Delta,
								const double& Dcond);


// Calcula o diametro equivalente do feixe
double db(const double& S, const double& N);


// Calcula o feixe equivalente
double deq(const double& db0, const double& D, const double& N);


// retorna os campos eletricos nos pontos
Eigen::MatrixXcd* Erms(const Eigen::Vector3d& H,
											 const Eigen::Vector3d& D,
											 const std::vector<double>& P,
											 const Eigen::Vector3cd& VQ);


//Matriz com as correntes
Eigen::Vector3cd* Iri(const double& V,
										  const Eigen::Vector3d& AngABC);


// Matriz de coeficientes de potenciais de Maxwell[3,3]
Eigen::MatrixXd* MatrizP(const Eigen::MatrixXd& MP,
												 const Eigen::MatrixXd& MM);

// Coeficiente de potencial proprio
Eigen::MatrixXd* Pkk(const Eigen::Vector3d& H,
										 const double& deq0);

// Coeficiente de potencial mutuo
Eigen::MatrixXd* Pkl(const Eigen::MatrixXd& Skll0,
										 const Eigen::MatrixXd& Skl0);


// Profundidade da imagem do condutor para o campo magnetico
double ProfImg(const double& R,
							 const double& P,
							 const double& FHz);



//// Calcula a distância entre os condutores k e l
Eigen::MatrixXd* Skl(const Eigen::Vector3d& D,
										 const Eigen::Vector3d& H);

//Calcula a distancia entre o condutor k e a imagem do condutor l.
Eigen::MatrixXd* Skll(const Eigen::Vector3d& D,
										  const Eigen::Vector3d& H);

//Retorna a Matriz com as tensões(2,3)
Eigen::Vector3cd* Vri(const double& V,
										  const Eigen::Vector3d& AngABC);



// Criar sequencia de dados
template <typename T>
inline const std::vector<T>* collect(T const& inicio, T const& passo, T const& fim){
  std::vector<T>* v = new std::vector<T>;
  for(T i=inicio; i<=fim; i+=passo){v->push_back(i);}
  return v;
}


#endif