"""Janela principal da aplicação CampelMag."""

import numpy as np
from PySide6.QtCore import Qt
from PySide6.QtWidgets import QLineEdit, QMainWindow

from . import __version__
from .campmag import CampelMag
from .ui_mainwindow import Ui_MainWindow


class MainWindow(QMainWindow):
    """Janela principal da aplicação."""

    def __init__(self, parent=None):
        """Inicializar a janela principal."""
        super().__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # Define título com a versão do pacote
        self.setWindowTitle(f"CampelMag - v{__version__}")

        # Instância da classe de cálculo
        self.campelmag = CampelMag()

        # Dados de entrada padrão
        self.tensao = 765e3
        self.potencia = 2.2e9
        self.coef = 0.87
        self.hmax = 25.0
        self.flecha = 8.63
        self.corrente = self.potencia / self.tensao
        self.espacamento = 0.457
        self.diamCabo = 0.02959
        self.nCondutores = 4
        self.hmin = self.hmax - self.flecha - (self.espacamento / 2.0)

        self.xmin = -80.0
        self.xmax = 80.0
        self.passo = 0.5
        self.hsolo = 1.5

        self.posFaseA = -14.34
        self.posFaseB = 0.0
        self.posFaseC = 14.34

        self.angFaseA = 120.0
        self.angFaseB = 0.0
        self.angFaseC = -120.0

        # Conectar sinais
        self._connect_signals()

        # Configurar dados iniciais
        self.set_data()

    def _connect_signals(self):
        """Conectar sinais dos botões e campos."""
        # Botões
        self.ui.pushButtonCalcular.clicked.connect(self.calcula)
        self.ui.pushButtonReset.clicked.connect(self.reset_data)
        self.ui.pushButtonLimpar.clicked.connect(self.clear_data)
        self.ui.pushButtonSair.clicked.connect(self.close)

        # Return pressed em todos os LineEdit
        for line_edit in self.findChildren(QLineEdit):
            line_edit.returnPressed.connect(self.calcula)

    def get_data(self):
        """Obter dados dos campos de entrada."""
        try:
            data = {
                "tensao": float(self.ui.lineEditTensao.text()),
                "potencia": float(self.ui.lineEditPot.text()),
                "coef": float(self.ui.lineEditCoef.text()),
                "hmax": float(self.ui.lineEditHmax.text()),
                "flecha": float(self.ui.lineEditFlecha.text()),
                "espacamento": float(self.ui.lineEditEspacamento.text()),
                "diamCabo": float(self.ui.lineEditDiametroCabos.text()),
                "nCondutores": float(self.ui.lineEditNcondutores.text()),
                "xmin": float(self.ui.lineEditXmin.text()),
                "xmax": float(self.ui.lineEditXmax.text()),
                "passo": float(self.ui.lineEditPasso.text()),
                "hsolo": float(self.ui.lineEditHsolo.text()),
                "posFaseA": float(self.ui.lineEditFeixeA.text()),
                "posFaseB": float(self.ui.lineEditFeixeB.text()),
                "posFaseC": float(self.ui.lineEditFeixeC.text()),
                "angFaseA": float(self.ui.lineEditFasea.text()),
                "angFaseB": float(self.ui.lineEditFaseb.text()),
                "angFaseC": float(self.ui.lineEditFasec.text()),
            }
            data["corrente"] = data["potencia"] / data["tensao"]
            data["hmin"] = data["hmax"] - data["flecha"] - (data["espacamento"] / 2.0)
            return data
        except ValueError as e:
            self.ui.statusBar.showMessage(f"Erro ao ler dados: {e}")
            return None

    def set_data(self):
        """Configurar dados iniciais nos campos."""
        self.ui.lineEditTensao.setText(f"{self.tensao:.2e}")
        self.ui.lineEditPot.setText(f"{self.potencia:.2e}")
        self.ui.lineEditCoef.setText(str(self.coef))
        self.ui.lineEditHmax.setText(str(self.hmax))
        self.ui.lineEditFlecha.setText(str(self.flecha))
        self.ui.lineEditEspacamento.setText(str(self.espacamento))
        self.ui.lineEditDiametroCabos.setText(str(self.diamCabo))
        self.ui.lineEditNcondutores.setText(str(int(self.nCondutores)))
        self.ui.lineEditXmin.setText(str(self.xmin))
        self.ui.lineEditXmax.setText(str(self.xmax))
        self.ui.lineEditPasso.setText(str(self.passo))
        self.ui.lineEditHsolo.setText(str(self.hsolo))
        self.ui.lineEditFeixeA.setText(str(self.posFaseA))
        self.ui.lineEditFeixeB.setText(str(self.posFaseB))
        self.ui.lineEditFeixeC.setText(str(self.posFaseC))
        self.ui.lineEditFasea.setText(str(self.angFaseA))
        self.ui.lineEditFaseb.setText(str(self.angFaseB))
        self.ui.lineEditFasec.setText(str(self.angFaseC))

        # Habilitar campos
        for line_edit in self.findChildren(QLineEdit):
            line_edit.setEnabled(True)

        self.ui.pushButtonCalcular.setEnabled(True)

    def reset_data(self):
        """Resetar dados para valores padrão."""
        self.set_data()
        self.ui.statusBar.showMessage("Dados resetados")

    def clear_data(self):
        """Limpar todos os campos."""
        for line_edit in self.findChildren(QLineEdit):
            line_edit.clear()
            line_edit.setEnabled(False)

        self.ui.pushButtonCalcular.setEnabled(False)
        self.ui.statusBar.showMessage("Dados limpos")

    def calcula(self):
        """Calcular campos elétrico e magnético."""
        data = self.get_data()
        if data is None:
            return

        try:
            # Parâmetros de altura dos condutores
            H = [data["hmin"], data["hmin"], data["hmin"]]

            # Posições horizontais
            D = [data["posFaseA"], data["posFaseB"], data["posFaseC"]]

            # Ângulos de fase
            angulos = [data["angFaseA"], data["angFaseB"], data["angFaseC"]]

            # Linha de medição
            linha_med = [data["xmin"], data["xmax"], data["passo"], data["hsolo"]]

            # Calcular campo elétrico
            E_result = self.campelmag.CalcEkv(
                H,
                D,
                data["espacamento"],
                data["nCondutores"],
                data["diamCabo"],
                data["tensao"],
                angulos,
                linha_med,
            )

            # Calcular correntes
            I = self.campelmag.Iri(data["corrente"], angulos)

            # Calcular campo magnético
            B_result = self.campelmag.Brms(I, H, D, linha_med)

            # Gerar pontos X
            Px = np.arange(linha_med[0], linha_med[1] + linha_med[2], linha_med[2])

            # Plotar resultados
            self.plota_campo_eletrico(Px, E_result[:, 2])
            self.plota_campo_magnetico(Px, B_result[:, 3])

            # Mostrar máximos na barra de status
            ekvmax = np.max(E_result[:, 2])
            butmax = np.max(B_result[:, 3])

            msg = f"Campo Elétrico máximo: {ekvmax:.2f} [kV/m]           "
            msg += f"\t\t\t\t\t\t\t\t\t\tCampo Magnético máximo: {butmax:.2f} [µT]"
            self.ui.statusBar.showMessage(msg)

        except Exception as e:
            self.ui.statusBar.showMessage(f"Erro no cálculo: {e}")

    def plota_campo_eletrico(self, x, y):
        """Plotar campo elétrico."""
        ax = self.ui.plotxE.axes
        ax.clear()

        ax.plot(x, y, color="blue", linewidth=2)
        ax.fill_between(x, y, alpha=0.2, color="blue")

        ax.set_xlabel("Posição em baixo do vão [m]")
        ax.set_ylabel("Campo Elétrico [kV/m]")
        ax.grid(True, alpha=0.3)

        self.ui.plotxE.fig.tight_layout()
        self.ui.plotxE.draw()

    def plota_campo_magnetico(self, x, y):
        """Plotar campo magnético."""
        ax = self.ui.plotxB.axes
        ax.clear()

        ax.plot(x, y, color="red", linewidth=2)
        ax.fill_between(x, y, alpha=0.2, color="red")

        ax.set_xlabel("Posição em baixo do vão [m]")
        ax.set_ylabel("Campo Magnético [µT]")
        ax.grid(True, alpha=0.3)

        self.ui.plotxB.fig.tight_layout()
        self.ui.plotxB.draw()
