#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Perfil de campo elétrico
# EPRI AC - Transmission Line Reference Book - 200kV and above
# Capítulo 7 - Electric and Magnetic Fields
# Autor: Alan Franco Rosa
# 03/08/2016
#
# Necessário ter o scipy (numpy e matplotlib)
# Para rodar: só executar "LT-3F-525kv.py"
# nT = (V/m)2 / 90,103              mG = (V/m)2 / 9,010.3

import sys
sys.path.insert(0,'campelmag/campmag.py')

import numpy as np
np.seterr(divide='ignore', invalid='ignore') # desabilitar no debug

import matplotlib.pyplot as plt

from campelmag.campmag import CampelMag
cm = CampelMag()

print("\n#### CALCULO DOS CAMPOS ELÉTRICO E MAGNÉTICO ####")
#Tensão da Linha (V)
V = 765e3

#Potência Transmitida (W)
P = 1.2e9
Ir = P/V

#Angulos das fases (graus)
AngA = 120
AngB = 0
AngC = -120

AngulosABC = [AngA,AngB,AngC]

#Corrente com a defasagem
I = cm.Iri(Ir,AngulosABC)

#Caracteristicas Mecânicas
LinhaDeMedicao = [-80,80,0.5,1.5] #[Xmin Xmax Passo Altura]
Px = np.arange(LinhaDeMedicao[0],LinhaDeMedicao[1]+LinhaDeMedicao[2],LinhaDeMedicao[2])
Hmax = 25.0 #[m]
FLECHA = 8.63 #[m]
EspacSubCond = 0.457 # [m]
Dcabo = 0.02959
Ncond = 4.0
h0 = Hmax-FLECHA-EspacSubCond/2.0
H = [h0,h0,h0] #' #Altura mínima do cabo no feixe
D = [-14.34,0,14.34] #'


print("\nV = {0:5,.2f} [kV]".format(V/1e3))
print("H = [{0:5,.2f}, {1:5,.2f}, {2:5,.2f}] [m]".format(H[0], H[1], H[2]))
print("D = [{0:5,.2f}, {1:5,.2f}, {2:5,.2f}] [m]".format(D[0], D[1], D[2]))
print("Perfil de medição = [{0:5,.2f}, {1:5,.2f}, {2:5,.2f}, {3:5,.2f}] [m]" \
		.format(LinhaDeMedicao[0], LinhaDeMedicao[1], LinhaDeMedicao[2], LinhaDeMedicao[3]))


#Campo Elétrico
# Px = collect(LinhaDeMedicao[1]:LinhaDeMedicao[3]:LinhaDeMedicao[2]);
EkVm = cm.CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)

# print("kV/m máximo = {0:5,.2f} [kV/m]".format(np.max(EkVm[:,2]))
# println(@sprintf "Numero de pontos de medição = %5.2f" length(Px));
# println(@sprintf "Altura de partida dos cabos = %5.2f" H[1,1]+FLECHA+EspacSubCond/2);
# println(@sprintf "Altura dos cabos no ponto mais baixo = %5.2f" H[1]);
# #println(@sprintf "kV/m máximo = %5.2f [kV/m]\n\n" maximum(Py)-3.0801);

#Encontra a menor altura para manter o limite máximo de campo
#h0 = H[1];
# #while maximum(Py) >= 11.41 # Equivale a 8.33 no Tricamp
# #  h0 += 0.001;
# #  H2 = [h0 0 h0]';
# #  Py = CalcEkv(H2, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)
# #end


#Campo Magnético [SBr SBx SBh B]
B = cm.Brms(I,H,D,LinhaDeMedicao)
# println(@sprintf "Campo Elétrico Máximo = %5.2f [kV/m]" maximum(real(EkVm[:,3])));
# println(@sprintf "Campo Magnético Máximo = %5.2f [micro Tesla]\n\n" maximum(real(B[:,4])));

#display(plt.plot(Px, Py-3.0801, color="red", label="Campo Elétrico"));
plt.figure(1)
plt.plot(Px, EkVm[:,2], color="red", marker="None", label="E_max")
plt.title("Campo Elétrico x Perfil de medição")
plt.xlabel("Posição na faixa de passagem [m]")
plt.ylabel("Campo Elétrico [kV/m]")
plt.grid(True)
plt.legend(loc="upper right",fancybox=True)


plt.figure(2)
plt.plot(Px, B[:,3], color="blue", marker="None", label="Bmag")
plt.title("Campo Magnético x Perfil de medição")
plt.xlabel("Posição na faixa de passagem [m]")
plt.ylabel("Campo Magnético ["+r"$\mu T$"+"]")
plt.grid("True",which="both",ls="-")
plt.legend(loc="upper right",fancybox="True")
plt.show()



