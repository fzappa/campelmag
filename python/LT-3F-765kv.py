#!/usr/bin/env python
"""
Perfil de campo elétrico
EPRI AC - Transmission Line Reference Book - 200kV and above
Capítulo 7 - Electric and Magnetic Fields
Autor: Alan Franco Rosa
03/08/2016

Necessário ter o scipy (numpy e matplotlib)
nT = (V/m)2 / 90,103              mG = (V/m)2 / 9,010.3
"""

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

# Adicionar o diretório campelmag ao path
sys.path.insert(0, str(Path(__file__).parent / "campelmag"))

from campelmag.campmag import CampelMag

np.seterr(divide="ignore", invalid="ignore")  # desabilitar no debug

cm = CampelMag()

print("\n#### CALCULO DOS CAMPOS ELÉTRICO E MAGNÉTICO ####")

# Tensão da Linha (V)
V = 765e3

# Potência Transmitida (W)
P = 1.2e9
Ir = P / V

# Angulos das fases (graus)
AngA = 120
AngB = 0
AngC = -120

AngulosABC = [AngA, AngB, AngC]

# Corrente com a defasagem
I = cm.Iri(Ir, AngulosABC)

# Caracteristicas Mecânicas
LinhaDeMedicao = [-80, 80, 0.5, 1.5]  # [Xmin Xmax Passo Altura]
Px = np.arange(
    LinhaDeMedicao[0], LinhaDeMedicao[1] + LinhaDeMedicao[2], LinhaDeMedicao[2]
)
Hmax = 25.0  # [m]
FLECHA = 8.63  # [m]
EspacSubCond = 0.457  # [m]
Dcabo = 0.02959
Ncond = 4.0
h0 = Hmax - FLECHA - EspacSubCond / 2.0
H = [h0, h0, h0]  # Altura mínima do cabo no feixe
D = [-14.34, 0, 14.34]

print(f"\nV = {V/1e3:5,.2f} [kV]")
print(f"H = [{H[0]:5,.2f}, {H[1]:5,.2f}, {H[2]:5,.2f}] [m]")
print(f"D = [{D[0]:5,.2f}, {D[1]:5,.2f}, {D[2]:5,.2f}] [m]")
print(
    f"Perfil de medição = [{LinhaDeMedicao[0]:5,.2f}, {LinhaDeMedicao[1]:5,.2f}, "
    f"{LinhaDeMedicao[2]:5,.2f}, {LinhaDeMedicao[3]:5,.2f}] [m]"
)


# Campo Elétrico
EkVm = cm.CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)

# Campo Magnético [SBr SBx SBh B]
B = cm.Brms(I, H, D, LinhaDeMedicao)

print(f"Campo Elétrico Máximo = {np.max(EkVm[:, 2]):5,.2f} [kV/m]")
print(f"Campo Magnético Máximo = {np.max(B[:, 3]):5,.2f} [µT]\n")

# Gráfico do Campo Elétrico
plt.figure(1)
plt.plot(Px, EkVm[:, 2], color="red", marker="None", label="E_max")
plt.title("Campo Elétrico × Perfil de medição")
plt.xlabel("Posição na faixa de passagem [m]")
plt.ylabel("Campo Elétrico [kV/m]")
plt.grid(True)
plt.legend(loc="upper right", fancybox=True)

# Gráfico do Campo Magnético
plt.figure(2)
plt.plot(Px, B[:, 3], color="blue", marker="None", label="B_mag")
plt.title("Campo Magnético × Perfil de medição")
plt.xlabel("Posição na faixa de passagem [m]")
plt.ylabel(r"Campo Magnético [$\mu$T]")
plt.grid(True, which="both", ls="-")
plt.legend(loc="upper right", fancybox=True)

plt.show()
