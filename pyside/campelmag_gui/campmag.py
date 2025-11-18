"""Classe para cálculo de campos elétrico e magnético.

Funções para plotar o perfil de campo elétrico e magnético
EPRI AC - Transmission Line Reference Book - 200kV and above
Capítulo 7 - Electric and Magnetic Fields
Autor: Alan Franco
03/08/2016
"""

from typing import Sequence, Union

import numpy as np
from numpy.typing import NDArray
from scipy.constants import epsilon_0, pi


class CampelMag:
    """Classe para cálculo de campos elétrico e magnético em linhas de transmissão."""

    def __init__(self) -> None:
        self.__r3: float = 1.7320508075688772  # raiz(3)
        self.__zero: float = 0.0

    def Vri(self, V: float, Angulos: Sequence[Union[int, float]]) -> NDArray[np.complex128]:
        """Retorna a Matriz com as tensões (3,2).

        Args:
                V: Tensão de fase [V]
                Angulos: Ângulos de defasagem das fases [A, B, C] em graus

        Returns:
                Matriz (3,2) com as tensões de cada fase [real, imaginário]
        """
        saida = np.zeros((3, 2))
        for i, angulo in enumerate(Angulos):
            ang_rad = np.deg2rad(angulo)
            saida[i, :] = (np.cos(ang_rad) * V / self.__r3, np.sin(ang_rad) * V / self.__r3)
        return saida

    def Skll(
        self, D: Sequence[Union[int, float]], H: Sequence[Union[int, float]]
    ) -> NDArray[np.float64]:
        """Calcula a distância entre o condutor k e a imagem do condutor l (3,3).

        Args:
                D: Posição relativa das fases [m]
                H: Altura relativa das fases [m]

        Returns:
                Matriz (3,3) com as distâncias
        """
        saida = np.zeros((3, 3))
        saida[0, :] = (
            2 * H[0],
            np.sqrt((D[1] - D[0]) ** 2 + (2 * H[1]) ** 2),
            np.sqrt((D[2] - D[0]) ** 2 + (2 * H[2]) ** 2),
        )
        saida[1, :] = (
            np.sqrt((D[0] - D[1]) ** 2 + (2 * H[0]) ** 2),
            2 * H[1],
            np.sqrt((D[2] - D[1]) ** 2 + (2 * H[2]) ** 2),
        )
        saida[2, :] = (
            np.sqrt((D[0] - D[2]) ** 2 + (2 * H[0]) ** 2),
            np.sqrt((D[1] - D[2]) ** 2 + (2 * H[1]) ** 2),
            2 * H[2],
        )
        return saida

    def Skl(
        self, D: Sequence[Union[int, float]], H: Sequence[Union[int, float]]
    ) -> NDArray[np.float64]:
        """Calcula a distância entre os condutores k e l (3,3).

        Args:
                D: Posição relativa das fases [m]
                H: Altura relativa das fases [m]

        Returns:
                Matriz (3,3) com as distâncias entre condutores
        """
        saida = np.zeros((3, 3))
        saida[0, :] = (
            self.__zero,
            np.sqrt((D[1] - D[0]) ** 2 + (H[1] - H[0]) ** 2),
            np.sqrt((D[2] - D[0]) ** 2 + (H[2] - H[0]) ** 2),
        )
        saida[1, :] = (
            np.sqrt((D[0] - D[1]) ** 2 + (H[0] - H[1]) ** 2),
            self.__zero,
            np.sqrt((D[2] - D[1]) ** 2 + (H[2] - H[1]) ** 2),
        )
        saida[2, :] = (
            np.sqrt((D[0] - D[2]) ** 2 + (H[0] - H[2]) ** 2),
            np.sqrt((D[1] - D[2]) ** 2 + (H[1] - H[2]) ** 2),
            self.__zero,
        )
        return saida

    def Pkl(self, proprio: NDArray[np.float64], mutuo: NDArray[np.float64]) -> NDArray[np.float64]:
        """Calcula o coeficiente de potencial mútuo entre os condutores k e l (3,3).

        Args:
                proprio: Distância entre o condutor k e a imagem do condutor l
                mutuo: Distância entre o condutor k e l

        Returns:
                Matriz (3,3) com os coeficientes de potencial mútuo
        """
        return (1.0 / (2 * pi * epsilon_0)) * np.log(proprio / mutuo)

    def Pkk(self, H: Sequence[Union[int, float]], deq: float) -> NDArray[np.float64]:
        """Calcula o coeficiente de potencial próprio do condutor k.

        Args:
                H: Altura do condutor [m]
                deq: Diâmetro equivalente do condutor [m]

        Returns:
                Array com os coeficientes de potencial próprio
        """
        return (1.0 / (2.0 * pi * epsilon_0)) * np.log(np.multiply(4.0, H) / deq)

    def Iri(self, I: float, Angulos: Sequence[Union[int, float]]) -> NDArray[np.complex128]:
        """Retorna a Matriz com as correntes (3,1) [real, imag].

        Args:
                I: Corrente por fase (circuito equilibrado) [A]
                Angulos: Ângulos de defasagem das fases [A, B, C] em graus

        Returns:
                Matriz (3,1) com as correntes complexas
        """
        saida = np.zeros((3, 1), dtype=np.complex128)
        for i, angulo in enumerate(Angulos):
            ang_rad = np.deg2rad(angulo)
            saida[i] = np.complex128(
                np.cos(ang_rad) * I / self.__r3 + 1j * np.sin(ang_rad) * I / self.__r3
            )
        return saida

    def deq(self, db: float, D: float, N: float) -> float:
        """Calcula o feixe equivalente.

        Args:
                db: Diâmetro do feixe [m]
                D: Diâmetro do cabo [m]
                N: Número de condutores

        Returns:
                Diâmetro equivalente do feixe [m]
        """
        return db * ((N * D / db) ** (1 / N))

    def db(self, S: float, N: float) -> float:
        """Calcula o diâmetro do feixe.

        Args:
                S: Espaçamento entre os condutores [m]
                N: Número de condutores

        Returns:
                Diâmetro do feixe [m]
        """
        return S / np.sin(pi / N)

    def ProfImg(self, R: float, P: float, F: float) -> float:
        """Retorna a profundidade da imagem do condutor para o campo Magnético.

        Args:
                R: Resistividade ρ do solo [Ω]
                P: Permeabilidade μ do solo [H/m] - Típico = 4π 10^-7 H/m
                F: Frequência da rede [Hz]

        Returns:
                Profundidade em metros [Δ]
        """
        return np.sqrt(R / (pi * F * P))

    def MatrizP(self, Mp: NDArray[np.float64], Mm: NDArray[np.float64]) -> NDArray[np.float64]:
        """Retorna a Matriz de Coeficientes de Potenciais de Maxwell (3,3).

        Args:
                Mp: Matriz própria de coeficientes
                Mm: Matriz mútua de coeficientes

        Returns:
                Matriz (3,3) de coeficientes de potenciais
        """
        M = Mm.copy()
        M[0, 0] = Mp[0]
        M[1, 1] = Mp[1]
        M[2, 2] = Mp[2]
        return M

    def CoronaVc(self, M: float, Delta: float, Dcond: float, DistCond: float) -> float:
        """Calcula a tensão crítica de pico (Equação de Peek).

        Args:
                M: Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
                Delta: Coeficiente que depende da altura e altitude
                Dcond: Diâmetro do condutor [m]
                DistCond: Distância entre condutores [m]

        Returns:
                Tensão crítica [kV]
        """
        A0 = 2.43 * M * Delta * Dcond * 1000
        A1 = np.log10((2 * DistCond * 1000) / (Dcond * 1000))
        return A0 * A1

    def CoronaDelta(self, H: float, t: float) -> float:
        """Calcula o delta crítico.

        Args:
                H: Altitude da instalação [m]
                t: Temperatura média anual [°C]

        Returns:
                Delta crítico
        """
        return (0.386 * (760 - 0.086 * H)) / (273 + t)

    def CoronaPerdas(
        self, Delta: float, F: float, Dcond: float, DistCond: float, V: float, Vc: float
    ) -> float:
        """Calcula as perdas devido ao Efeito Corona.

        Args:
                Delta: Delta Crítico
                F: Frequência da rede [Hz]
                Dcond: Diâmetro do condutor [m]
                DistCond: Distância entre os condutores [m]
                V: Tensão da rede pico-a-pico [V]
                Vc: Tensão Crítica da linha [V]

        Returns:
                Perdas [kW/km]
        """
        A0 = (3.44 / Delta) * F
        A1 = np.sqrt((Dcond * 1000) / (2 * (DistCond * 1000)))
        A2 = (V - Vc) * (V - Vc) * 0.001
        return A0 * A1 * A2

    def CoronaEc(self, M: float, Delta: float, Dcond: float) -> float:
        """Calcula o campo elétrico crítico.

        Args:
                M: Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
                Delta: Delta Crítico
                Dcond: Diâmetro do condutor [m]

        Returns:
                Campo elétrico crítico [kV/cm]
        """
        A0 = M * (21.1e3) * Delta
        A1 = 1 + (np.sqrt(1 / 0.301) / (np.sqrt(Delta * Dcond * 1000)))
        return A0 * A1

    def CalcQ(
        self,
        H: Sequence[Union[int, float]],
        D: Sequence[Union[int, float]],
        EspacSubCond: float,
        Ncond: float,
        Dcabo: float,
        V: float,
        AngulosABC: Sequence[Union[int, float]],
    ) -> NDArray[np.float64]:
        """Calcula a matriz de distribuição de cargas.

        Args:
                H: Altura dos condutores [HA, HB, HC] [m]
                D: Distância entre os condutores [DA, DB, DC] [m]
                EspacSubCond: Espaçamento entre os subcondutores [m]
                Ncond: Número de condutores por feixe
                Dcabo: Diâmetro do cabo do feixe [m]
                V: Tensão da linha [V]
                AngulosABC: Defasagem angular entre as fases [AA, AB, AC] [graus]

        Returns:
                Matriz de distribuição de cargas Q
        """
        DB = self.db(EspacSubCond, Ncond)
        DEQ = self.deq(DB, Dcabo, Ncond)

        Mp = self.Pkk(H, DEQ)  # Matriz própria
        Mm = self.Pkl(self.Skll(D, H), self.Skl(D, H))  # Matriz mútua

        P = self.MatrizP(Mp, Mm)
        C = np.linalg.inv(P)
        V_matrix = self.Vri(V, AngulosABC)

        return np.dot(C, V_matrix)  # Q = C @ V

    def Erms(
        self,
        LinhaDeMed: Sequence[Union[int, float]],
        H: Sequence[Union[int, float]],
        D: Sequence[Union[int, float]],
        Q: NDArray[np.float64],
    ) -> NDArray[np.float64]:
        """Retorna um vetor com os campos elétricos correspondentes aos pontos [SEx, SEy, Erms].

        Args:
                LinhaDeMed: Características da linha de medição [-X, X, Passo, Altura]
                H: Posições verticais dos cabos [A, B, C] [m]
                D: Posições horizontais relativas dos cabos [A, 0, C] [m]
                Q: Matriz com as cargas

        Returns:
                Matriz (N, 3) com [SEx, SEy, Erms]
        """
        Px = np.arange(LinhaDeMed[0], LinhaDeMed[1] + LinhaDeMed[2], LinhaDeMed[2])
        Ex = np.zeros((Px.size, 3), dtype=np.complex128)
        Ey = np.zeros((Px.size, 3), dtype=np.complex128)
        Hm = LinhaDeMed[3]  # Altura do ponto de medição

        for i in range(3):
            Xm = Px - D[i]  # Distância entre o ponto de medição e a linha
            Hk = H[i]  # Altura da linha

            C = np.complex128(Q[i, 0] + 1j * Q[i, 1]) / (2 * pi * epsilon_0)
            T1x = Xm / (Xm**2 + (Hk - Hm) ** 2)
            T2x = Xm / (Xm**2 + (Hk + Hm) ** 2)

            T1y = (Hm - Hk) / (Xm**2 + (Hk - Hm) ** 2)
            T2y = (Hm + Hk) / (Xm**2 + (Hk + Hm) ** 2)

            Ex[:, i] = C * (T1x - T2x)
            Ey[:, i] = C * (T1y - T2y)

        # Soma as contribuições de cada Linha
        saida = np.zeros((Px.size, 3))
        saida[:, 0] = np.real(np.sum(Ex, 1))  # SEx
        saida[:, 1] = np.real(np.sum(Ey, 1))  # SEy
        SEx = saida[:, 0]
        SEy = saida[:, 1]
        saida[:, 2] = np.sqrt(SEx**2 + SEy**2)  # Erms

        return saida

    def CalcEkv(
        self,
        H: Sequence[Union[int, float]],
        D: Sequence[Union[int, float]],
        EspacSubCond: float,
        Ncond: float,
        Dcabo: float,
        V: float,
        AngulosABC: Sequence[Union[int, float]],
        LinhaDeMedicao: Sequence[Union[int, float]],
    ) -> NDArray[np.float64]:
        """Calcula o campo elétrico medido em um perfil.

        Args:
                H: Altura dos condutores [HA, HB, HC] [m]
                D: Distância entre os condutores [DA, DB, DC] [m]
                EspacSubCond: Espaçamento entre os subcondutores [m]
                Ncond: Número de condutores por feixe
                Dcabo: Diâmetro do cabo do feixe [m]
                V: Tensão da linha [V]
                AngulosABC: Defasagem angular entre as fases [AA, AB, AC] [graus]
                LinhaDeMedicao: Perfil de medição [Xmin, Xmax, Passo, Altura] [m]

        Returns:
                Matriz com os campos elétricos em [kV/m]
        """
        DB = self.db(EspacSubCond, Ncond)
        DEQ = self.deq(DB, Dcabo, Ncond)

        Mp = self.Pkk(H, DEQ)  # Matriz própria
        Mm = self.Pkl(self.Skll(D, H), self.Skl(D, H))  # Matriz mútua

        P = self.MatrizP(Mp, Mm)
        C = np.linalg.inv(P)
        V_matrix = self.Vri(V, AngulosABC)

        Q = np.dot(C, V_matrix)  # C @ V
        saida = self.Erms(LinhaDeMedicao, H, D, Q)

        return saida / 1e3

    def Brms(
        self,
        I: NDArray[np.complex128],
        H: Sequence[Union[int, float]],
        D: Sequence[Union[int, float]],
        P: Sequence[Union[int, float]],
    ) -> NDArray[np.float64]:
        """Retorna um vetor com o campo magnético resultante nos pontos da faixa de passagem.

        Args:
                I: Corrente que passa pela linha [IA, IB, IC] [A]
                H: Posições verticais dos cabos [A, B, C] [m]
                D: Posições horizontais relativas dos cabos [A, 0, C] [m]
                P: Características da linha de medição [-X, X, Passo, Altura]

        Returns:
                Matriz (N, 4) com [SBr, SBx, SBh, B] em micro Tesla (µT)
        """
        Xm = np.arange(P[0], P[1] + P[2], P[2])
        Hm = P[3]
        Bkr = np.zeros((Xm.size, 3), dtype=np.complex128)
        Bkx = np.zeros((Xm.size, 3), dtype=np.complex128)
        Bkh = np.zeros((Xm.size, 3), dtype=np.complex128)

        for i in range(3):
            # Resultado em Tesla
            dist_sq = (Xm - D[i]) ** 2 + (Hm - H[i]) ** 2
            dist = np.sqrt(dist_sq)
            Bkr[:, i] = (2e-7 * I[i]) / dist
            Bkx[:, i] = (2e-7 * I[i] * (Xm - D[i])) / dist
            Bkh[:, i] = (2e-7 * I[i] * (Hm - H[i])) / dist

        SBr = np.sum(Bkr, 1)
        SBx = np.sum(Bkx, 1)
        SBh = np.sum(Bkh, 1)
        Bmag = np.sqrt(np.real(SBx) ** 2 + np.real(SBh) ** 2)

        # Converte para micro Tesla (1e6)
        saida = np.zeros((Xm.size, 4))
        saida[:, 0] = np.real(SBr)
        saida[:, 1] = np.real(SBx)
        saida[:, 2] = np.real(SBh)
        saida[:, 3] = np.real(Bmag)

        return saida * 1e6
