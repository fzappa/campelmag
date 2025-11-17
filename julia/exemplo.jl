#!/usr/bin/env julia
"""
Exemplo de cálculo de campo elétrico e magnético
Linha de transmissão 765 kV

Para executar:
    julia exemplo.jl
"""

# Ativar o projeto local
using Pkg
Pkg.activate(@__DIR__)

include("CampelMag.jl")
using PyPlot
using Printf

# Parâmetros da linha
V = 765e3  # Tensão da linha (V)
P = 1.2e9  # Potência transmitida (W)
Ir = P / V

# Ângulos das fases (graus)
AngA = 120
AngB = 0
AngC = -120
AngulosABC = [AngA, AngB, AngC]

# Correntes com defasagem
Itemp = Iri(Ir, AngulosABC)
IA = complex(Itemp[1,1], Itemp[1,2])
IB = complex(Itemp[2,1], Itemp[2,2])
IC = complex(Itemp[3,1], Itemp[3,2])
I = [IA, IB, IC]

# Características mecânicas
LinhaDeMedicao = [-80, 80, 0.5, 1.5]  # [Xmin Xmax Passo Altura]
Hmax = 25  # [m]
FLECHA = 8.63  # [m]
EspacSubCond = 0.457  # [m]
Dcabo = 0.02959
Ncond = 4
h0 = Hmax - FLECHA - EspacSubCond/2
H = [h0, h0, h0]  # Altura mínima do cabo no feixe
D = [-14.34, 0, 14.34]  # Posições horizontais

# Exibir parâmetros
println(@sprintf "\n\nV = %5.2f [kV]" V/1000)
println(@sprintf "H = [%5.2f %5.2f %5.2f] [m]" H[1] H[2] H[3])
println(@sprintf "D = [%5.2f %5.2f %5.2f] [m]" D[1] D[2] D[3])
println(@sprintf("Perfil de medição = [%5.2f %5.2f %5.2f %5.2f] [m]",
    LinhaDeMedicao[1], LinhaDeMedicao[2], LinhaDeMedicao[3], LinhaDeMedicao[4]))

# Campo elétrico
Px = collect(LinhaDeMedicao[1]:LinhaDeMedicao[3]:LinhaDeMedicao[2])
EkVm = CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)

println(@sprintf "Número de pontos de medição = %d" length(Px))
println(@sprintf "Altura de partida dos cabos = %5.2f" H[1] + FLECHA + EspacSubCond/2)
println(@sprintf "Altura dos cabos no ponto mais baixo = %5.2f" H[1])

# Campo magnético [SBr SBx SBh B]
B = Brms(I, H, D, LinhaDeMedicao)
println(@sprintf "Campo Elétrico Máximo = %5.2f [kV/m]" maximum(real(EkVm[:,3])))
println(@sprintf "Campo Magnético Máximo = %5.2f [µT]\n\n" maximum(real(B[:,4])))

# Plotar resultados
figure(1, figsize=(10, 6))
plot(Px, EkVm[:,3], color="red", linewidth=2, label="E_max")
title("Campo Elétrico × Perfil de Medição")
xlabel("Posição na faixa de passagem [m]")
ylabel("Campo Elétrico [kV/m]")
grid(true)
legend(loc="upper right", fancybox=true)

figure(2, figsize=(10, 6))
plot(Px, B[:,4], color="red", linewidth=2, label="B_mag")
title("Campo Magnético × Perfil de Medição")
xlabel("Posição na faixa de passagem [m]")
ylabel("Campo Magnético [µT]")
grid(true, which="both", ls="-")
legend(loc="upper right", fancybox=true)

PyPlot.show()
