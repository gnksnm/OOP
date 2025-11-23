#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include "udpclient.h"
#include "complex.h"
#include "polynom.h"
#include <complex>
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget*parent=nullptr);
private slots:
    void onSend();
    void onReceived(const QJsonObject &obj);
    void onCreatePolynomial();
    void onSetHighest();
    void onShowCoeffs();
    void onShowRoots();
    void onChangeRoot();
    void onEvaluatePolynomialLocal();
private:
    UdpClient client;
    QLineEdit *leServer, *lePort;
    QTextEdit *teLog;

    QLineEdit *leDegree;
    QLineEdit *lePolynomialInput;
    QLineEdit *leRootsInput;
    QLineEdit *leExtra;
    QPushButton *btnCreatePoly;
    QPushButton *btnSetHighest;
    QPushButton *btnShowCoeffs;
    QPushButton *btnShowRoots;
    QPushButton *btnEvaluate;
    QLineEdit *leRootIndex, *leNewRoot, *leX;
    QLabel *lblHighest;

    QLineEdit *leHighestInput;

    Polynom currentPoly;
    std::vector<std::complex<double>> lastRoots; // store user-entered roots
    Polynom parsePolynomialFromInputs();
    Polynom buildPolynomialFromRootsInput();
    QJsonObject polynomToJson(const Polynom &p);
    Polynom jsonToPolynom(const QJsonObject &o);
};