"""
    CampelMag

Módulo para cálculo de campos elétricos e magnéticos de linhas de transmissão.

Implementação de algumas funções para cálculo 2D de campos elétricos e magnéticos
de linhas de transmissão aéreas.

Baseado em: EPRI AC - Transmission Line Reference Book - 200kV and above
Capítulo 7 - Electric and Magnetic Fields

Autor: Alan Franco Rosa
Data: 03/08/2016
Atualizado: 2025
"""
module CampelMag

using PhysicalConstants.CODATA2018: ε_0
using Unitful: ustrip, @u_str
using Printf

# Constantes físicas (permissividade do vácuo em F/m)
const Epsilon = ustrip(Float64, u"F/m", ε_0)

# Exportar todas as funções públicas
export Brms, CalcEkv, Corona, db, deq, Erms, Iri, MatrizP
export Pkk, Pkl, ProfImg, Skl, Skll, Vri
export CoronaVc, CoronaDelta, CoronaPerdas, CoronaEc

# Incluir os arquivos de função
include("../funcoes/db.jl")
include("../funcoes/deq.jl")
include("../funcoes/Skl.jl")
include("../funcoes/Skll.jl")
include("../funcoes/ProfImg.jl")
include("../funcoes/Pkk.jl")
include("../funcoes/Pkl.jl")
include("../funcoes/MatrizP.jl")
include("../funcoes/Vri.jl")
include("../funcoes/Iri.jl")
include("../funcoes/Erms.jl")
include("../funcoes/CalcEkv.jl")
include("../funcoes/Brms.jl")
include("../funcoes/Corona.jl")

end # module
