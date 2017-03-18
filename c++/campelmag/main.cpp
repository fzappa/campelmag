#include <iostream>
#include <vector>
#include "FuncoesMag.h"
#include "Constantes.h"

using namespace std;
using namespace Eigen;

int main()
{

	try{

		// Valores Definidos
		printf("\n\nV = %5.2f [kV]", V/1000.0);
		printf("\nH = [%5.2f %5.2f %5.2f] [m]", H[0], H[1], H[2]);
		printf("\nD = [%5.2f %5.2f %5.2f] [m]", D[0], D[1], D[2]);
		printf("\nPerfil de medição = [%5.2f %5.2f %5.2f %5.2f] [m]\n", LinhaDeMedicao[0], LinhaDeMedicao[1], LinhaDeMedicao[2], LinhaDeMedicao[3]);

		// Pontos sobre o eixo X (comprimento)
		Eigen::VectorXd Px {Eigen::VectorXd::LinSpaced(minimo,passo,maximo)};
		printf("Numero de pontos de medição = %5.2f\n", (double)Px.size());

		printf("Altura de partida dos cabos = %5.2f\n", H[0]+FLECHA+EspacSubCond/2.0);
		printf("Altura dos cabos no ponto mais baixo = %5.2f\n", H[0]);

		// Calcula o campo elétrico
		Eigen::MatrixXcd* ptrEkVm = new Eigen::MatrixXcd;
		ptrEkVm = CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao);

		//Campo Magnético [SBr SBx SBh B]
		Eigen::MatrixXcd* ptrBrms = new Eigen::MatrixXcd;
		ptrBrms = Brms(I,H,D,LinhaDeMedicao);

		cout << "Campo Elétrico máximo = " << ptrEkVm->col(2).real().maxCoeff() << " [kV/m]\n";
		cout << "Campo Magnético Máximo = " << ptrBrms->col(3).real().maxCoeff() << " [micro Tesla]\n\n";
		//TODO -->> Plotar graficos
		//TODO -->> Ler XML
		//TODO -->> Escrever XML


		delete ptrEkVm;
		delete ptrBrms;


	}	catch (std::exception &e){
		cout << "Erro: " << e.what() << "\n";
	}

	return 0;
}
