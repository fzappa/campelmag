#!/usr/bin/env julia
#
# Perfil de campo elétrico
# EPRI AC - Transmission Line Reference Book - 200kV and above
# Capítulo 7 - Electric and Magnetic Fields
# Autor: Alan Franco Rosa
# 03/08/2016
#
# Necessário ter o python-matplotlib
# No Julia: using Pkg; Pkg.add("PyPlot")
# Para rodar: só abrir o julia e chamar include("LT-3F-525kv.jl")
# nT = (V/m)2 / 90,103              mG = (V/m)2 / 9,010.3

# Ativar o projeto local
using Pkg
Pkg.activate(@__DIR__)

include("CampelMag.jl")
using PyCall
using PyPlot
using Printf

#Tensão da Linha (V)
V = 765e3;

#Potência Transmitida (W)
P = 1.2e9;
Ir = P/V;

#Angulos das fases (graus)
AngA = 120;
AngB = 0;
AngC = -120;

AngulosABC = [AngA AngB AngC];

#Corrente com a defasagem
Itemp = Iri(Ir,AngulosABC);
IA = complex(Itemp[1,1],Itemp[1,2]);
IB = complex(Itemp[2,1],Itemp[2,2]);
IC = complex(Itemp[3,1],Itemp[3,2]);

I = [IA IB IC];

#Caracteristicas Mecânicas
LinhaDeMedicao = [-80 80 0.5 1.5]; #[Xmin Xmax Passo Altura]
Hmax = 25; #[m]
FLECHA = 8.63; #[m]
EspacSubCond = 0.457; # [m]
Dcabo = 0.02959;
Ncond = 4;
h0 = Hmax-FLECHA-EspacSubCond/2;
H = [h0 h0 h0]'; #Altura mínima do cabo no feixe
D = [-14.34 0 14.34]';


println(@sprintf "\n\nV = %5.2f [kV]" V/1000);
println(@sprintf "H = [%5.2f %5.2f %5.2f] [m]" H[1] H[2] H[3]);
println(@sprintf "D = [%5.2f %5.2f %5.2f] [m]" D[1] D[2] D[3]);
println(@sprintf "Perfil de medição = [%5.2f %5.2f %5.2f %5.2f] [m]" LinhaDeMedicao[1] LinhaDeMedicao[2] LinhaDeMedicao[3] LinhaDeMedicao[4]);


#Campo Elétrico
Px = collect(LinhaDeMedicao[1]:LinhaDeMedicao[3]:LinhaDeMedicao[2]);
EkVm = CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao);

#println(@sprintf "kV/m máximo = %5.2f [kV/m]" maximum(E[:,3]));
println(@sprintf "Numero de pontos de medição = %5.2f" length(Px));
println(@sprintf "Altura de partida dos cabos = %5.2f" H[1,1]+FLECHA+EspacSubCond/2);
println(@sprintf "Altura dos cabos no ponto mais baixo = %5.2f" H[1]);
#println(@sprintf "kV/m máximo = %5.2f [kV/m]\n\n" maximum(Py)-3.0801);

#Encontra a menor altura para manter o limite máximo de campo
#h0 = H[1];
#while maximum(Py) >= 11.41 # Equivale a 8.33 no Tricamp
#  h0 += 0.001;
#  H2 = [h0 0 h0]';
#  Py = CalcEkv(H2, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)
#end


#Campo Magnético [SBr SBx SBh B]
B = Brms(I,H,D,LinhaDeMedicao);
println(@sprintf "Campo Elétrico Máximo = %5.2f [kV/m]" maximum(real(EkVm[:,3])));
println(@sprintf "Campo Magnético Máximo = %5.2f [micro Tesla]\n\n" maximum(real(B[:,4])));

#display(plt.plot(Px, Py-3.0801, color="red", label="Campo Elétrico"));
figure(1);
#display(plot(Px, EkVm[:,1], color="green", marker="None", label="E_x"));
#display(plot(Px, EkVm[:,2], color="blue", marker="None", label="E_y"));
plot(Px, EkVm[:,3], color="red", marker="None", label="E_max")
title("Campo Elétrico x Perfil de medição")
xlabel("Posição na faixa de passagem [m]")
ylabel("Campo Elétrico [kV/m]")
grid(true)
legend(loc="upper right",fancybox=true)


figure(2);
plot(Px, B[:,4], color="red", marker="None", label="Bmag")
#display(plot(Px, B[:,3], color="green", marker="None", label="Bh"));
#display(plot(Px, B[:,2], color="blue", marker="None", label="Bx"));
#display(plot(Px, B[:,1], color="cyan", marker="None", label="Br"));
title("Campo Magnético x Perfil de medição")
#display(title("\$\\alpha + \\beta\$"));
xlabel("Posição na faixa de passagem [m]")
ylabel("Campo Magnético [\$\\mu T\$]")
grid("True",which="both",ls="-")
legend(loc="upper right",fancybox="true")
PyPlot.show()
