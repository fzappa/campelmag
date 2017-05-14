#ifndef PARSERXML_HPP
#define PARSERXML_HPP

#include <iostream>
#include <vector>

struct conf{

    // Dados do vao
    std::string nome {NULL};        // nome do caso
	int grafico {0};                // formato da saida
    double tensao {0.0};       		// numero de condutores no feixe
    double potencia {0.0};        	// diametro do feixe
	std::vector<double> angabc {0.0, 0.0, 0.0};
	std::vector<double> linhamed {0.0, 0.0, 0.0, 0.0};
    double coefm {0.0};      		// comprimento do vao
    double hmax {0.0};				// altura de partida
    double flecha {0.0};			// eds
	double hmin {0.0};
	double corrente {0.0};
    double espacsubcond {0.0};		// numero de divisoes na catenaria
    double diamcabo {0.0};			// nivel de tensao [V]
	double ncond {0.0};
	std::vector<double> pxfeixes {0.0, 0.0, 0.0};

	// Ler e salvar
    void load(const std::string&);
    void save(const std::string&);
};


struct relat{

    // Dados do caso
    std::string titulo {NULL};  // nome do caso
	double tensao {0.0};       	// nivel de tensão
	double potencia {0.0};
	double corrente {0.0};
    double hmin {0.0};          // altura minima
	double hmax {0.0};
    double distfeixe {0.0};     // distancia entre feixes
    double npmed {0.0};			// numero de pontos
    double campel {0.0};		// campo eletrico maximo
    double campmag {0.0};		// campo magnetico maximo

	//Salvar relatorio
    void save(const std::string&);
};




#endif