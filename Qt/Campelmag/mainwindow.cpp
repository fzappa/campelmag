#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <algorithm>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("CampelMag - v0.5");
    setData();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::getData(conf &lt)
{

    // Coleta os dados para os forms
    lt.tensao   = QVariant(ui->lineEditTensao->text()).toDouble();
    lt.potencia = QVariant(ui->lineEditPot->text()).toDouble();
    lt.coefm    = QVariant(ui->lineEditCoef->text()).toDouble();
    lt.hmax     = QVariant(ui->lineEditHmax->text()).toDouble();

    lt.flecha       = QVariant(ui->lineEditFlecha->text()).toDouble();
    lt.corrente     = lt.potencia / lt.tensao;
    lt.espacsubcond = QVariant(ui->lineEditEspacamento->text()).toDouble();
    lt.diamcabo     = QVariant(ui->lineEditDiametroCabos->text()).toDouble();
    lt.ncond        = QVariant(ui->lineEditNcondutores->text()).toDouble();

    lt.hmin = lt.hmax - lt.flecha - (lt.espacsubcond / 2.0);

    // Faixa de medicao
    lt.linhamed[0] = QVariant(ui->lineEditXmin->text()).toDouble();
    lt.linhamed[1] = QVariant(ui->lineEditXmax->text()).toDouble();
    lt.linhamed[2] = QVariant(ui->lineEditPasso->text()).toDouble();
    lt.linhamed[3] = QVariant(ui->lineEditHsolo->text()).toDouble();

    // Posicao das fases
    lt.pxfeixes[0] = QVariant(ui->lineEditFeixeA->text()).toDouble();
    lt.pxfeixes[1] = QVariant(ui->lineEditFeixeB->text()).toDouble();
    lt.pxfeixes[2] = QVariant(ui->lineEditFeixeC->text()).toDouble();

    // Angulos das fases
    lt.angabc[0] = QVariant(ui->lineEditFasea->text()).toDouble();
    lt.angabc[1] = QVariant(ui->lineEditFaseb->text()).toDouble();
    lt.angabc[2] = QVariant(ui->lineEditFasec->text()).toDouble();
}

void MainWindow::setData()
{

    // Coleta os dados
    ui->lineEditTensao->setText(QString::number(tensao, 'e', 2));
    ui->lineEditPot->setText(QString::number(potencia, 'e', 2));
    ui->lineEditCoef->setText(QVariant(coef).toString());
    ui->lineEditHmax->setText(QVariant(hmax).toString());

    ui->lineEditFlecha->setText(QVariant(flecha).toString());
    ui->lineEditEspacamento->setText(QVariant(espacamento).toString());
    ui->lineEditDiametroCabos->setText(QVariant(diamCabo).toString());
    ui->lineEditNcondutores->setText(QVariant(nCondutores).toString());

    // Faixa de medicao
    ui->lineEditXmin->setText(QVariant(xmin).toString());
    ui->lineEditXmax->setText(QVariant(xmax).toString());
    ui->lineEditPasso->setText(QVariant(passo).toString());
    ui->lineEditHsolo->setText(QVariant(hsolo).toString());

    // Posicao dos feixes
    ui->lineEditFeixeA->setText(QVariant(posFaseA).toString());
    ui->lineEditFeixeB->setText(QVariant(posFaseB).toString());
    ui->lineEditFeixeC->setText(QVariant(posFaseC).toString());

    // Angulo de fase
    ui->lineEditFasea->setText(QVariant(angFaseA).toString());
    ui->lineEditFaseb->setText(QVariant(angFaseB).toString());
    ui->lineEditFasec->setText(QVariant(angFaseC).toString());

    // Libera a edicao
    foreach (QLineEdit *le, findChildren<QLineEdit *>()) {
        le->setDisabled(false);
    }

    // Libera o calculo
    ui->pushButtonCalcular->setDisabled(false);
}

void MainWindow::clearData()
{

    foreach (QLineEdit *le, findChildren<QLineEdit *>()) {
        le->clear();
        le->setDisabled(true);
    }

    ui->pushButtonCalcular->setDisabled(true);
}

void MainWindow::calcula()
{

    // TODO Criar relatorio (implementar do prog orignal)
    // TODO Ler XML
    try {

        conf lt;

        // Leitura dos dados
        getData(lt);

        // Passando por referencia, para facilitar a leitura
        const double &minimo{((lt.linhamed[1] - lt.linhamed[0]) / lt.linhamed[2] + 1)};
        const double &passo{lt.linhamed[0]};
        const double &maximo{lt.linhamed[0] +
                             lt.linhamed[2] * ((lt.linhamed[1] - lt.linhamed[0]) / lt.linhamed[2])};

        // Pontos sobre o eixo X (comprimento)
        //        std::unique_ptr<Eigen::VectorXd> ptrPx{std::make_unique<Eigen::VectorXd>()};
        //        *ptrPx = Eigen::VectorXd::LinSpaced(long(minimo), passo, maximo);
        Eigen::VectorXd Px{Eigen::VectorXd::LinSpaced(long(minimo), passo, maximo)};

        // Calcula o campo elétrico
        std::unique_ptr<Eigen::MatrixXcd> ptrEkVm{std::make_unique<Eigen::MatrixXcd>()};
        ptrEkVm = CalcEkv(lt);

        // Campo Magnético [SBr SBx SBh B]
        std::unique_ptr<Eigen::MatrixXcd> ptrBrms{std::make_unique<Eigen::MatrixXcd>()};
        ptrBrms = Brms(lt);

        // ---- INICIO ---- Mapea as saidas do Eigen para plotar

        // Gera o vetox de posicao
        std::vector<double> *vetorx = new std::vector<double>();
        vetorx->resize(int(Px.size()));
        Eigen::VectorXd::Map(&vetorx->at(0), Px.size()) = Px;

        // Gera o vetoryE para o campo eletrico E(x,y)
        std::vector<double> *vetoryE = new std::vector<double>();
        vetoryE->resize(Px.size());
        Eigen::VectorXd::Map(&vetoryE->at(0), ptrEkVm->col(2).size()) = ptrEkVm->col(2).real();

        // Gera o vetoryB para o campo magnetico B(x,y)
        std::vector<double> *vetoryB = new std::vector<double>();
        vetoryB->resize(Px.size());
        Eigen::VectorXd::Map(&vetoryB->at(0), ptrBrms->col(3).size()) = ptrBrms->col(3).real();

        // ---- FIM ---- Mapea as saidas do Eigen para plotar

        // Plota campo elétrico
        QCustomPlot *customPlot1 = ui->plotxE;

        customPlot1->addGraph();
        customPlot1->graph(0)->setPen(QPen(Qt::blue));  // line color blue for first graph
        customPlot1->graph(0)->setBrush(
            QBrush(QColor(0, 0, 255, 20)));  // first graph will be filled with translucent blue

        customPlot1->xAxis->setLabel("Posição em baixo do vão");
        customPlot1->yAxis->setLabel("Campo Elétrico [kV/m]");

        plota(vetorx, vetoryE, customPlot1);

        // Plota campo magnetico
        QCustomPlot *customPlot2 = ui->plotxB;

        customPlot2->addGraph();
        customPlot2->graph(0)->setPen(QPen(Qt::red));  // line color blue for first graph
        customPlot2->graph(0)->setBrush(
            QBrush(QColor(255, 0, 0, 20)));  // first graph will be filled with translucent blue

        customPlot2->xAxis->setLabel("Posição em baixo do vão");
        customPlot2->yAxis->setLabel("Campo Magnético [µT]");

        plota(vetorx, vetoryB, customPlot2);

        double ekvmax = *max_element(vetoryE->begin(), vetoryE->end());
        double butmax = *max_element(vetoryB->begin(), vetoryB->end());

        QString texto1 =
            "Campo Elétrico máximo: " + QString::number(ekvmax, 'f', 2) + " [kV/m]           ";
        QString texto2 =
            "\t\t\t\t\t\t\t\t\t\tCampo Magnético máximo: " + QString::number(butmax, 'f', 2) +
            " [µT]";

        // QLabel *stat = new QLabel(texto1 + texto2);
        // stat->setAlignment(Qt::AlignRight);
        // statusBar()->addWidget(stat, 1);

        ui->statusBar->showMessage(texto1 + texto2);

        delete vetorx;
        delete vetoryE;
        delete vetoryB;
    }
    catch (std::exception &e) {
        qDebug() << "Erro: " << e.what() << "\n";
    }
}

void MainWindow::plota(std::vector<double> *vx, std::vector<double> *vy, QCustomPlot *plot)
{

    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(true);
    // plot->yAxis2->setLabel("Altura [m]");

    QVector<double> qVecX = QVector<double>::fromStdVector(*vx);
    QVector<double> qVecE = QVector<double>::fromStdVector(*vy);

    // qDebug() << qVecE << "\n";

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));

    // qDebug() << "Plota" << "\n";
    plot->graph(0)->setData(qVecX, qVecE);

    plot->graph(0)->rescaleAxes();
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    plot->replot();
    plot->update();
}

// --- INICIO - EVENTOS DOS BOTOES E CAIXAS --- //

void MainWindow::on_pushButtonReset_clicked()
{
    setData();
    ui->statusBar->showMessage("Dados resetados");
}

void MainWindow::on_pushButtonLimpar_clicked()
{
    clearData();
    ui->statusBar->showMessage("Dados limpos");
}

void MainWindow::on_pushButtonCalcular_clicked()
{
    calcula();
    // ui->statusBar->showMessage("Cálculos executados");
}

// ON ENTER
void MainWindow::on_lineEditTensao_returnPressed() { calcula(); }
void MainWindow::on_lineEditPot_returnPressed() { calcula(); }
void MainWindow::on_lineEditCoef_returnPressed() { calcula(); }
void MainWindow::on_lineEditHmax_returnPressed() { calcula(); }

void MainWindow::on_lineEditFlecha_returnPressed() { calcula(); }
void MainWindow::on_lineEditEspacamento_returnPressed() { calcula(); }
void MainWindow::on_lineEditDiametroCabos_returnPressed() { calcula(); }
void MainWindow::on_lineEditNcondutores_returnPressed() { calcula(); }

void MainWindow::on_lineEditXmax_returnPressed() { calcula(); }
void MainWindow::on_lineEditXmin_returnPressed() { calcula(); }
void MainWindow::on_lineEditPasso_returnPressed() { calcula(); }
void MainWindow::on_lineEditHsolo_returnPressed() { calcula(); }

void MainWindow::on_lineEditFeixeA_returnPressed() { calcula(); }
void MainWindow::on_lineEditFeixeB_returnPressed() { calcula(); }
void MainWindow::on_lineEditFeixeC_returnPressed() { calcula(); }

void MainWindow::on_lineEditFasea_returnPressed() { calcula(); }
void MainWindow::on_lineEditFaseb_returnPressed() { calcula(); }
void MainWindow::on_lineEditFasec_returnPressed() { calcula(); }

// --- FIM - EVENTOS DOS BOTOES E CAIXAS --- //
