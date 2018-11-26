#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "lib/FuncoesMag.hpp"
#include "lib/qcustomplot.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void getData(conf&);
    void setData();
    void clearData();
    void calcula();
    void plota(std::vector<double>*, std::vector<double>*, QCustomPlot*);

private slots:
    void on_pushButtonReset_clicked();
    void on_pushButtonLimpar_clicked();
    void on_pushButtonCalcular_clicked();
    void on_lineEditTensao_returnPressed();
    void on_lineEditCoef_returnPressed();
    void on_lineEditHmax_returnPressed();
    void on_lineEditPot_returnPressed();
    void on_lineEditFlecha_returnPressed();
    void on_lineEditEspacamento_returnPressed();
    void on_lineEditDiametroCabos_returnPressed();
    void on_lineEditNcondutores_returnPressed();
    void on_lineEditXmax_returnPressed();
    void on_lineEditXmin_returnPressed();
    void on_lineEditPasso_returnPressed();
    void on_lineEditHsolo_returnPressed();
    void on_lineEditFeixeA_returnPressed();
    void on_lineEditFeixeB_returnPressed();
    void on_lineEditFeixeC_returnPressed();
    void on_lineEditFasea_returnPressed();
    void on_lineEditFaseb_returnPressed();
    void on_lineEditFasec_returnPressed();

private:
    Ui::MainWindow *ui;

    // Dados de entrada
    const double tensao {765e3};
    const double potencia {2.2e9};
    const double coef {0.87};
    const double hmax {25.0};
    const double flecha {8.63};
    const double corrente {potencia/tensao};
    const double espacamento {0.457};
    const double diamCabo {0.02959};
    const double nCondutores {4};
    const double hmin {hmax-flecha-(espacamento/2.0)};      //TODO Identificar altura mínima

    const double xmin {-80.0};
    const double xmax {80.0};
    const double passo {0.5};
    const double hsolo {1.5};

    const double posFaseA {-14.34};
    const double posFaseB {0.0};
    const double posFaseC {14.34};

    const double angFaseA {120.0};
    const double angFaseB {0.0};
    const double angFaseC {-120.0};

};

#endif // MAINWINDOW_HPP
