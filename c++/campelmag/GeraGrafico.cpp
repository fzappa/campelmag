#include <stdexcept>
#include <chrono>
#include <thread>

#include "GeraGrafico.hpp"
#include "gnuplot_i.hpp"

// Delay para o programa conseguir acessar o disco
//using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
//using std::chrono::system_clock;


void gera_grafico(const std::vector<double>* px,
									const std::vector<double>* py,
									const conf& dados){

			// Caso seja passado um ponteiro nulo
			if(px==nullptr || py==nullptr) throw std::runtime_error(std::string("Ponteiro x ou y nulo."));

			// ------- Gera o grafico usando o Gnuplot ---------- //
			Gnuplot gp;
			std::ostringstream legenda;  // para compor a legenda

			// Limpa a tabela
			gp.reset_all();

			// Para escolher o formato de saida do grafico
			switch (dados.grafico){
				case 1: // PS
					gp.savetops(dados.nome);
					break;
				case 2: // SVG
					gp.savetosvg(dados.nome);
					break;
				case 3: // JPG
					gp.savetojpg(dados.nome);
					break;
				case 4: // PNG
					gp.savetopng(dados.nome);
					break;
				case 5: // PDF
					gp.savetopdf(dados.nome);
					break;
				case 6: // EPS Latex
					gp.savetoepslatex(dados.nome);
					break;
				default: // Se nao fornecer opcao, salva em png
					gp.savetopng(dados.nome);
			}

			//Define a grade e forma do traco
			gp.set_grid().set_style("lines");

			// Define o range XY
			gp.set_xrange(dados.linhamed[0],dados.linhamed[1]).set_yrange(0,dados.hmax+5.0);

			// Titulo
			gp.set_title(dados.nome);

			//Legenda dos eixos
			gp.set_xlabel("Comprimento").set_ylabel("Campo");

			// Legenda do tracado
			legenda << dados.nome;

			// Desenha os pontos
			gp.plot_xy(*px,*py,legenda.str());

			// Executa uma pausa devido ao delay de escrita em disco.
			// Necessario, pois o gnuplot salva em um arquivo temporario
			// antes de gerar o arquivo.
			std::this_thread::sleep_for(100ms);
}