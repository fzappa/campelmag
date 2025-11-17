# Para usar como módulo, descomentar a linha abaixo:
# include("src/CampelMag.jl")
# using .CampelMag

# Para compatibilidade com código antigo, incluir as funções diretamente:
using PhysicalConstants.CODATA2018: ε_0
using Unitful: ustrip, @u_str
using Printf

# Constantes físicas (permissividade do vácuo em F/m)
const Epsilon = ustrip(Float64, u"F/m", ε_0)

include("funcoes/db.jl")         # Calcula o diâmetro do feixe
include("funcoes/deq.jl")        # Calcula o feixe equivalente
include("funcoes/Skl.jl")        # Calcula a distância entre os condutores k e l.
include("funcoes/Skll.jl")       # Calcula a distância entre o condutor k e a imagem do condutor l.
include("funcoes/ProfImg.jl")    # Retorna a profundidade da imagem do condutor para o campo Magnético
include("funcoes/Pkk.jl")        # Calcula o coeficiente de potencial próprio do condutor k.
include("funcoes/Pkl.jl")        # Calcula o coeficiente de potencial mútuo entre os condutores k e l.
include("funcoes/MatrizP.jl")    # Retorna a Matriz de Coeficientes de Potenciais de Maxwell (3,3)
include("funcoes/Vri.jl")        # Retorna a Matriz com as tensões(2,3)
include("funcoes/Iri.jl")        # Retorna a Matriz com as correntes(2,3)
include("funcoes/Erms.jl")       # Retorna um vetor com as tensões correspondentes aos pontos [SEx SEy Erms];
include("funcoes/CalcEkv.jl")    # Calcula o campo elétrico medido em um perfil
include("funcoes/Brms.jl")       # Retorna um vetor com o campo magnético resultante nos pontos da faixa de passagem
include("funcoes/Corona.jl")     # Funções para avaliar a ocorrência de Corona
