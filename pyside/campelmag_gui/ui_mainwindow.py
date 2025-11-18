"""Interface de usuário gerada para a janela principal."""

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PySide6.QtCore import QRect, QSize, Qt
from PySide6.QtWidgets import (
    QFrame,
    QGridLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QMainWindow,
    QPushButton,
    QSizePolicy,
    QSpacerItem,
    QStatusBar,
    QVBoxLayout,
    QWidget,
)


class MplCanvas(FigureCanvas):
    """Canvas para gráficos matplotlib."""

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = self.fig.add_subplot(111)
        super().__init__(self.fig)
        self.setParent(parent)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.updateGeometry()


class Ui_MainWindow:
    """Classe de interface de usuário para a janela principal."""

    def setupUi(self, MainWindow):
        """Configurar a interface de usuário."""
        if not MainWindow.objectName():
            MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1392, 703)

        self.centralWidget = QWidget(MainWindow)
        self.centralWidget.setObjectName("centralWidget")

        self.gridLayout = QGridLayout(self.centralWidget)
        self.gridLayout.setObjectName("gridLayout")

        # Frame esquerdo com inputs
        self.frame = QFrame(self.centralWidget)
        self.frame.setObjectName("frame")
        self.frame.setMinimumSize(QSize(560, 610))
        self.frame.setMaximumSize(QSize(560, 16777215))
        self.frame.setFrameShape(QFrame.StyledPanel)
        self.frame.setFrameShadow(QFrame.Raised)

        # Layout principal do frame
        self.frameLayout = QVBoxLayout(self.frame)

        # Parâmetros principais
        self.paramsLayout = QHBoxLayout()

        # Coluna esquerda
        self.leftParamsLayout = QVBoxLayout()

        # Tensão
        self.tensaoLayout = QHBoxLayout()
        self.labelTensao = QLabel("Tensão [V]:")
        self.labelTensao.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelTensao.setMaximumWidth(120)
        self.lineEditTensao = QLineEdit()
        self.lineEditTensao.setMaximumWidth(80)
        self.tensaoLayout.addWidget(self.labelTensao)
        self.tensaoLayout.addWidget(self.lineEditTensao)
        self.leftParamsLayout.addLayout(self.tensaoLayout)

        # Potência
        self.potLayout = QHBoxLayout()
        self.labelPot = QLabel("Potência [W]:")
        self.labelPot.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelPot.setMaximumWidth(120)
        self.lineEditPot = QLineEdit()
        self.lineEditPot.setMaximumWidth(80)
        self.potLayout.addWidget(self.labelPot)
        self.potLayout.addWidget(self.lineEditPot)
        self.leftParamsLayout.addLayout(self.potLayout)

        # Coeficiente
        self.coefLayout = QHBoxLayout()
        self.labelCoef = QLabel("Coef. Med:")
        self.labelCoef.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelCoef.setMaximumWidth(120)
        self.lineEditCoef = QLineEdit()
        self.lineEditCoef.setMaximumWidth(80)
        self.coefLayout.addWidget(self.labelCoef)
        self.coefLayout.addWidget(self.lineEditCoef)
        self.leftParamsLayout.addLayout(self.coefLayout)

        # Altura máxima
        self.hmaxLayout = QHBoxLayout()
        self.labelHmax = QLabel("Altura max [m]:")
        self.labelHmax.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelHmax.setMaximumWidth(120)
        self.lineEditHmax = QLineEdit()
        self.lineEditHmax.setMaximumWidth(80)
        self.hmaxLayout.addWidget(self.labelHmax)
        self.hmaxLayout.addWidget(self.lineEditHmax)
        self.leftParamsLayout.addLayout(self.hmaxLayout)

        self.paramsLayout.addLayout(self.leftParamsLayout)

        # Spacer
        self.paramsLayout.addItem(QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum))

        # Coluna direita
        self.rightParamsLayout = QVBoxLayout()

        # Flecha
        self.flechaLayout = QHBoxLayout()
        self.labelFlecha = QLabel("Flecha [m]:")
        self.labelFlecha.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelFlecha.setMaximumWidth(200)
        self.lineEditFlecha = QLineEdit()
        self.lineEditFlecha.setMaximumWidth(80)
        self.flechaLayout.addWidget(self.labelFlecha)
        self.flechaLayout.addWidget(self.lineEditFlecha)
        self.rightParamsLayout.addLayout(self.flechaLayout)

        # Espaçamento
        self.espacLayout = QHBoxLayout()
        self.labelEspac = QLabel("Espaçamento [m]:")
        self.labelEspac.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelEspac.setMaximumWidth(200)
        self.lineEditEspacamento = QLineEdit()
        self.lineEditEspacamento.setMaximumWidth(80)
        self.espacLayout.addWidget(self.labelEspac)
        self.espacLayout.addWidget(self.lineEditEspacamento)
        self.rightParamsLayout.addLayout(self.espacLayout)

        # Diâmetro
        self.diamLayout = QHBoxLayout()
        self.labelDiam = QLabel("Diametro do cabo [m]:")
        self.labelDiam.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelDiam.setMaximumWidth(200)
        self.lineEditDiametroCabos = QLineEdit()
        self.lineEditDiametroCabos.setMaximumWidth(80)
        self.diamLayout.addWidget(self.labelDiam)
        self.diamLayout.addWidget(self.lineEditDiametroCabos)
        self.rightParamsLayout.addLayout(self.diamLayout)

        # N condutores
        self.ncondLayout = QHBoxLayout()
        self.labelNcond = QLabel("Número de condutores:")
        self.labelNcond.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.labelNcond.setMaximumWidth(200)
        self.lineEditNcondutores = QLineEdit()
        self.lineEditNcondutores.setMaximumWidth(80)
        self.ncondLayout.addWidget(self.labelNcond)
        self.ncondLayout.addWidget(self.lineEditNcondutores)
        self.rightParamsLayout.addLayout(self.ncondLayout)

        self.paramsLayout.addLayout(self.rightParamsLayout)
        self.frameLayout.addLayout(self.paramsLayout)

        # Faixa de medição
        self.groupBoxMed = QGroupBox("Faixa de medição [m]")
        self.groupBoxMed.setAlignment(Qt.AlignCenter)
        self.groupBoxMed.setMaximumHeight(110)

        self.medLayout = QVBoxLayout(self.groupBoxMed)

        self.medLabelsLayout = QHBoxLayout()
        self.labelXmax = QLabel("X máx")
        self.labelXmax.setAlignment(Qt.AlignCenter)
        self.labelXmax.setMaximumWidth(100)
        self.labelXmin = QLabel("X mín")
        self.labelXmin.setAlignment(Qt.AlignCenter)
        self.labelXmin.setMaximumWidth(100)
        self.labelPasso = QLabel("Passo")
        self.labelPasso.setAlignment(Qt.AlignCenter)
        self.labelPasso.setMaximumWidth(100)
        self.labelHsolo = QLabel("H solo")
        self.labelHsolo.setAlignment(Qt.AlignCenter)
        self.labelHsolo.setMaximumWidth(100)

        self.medLabelsLayout.addWidget(self.labelXmax)
        self.medLabelsLayout.addWidget(self.labelXmin)
        self.medLabelsLayout.addWidget(self.labelPasso)
        self.medLabelsLayout.addWidget(self.labelHsolo)

        self.medInputsLayout = QHBoxLayout()
        self.lineEditXmax = QLineEdit()
        self.lineEditXmax.setMaximumWidth(100)
        self.lineEditXmin = QLineEdit()
        self.lineEditXmin.setMaximumWidth(100)
        self.lineEditPasso = QLineEdit()
        self.lineEditPasso.setMaximumWidth(100)
        self.lineEditHsolo = QLineEdit()
        self.lineEditHsolo.setMaximumWidth(100)

        self.medInputsLayout.addWidget(self.lineEditXmax)
        self.medInputsLayout.addWidget(self.lineEditXmin)
        self.medInputsLayout.addWidget(self.lineEditPasso)
        self.medInputsLayout.addWidget(self.lineEditHsolo)

        self.medLayout.addLayout(self.medLabelsLayout)
        self.medLayout.addLayout(self.medInputsLayout)

        self.frameLayout.addWidget(self.groupBoxMed)

        # Posição dos feixes
        self.groupBoxFeixes = QGroupBox("Posição dos feixes [m]")
        self.groupBoxFeixes.setAlignment(Qt.AlignCenter)
        self.groupBoxFeixes.setMaximumHeight(110)

        self.feixesLayout = QVBoxLayout(self.groupBoxFeixes)

        self.feixesLabelsLayout = QHBoxLayout()
        self.labelFeixeA = QLabel("Fase Ax")
        self.labelFeixeA.setAlignment(Qt.AlignCenter)
        self.labelFeixeA.setMaximumWidth(100)
        self.labelFeixeB = QLabel("Fase Bx")
        self.labelFeixeB.setAlignment(Qt.AlignCenter)
        self.labelFeixeB.setMaximumWidth(100)
        self.labelFeixeC = QLabel("Fase Cx")
        self.labelFeixeC.setAlignment(Qt.AlignCenter)
        self.labelFeixeC.setMaximumWidth(100)

        self.feixesLabelsLayout.addWidget(self.labelFeixeA)
        self.feixesLabelsLayout.addWidget(self.labelFeixeB)
        self.feixesLabelsLayout.addWidget(self.labelFeixeC)

        self.feixesInputsLayout = QHBoxLayout()
        self.lineEditFeixeA = QLineEdit()
        self.lineEditFeixeA.setMaximumWidth(100)
        self.lineEditFeixeB = QLineEdit()
        self.lineEditFeixeB.setMaximumWidth(100)
        self.lineEditFeixeC = QLineEdit()
        self.lineEditFeixeC.setMaximumWidth(100)

        self.feixesInputsLayout.addWidget(self.lineEditFeixeA)
        self.feixesInputsLayout.addWidget(self.lineEditFeixeB)
        self.feixesInputsLayout.addWidget(self.lineEditFeixeC)

        self.feixesLayout.addLayout(self.feixesLabelsLayout)
        self.feixesLayout.addLayout(self.feixesInputsLayout)

        self.frameLayout.addWidget(self.groupBoxFeixes)

        # Ângulos de fase
        self.groupBoxAngulos = QGroupBox("Ângulos de fase [graus]")
        self.groupBoxAngulos.setAlignment(Qt.AlignCenter)
        self.groupBoxAngulos.setMaximumHeight(110)

        self.angulosLayout = QVBoxLayout(self.groupBoxAngulos)

        self.angulosLabelsLayout = QHBoxLayout()
        self.labelFaseA = QLabel("Fase A")
        self.labelFaseA.setAlignment(Qt.AlignCenter)
        self.labelFaseA.setMaximumWidth(100)
        self.labelFaseB = QLabel("Fase B")
        self.labelFaseB.setAlignment(Qt.AlignCenter)
        self.labelFaseB.setMaximumWidth(100)
        self.labelFaseC = QLabel("Fase C")
        self.labelFaseC.setAlignment(Qt.AlignCenter)
        self.labelFaseC.setMaximumWidth(100)

        self.angulosLabelsLayout.addWidget(self.labelFaseA)
        self.angulosLabelsLayout.addWidget(self.labelFaseB)
        self.angulosLabelsLayout.addWidget(self.labelFaseC)

        self.angulosInputsLayout = QHBoxLayout()
        self.lineEditFasea = QLineEdit()
        self.lineEditFasea.setMaximumWidth(100)
        self.lineEditFaseb = QLineEdit()
        self.lineEditFaseb.setMaximumWidth(100)
        self.lineEditFasec = QLineEdit()
        self.lineEditFasec.setMaximumWidth(100)

        self.angulosInputsLayout.addWidget(self.lineEditFasea)
        self.angulosInputsLayout.addWidget(self.lineEditFaseb)
        self.angulosInputsLayout.addWidget(self.lineEditFasec)

        self.angulosLayout.addLayout(self.angulosLabelsLayout)
        self.angulosLayout.addLayout(self.angulosInputsLayout)

        self.frameLayout.addWidget(self.groupBoxAngulos)

        # Botões
        self.buttonsLayout = QHBoxLayout()
        self.pushButtonCalcular = QPushButton("Calcular")
        self.pushButtonCalcular.setMaximumHeight(30)
        self.pushButtonReset = QPushButton("Reset")
        self.pushButtonReset.setMaximumHeight(30)
        self.pushButtonLimpar = QPushButton("Limpar")
        self.pushButtonLimpar.setMaximumHeight(30)
        self.pushButtonSair = QPushButton("Sair")
        self.pushButtonSair.setMaximumHeight(30)

        self.buttonsLayout.addWidget(self.pushButtonCalcular)
        self.buttonsLayout.addWidget(self.pushButtonReset)
        self.buttonsLayout.addWidget(self.pushButtonLimpar)
        self.buttonsLayout.addWidget(self.pushButtonSair)

        self.frameLayout.addLayout(self.buttonsLayout)
        self.frameLayout.addStretch()

        self.gridLayout.addWidget(self.frame, 0, 0)

        # Gráficos
        self.plotsLayout = QHBoxLayout()

        self.plotxE = MplCanvas(self.centralWidget, width=5, height=4, dpi=100)
        self.plotxE.setMinimumSize(QSize(400, 610))

        self.plotxB = MplCanvas(self.centralWidget, width=5, height=4, dpi=100)
        self.plotxB.setMinimumSize(QSize(400, 610))

        self.plotsLayout.addWidget(self.plotxE)
        self.plotsLayout.addWidget(self.plotxB)

        self.gridLayout.addLayout(self.plotsLayout, 0, 1)

        MainWindow.setCentralWidget(self.centralWidget)

        # Status bar
        self.statusBar = QStatusBar(MainWindow)
        self.statusBar.setObjectName("statusBar")
        MainWindow.setStatusBar(self.statusBar)

        self.retranslateUi(MainWindow)

    def retranslateUi(self, MainWindow):
        """Configurar textos da interface."""
        MainWindow.setWindowTitle("CampelMag")
