"""Exemplo de uso da aplicação CampelMag GUI."""

import numpy as np
from campelmag_gui.campmag import CampelMag


def exemplo_basico():
    """Exemplo básico de cálculo de campos."""
    # Criar instância
    cm = CampelMag()

    # Parâmetros da linha de 765 kV
    tensao = 765e3  # V
    potencia = 2.2e9  # W
    corrente = potencia / tensao

    # Altura e posição dos condutores
    hmax = 25.0  # m
    flecha = 8.63  # m
    espacamento = 0.457  # m
    hmin = hmax - flecha - (espacamento / 2.0)
    H = [hmin, hmin, hmin]

    # Posições horizontais das fases
    D = [-14.34, 0.0, 14.34]  # m

    # Ângulos de fase
    angulos = [120.0, 0.0, -120.0]  # graus

    # Parâmetros dos condutores
    diamCabo = 0.02959  # m
    nCondutores = 4

    # Linha de medição
    linha_med = [-80.0, 80.0, 0.5, 1.5]  # [xmin, xmax, passo, altura]

    # Calcular campo elétrico
    E_result = cm.CalcEkv(H, D, espacamento, nCondutores, diamCabo, tensao, angulos, linha_med)

    # Calcular correntes
    I = cm.Iri(corrente, angulos)

    # Calcular campo magnético
    B_result = cm.Brms(I, H, D, linha_med)

    # Resultados
    print("=" * 60)
    print("Exemplo de Cálculo - Linha de 765 kV")
    print("=" * 60)
    print(f"Tensão: {tensao/1e3:.0f} kV")
    print(f"Potência: {potencia/1e9:.1f} GW")
    print(f"Corrente: {corrente:.1f} A")
    print(f"Altura mínima dos condutores: {hmin:.2f} m")
    print("=" * 60)
    print(f"Campo Elétrico máximo: {np.max(E_result[:, 2]):.2f} kV/m")
    print(f"Campo Magnético máximo: {np.max(B_result[:, 3]):.2f} µT")
    print("=" * 60)


if __name__ == "__main__":
    exemplo_basico()
