#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>


#include "FuncoesMag.h"
#include "GeraGrafico.h"
#include "ParserXML.h"

using namespace std;
using namespace Eigen;

int main()
{

	try{

		//Leitura dos dados
		conf lt;
		relat r;
		lt.load("dados.xml");

		//Passando por referencia, para facilitar a leitura
		const double& minimo {((lt.linhamed[1]-lt.linhamed[0])/lt.linhamed[2]+1)};
		const double& passo {lt.linhamed[0]};
		const double& maximo {lt.linhamed[0]+lt.linhamed[2]*((lt.linhamed[1]-lt.linhamed[0])/lt.linhamed[2])};


		// Valores Definidos
		printf("\n V = %5.2f [kV]", lt.tensao/1000.0);
		printf("\n Potencia transmitida: %5.2f [MW]", lt.potencia/(1.0e6));
		printf("\n Altura minima dos feixes = [%5.2f %5.2f %5.2f] [m]", lt.hmin, lt.hmin, lt.hmin);
		printf("\n Distancia entre os feixes = [%5.2f %5.2f %5.2f] [m]", lt.pxfeixes[0], lt.pxfeixes[1], lt.pxfeixes[2]);
		printf("\n Perfil de medição: ");
		printf("\n\t\t Xmin: %5.2f", lt.linhamed[0]);
		printf("\n\t\t Xmax: %5.2f", lt.linhamed[1]);
		printf("\n\t\t Passo: %5.2f", lt.linhamed[2]);
		printf("\n\t\t Altura: %5.2f", lt.linhamed[3]);

		// Pontos sobre o eixo X (comprimento)
		Eigen::VectorXd Px {Eigen::VectorXd::LinSpaced(minimo,passo,maximo)};
		printf("\n\nNumero de pontos de medição = %5.2f", (double)Px.size());

		printf("\nAltura de partida dos cabos = %5.2f", lt.hmax);
		printf("\nAltura dos cabos no ponto mais baixo = %5.2f", lt.hmin);

		// Calcula o campo elétrico
		Eigen::MatrixXcd* ptrEkVm = new Eigen::MatrixXcd;
		ptrEkVm = CalcEkv(lt);

		//Campo Magnético [SBr SBx SBh B]
		Eigen::MatrixXcd* ptrBrms = new Eigen::MatrixXcd;
		ptrBrms = Brms(lt);

		r.campel = ptrEkVm->col(2).real().maxCoeff();
		r.campmag = ptrBrms->col(3).real().maxCoeff();
		cout << "\nCampo Elétrico máximo = " << r.campel << " [kV/m]\n";
		cout << "Campo Magnético Máximo = " << r.campmag << " [micro Tesla]\n\n";


		// Mapea a saida do Eigen para plotar
		std::vector<double>* vetorx = new vector<double>();
		vetorx->resize(Px.size());
		Eigen::VectorXd::Map(&vetorx->at(0), Px.size()) = Px;

		std::vector<double>* vetoryE = new vector<double>();
		vetoryE->resize(Px.size());
		Eigen::VectorXd::Map(&vetoryE->at(0), ptrEkVm->col(2).size()) = ptrEkVm->col(2).real();

		std::vector<double>* vetoryM = new vector<double>();
		vetoryM->resize(Px.size());
		Eigen::VectorXd::Map(&vetoryM->at(0), ptrBrms->col(3).size()) = ptrBrms->col(3).real();

		// Gambiarra pra nao mudar o gera_grafico
		string temp_nome {lt.nome};
		double temp_hmax {lt.hmax};

		lt.nome = "Campo Elétrico";
		lt.hmax = r.campel;
		gera_grafico(vetorx, vetoryE, lt);

		lt.nome = "Campo Magnético";
		lt.hmax = r.campmag;
		gera_grafico(vetorx, vetoryM, lt);

		// Restaura os valores
		lt.nome = temp_nome;
		lt.hmax = temp_hmax;

		//Salva os resultados do campo eletrico
		ofstream resultados;
		resultados.open ("campoEletrico.dat");
		for(unsigned short int i = 0;i< Px.size();++i){
			resultados << std::setprecision(4) << Px(i) << "\t \t \t" << (*ptrEkVm)(i,2).real() << endl;
		}
		resultados.close();

		//Salva os resultados do campo magnetico
		resultados.open ("campoMagnetico.dat");
		for(unsigned short int i = 0;i< Px.size();++i){
			resultados << std::setprecision(4) << Px(i) << "\t \t \t" << (*ptrBrms)(i,3).real() << endl;
		}
		resultados.close();

		// Salva o relatorio
		r.titulo = lt.nome;
		r.tensao = lt.tensao;
		r.potencia = lt.potencia;
		r.corrente = lt.corrente;
		r.hmin = lt.hmin;
		r.hmax = lt.hmax;
		r.distfeixe = lt.pxfeixes[2];
		r.npmed = (double)Px.size();
		r.save("relatorio.xml");

		//TODO -->> Plotar graficos

		delete ptrEkVm;
		delete ptrBrms;


	}	catch (std::exception &e){
		cout << "Erro: " << e.what() << "\n";
	}

	return 0;
}
