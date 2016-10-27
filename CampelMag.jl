# Para instalar o pacote com as constantes físicas
# Pkg.clone("https://github.com/DataWookie/PhysicalConstants.jl")
#
# Funções para plotar o perfil de campo elétrico e magnético
# EPRI AC - Transmission Line Reference Book - 200kV and above
# Capítulo 7 - Electric and Magnetic Fields
# Autor: Alan Franco
# 03/08/2016

include("funcoes/Brms.jl");       # Retorna um vetor com o campo magnético resultante nos pontos da faixa de passagem
include("funcoes/CalcEkv.jl");    # Calcula o campo elétrico medido em um perfilCalcula o campo elétrico medido em um perfil
include("funcoes/db.jl");         # Calcula o diâmetro do feixe
include("funcoes/deq.jl");        # Calcula o feixe equivalente
include("funcoes/Erms.jl");       # Retorna um vetor com as tensões correspondentes aos pontos [SEx SEy Erms];
include("funcoes/Iri.jl");        # Retorna a Matriz com as correntes(2,3)
include("funcoes/MatrizP.jl");    # Retorna a Matriz de Coeficientes de Potenciais de Maxwell (3,3)
include("funcoes/Pkk.jl");        # Calcula o coeficiente de potencial próprio do condutor k.
include("funcoes/Pkl.jl");        # Calcula o coeficiente de potencial mútuo entre os condutores k e l.
include("funcoes/ProfImg.jl");    # Retorna a profundidade da imagem do condutor para o campo Magnético
include("funcoes/Skl.jl");        # Calcula a distância entre os condutores k e l.
include("funcoes/Skll.jl");       # Calcula a distância entre o condutor k e a imagem do condutor l.
include("funcoes/Vri.jl");        # Retorna a Matriz com as tensões(2,3)Retorna a Matriz com as tensões(2,3)
include("funcoes/Corona.jl");     # Funções para avaliar a ocorrência de Corona
