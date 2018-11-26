#ifndef FUNCOESMAG_HPP
#define FUNCOESMAG_HPP

//#define EIGEN_USE_MKL_ALL
//#include <Eigen/Core>
#include <Eigen/Dense>
#include <memory>  // std::unique_ptr
#include <vector>

struct conf {

    // Dados do vao
    std::string nome{""};  // nome do caso
    double tensao{0.0};
    double potencia{0.0};
    std::vector<double> angabc{0.0, 0.0, 0.0};         // angulos de fase
    std::vector<double> linhamed{0.0, 0.0, 0.0, 0.0};  // xmin, xmax, passo, altura
    double coefm{0.0};                                 // coeficiente de medicao
    double hmax{0.0};                                  // altura de partida (altura da torre)
    double flecha{0.0};                                // flecha
    double hmin{0.0};                                  // altura mininma
    double corrente{0.0};                              // corrente em A
    double espacsubcond{0.0};                     // espacamento entre os subcondutores do feixe
    double diamcabo{0.0};                         // diametro do cabo em m
    double ncond{0.0};                            // quantidade de condutores por feixe
    std::vector<double> pxfeixes{0.0, 0.0, 0.0};  // posicao dos feixes em relacao a x
};

// Campo magnetico resultante
std::unique_ptr<Eigen::MatrixXcd> Brms(const conf &dados);

// Campo eletrico resultante
std::unique_ptr<Eigen::MatrixXcd> CalcEkv(const conf &dados);

// Tensao critica de corona
double CoronaVc(const double &M, const double &Delta, const double &Dcond, const double &DistCond);

// Delta critico de corona
double CoronaDeltaCrit(const double &H, const double &Tc);

// Perdas corona
double CoronaPerdas(const double &Delta, const double &FHz, const double &Dcond,
                    const double &DistCond, const double &Vpp, const double &Vc);

// Campo eletrico critico corona
double CoronaEC(const double &M, const double &Delta, const double &Dcond);

// Calcula o diametro equivalente do feixe
double db(const double &S, const double &N);

// Calcula o feixe equivalente
double deq(const double &db0, const double &D, const double &N);

// retorna os campos eletricos nos pontos
std::unique_ptr<Eigen::MatrixXcd> Erms(const Eigen::Vector3d &H, const Eigen::Vector3d &D,
                                       const std::vector<double> &P, const Eigen::Vector3cd &VQ);

// Matriz com as correntes
std::unique_ptr<Eigen::Vector3cd> Iri(const double &V, const Eigen::Vector3d &AngABC);

// Matriz de coeficientes de potenciais de Maxwell[3,3]
std::unique_ptr<Eigen::MatrixXd> MatrizP(const Eigen::MatrixXd &MP, const Eigen::MatrixXd &MM);

// Coeficiente de potencial proprio
std::unique_ptr<Eigen::MatrixXd> Pkk(const Eigen::Vector3d &H, const double &deq0);

// Coeficiente de potencial mutuo
std::unique_ptr<Eigen::MatrixXd> Pkl(const Eigen::MatrixXd &Skll0, const Eigen::MatrixXd &Skl0);

// Profundidade da imagem do condutor para o campo magnetico
double ProfImg(const double &R, const double &P, const double &FHz);

//// Calcula a distância entre os condutores k e l
std::unique_ptr<Eigen::MatrixXd> Skl(const Eigen::Vector3d &D, const Eigen::Vector3d &H);

// Calcula a distancia entre o condutor k e a imagem do condutor l.
std::unique_ptr<Eigen::MatrixXd> Skll(const Eigen::Vector3d &D, const Eigen::Vector3d &H);

// Retorna a Matriz com as tensões(2,3)
std::unique_ptr<Eigen::Vector3cd> Vri(const double &V, const Eigen::Vector3d &AngABC);

// Criar sequencia de dados
// template <typename T>
// inline const std::vector<T> *collect(T const &inicio, T const &passo, T const &fim)
//{
//    std::vector<T> *v = new std::vector<T>;
//    for (T i = inicio; i <= fim; i += passo) {
//        v->push_back(i);
//    }
//    return v;
//}

#endif
