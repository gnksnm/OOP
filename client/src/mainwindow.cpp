#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QHostAddress>
#include <QTimer>
#include <QMessageBox>
#include <QRegularExpression>
#include <complex>
#include <cmath>

static bool tryParseRoot(const QString &s, std::complex<double> &out) {
    QString t = s.trimmed();
    if (t.isEmpty()) return false;
    QRegularExpression reComplex(R"(^\s*([+-]?\d*\.?\d+)\s*([+-]\s*\d*\.?\d+)i\s*$)");
    QRegularExpressionMatch m = reComplex.match(t);
    if (m.hasMatch()) {
        double reVal = m.captured(1).toDouble();
        QString imCap = m.captured(2).remove(' ');
        double imVal = imCap.toDouble();
        out = std::complex<double>(reVal, imVal);
        return true;
    }
    QStringList comma = t.split(',', Qt::SkipEmptyParts);
    if (comma.size() == 2) {
        bool ok1, ok2;
        double reVal = comma[0].trimmed().toDouble(&ok1);
        double imVal = comma[1].trimmed().toDouble(&ok2);
        if (ok1 && ok2) { out = std::complex<double>(reVal, imVal); return true; }
    }
    bool ok; double reVal = t.toDouble(&ok);
    if (ok) { out = std::complex<double>(reVal, 0.0); return true; }
    return false;
}

MainWindow::MainWindow(QWidget*parent): QMainWindow(parent){
    QWidget *cw = new QWidget(this);
    setCentralWidget(cw);
    auto *mainLayout = new QVBoxLayout(cw);

    auto *netLayout = new QHBoxLayout();
    netLayout->addWidget(new QLabel("Server IP:"));
    leServer = new QLineEdit("127.0.0.1"); netLayout->addWidget(leServer);
    netLayout->addWidget(new QLabel("Port:")); lePort = new QLineEdit("45454"); lePort->setMaximumWidth(90); netLayout->addWidget(lePort);
    mainLayout->addLayout(netLayout);

    // ------------------ ADDED BLOCK: TYPE SELECTOR ------------------
    auto *typeRow = new QHBoxLayout();
    typeRow->addWidget(new QLabel("Number type:"));
    typeCombo = new QComboBox();
    typeCombo->addItem("Complex");
    typeCombo->addItem("Real");
    typeRow->addWidget(typeCombo);
    mainLayout->addLayout(typeRow);
    // ----------------------------------------------------------------

    QTabWidget *tabs = new QTabWidget();
    QWidget *arrayTab = new QWidget();
    QWidget *polyTab = new QWidget();
    tabs->addTab(arrayTab, "Array");
    tabs->addTab(polyTab, "Polynomial");
    mainLayout->addWidget(tabs);

    auto *aLayout = new QVBoxLayout(arrayTab);
    aLayout->addWidget(new QLabel("Array (placeholder)"));

    auto *pLayout = new QVBoxLayout(polyTab);
    auto *topRow = new QHBoxLayout();
    topRow->addWidget(new QLabel("Degree:"));
    leDegree = new QLineEdit(); leDegree->setMaximumWidth(80); topRow->addWidget(leDegree);
    topRow->addWidget(new QLabel("Polynomial coefficients (optional) or use Roots below"));
    lePolynomialInput = new QLineEdit(); lePolynomialInput->setText(""); topRow->addWidget(lePolynomialInput);
    pLayout->addLayout(topRow);

    auto *midRow = new QHBoxLayout();
    midRow->addWidget(new QLabel("Roots (semicolon or comma separated) e.g. 1+7i;6+8i;5"));
    leRootsInput = new QLineEdit(); leRootsInput->setText(""); midRow->addWidget(leRootsInput);
    midRow->addWidget(new QLabel("Extra (not used):")); leExtra = new QLineEdit(); leExtra->setText(""); midRow->addWidget(leExtra);
    btnCreatePoly = new QPushButton("Create polynomial from Roots"); midRow->addWidget(btnCreatePoly);
    pLayout->addLayout(midRow);

    btnSetHighest = new QPushButton("Set highest (from input)");
    leHighestInput = new QLineEdit(); leHighestInput->setPlaceholderText("re,im or a+bi");
    auto *hboxHighest = new QHBoxLayout(); hboxHighest->addWidget(leHighestInput); hboxHighest->addWidget(btnSetHighest); pLayout->addLayout(hboxHighest);

    auto *changeRow = new QHBoxLayout();
    leRootIndex = new QLineEdit(); leRootIndex->setPlaceholderText("Root index (1..n)"); leRootIndex->setMaximumWidth(120);
    changeRow->addWidget(leRootIndex);
    leNewRoot = new QLineEdit(); leNewRoot->setPlaceholderText("New root value (a+bi)"); changeRow->addWidget(leNewRoot);
    QPushButton *btnChangeRoot = new QPushButton("Change root"); changeRow->addWidget(btnChangeRoot);
    pLayout->addLayout(changeRow);

    auto *evalRow = new QHBoxLayout();
    leX = new QLineEdit(); leX->setPlaceholderText("x (re,im)"); evalRow->addWidget(leX);
    btnEvaluate = new QPushButton("Evaluate polynomial (send to server)"); evalRow->addWidget(btnEvaluate);
    pLayout->addLayout(evalRow);

    auto *buttonsRow = new QHBoxLayout();
    btnShowCoeffs = new QPushButton("Show coefficients"); buttonsRow->addWidget(btnShowCoeffs);
    btnShowRoots = new QPushButton("Show roots"); buttonsRow->addWidget(btnShowRoots);
    pLayout->addLayout(buttonsRow);

    lblHighest = new QLabel("Highest: -"); pLayout->addWidget(lblHighest);

    teLog = new QTextEdit(); teLog->setReadOnly(true); teLog->setMinimumHeight(200);
    mainLayout->addWidget(teLog);

    connect(btnCreatePoly, &QPushButton::clicked, this, &MainWindow::onCreatePolynomial);
    connect(btnSetHighest, &QPushButton::clicked, this, &MainWindow::onSetHighest);
    connect(btnShowCoeffs, &QPushButton::clicked, this, &MainWindow::onShowCoeffs);
    connect(btnShowRoots, &QPushButton::clicked, this, &MainWindow::onShowRoots);
    connect(btnChangeRoot, &QPushButton::clicked, this, &MainWindow::onChangeRoot);
    connect(btnEvaluate, &QPushButton::clicked, this, &MainWindow::onSend);
    connect(&client, &UdpClient::received, this, &MainWindow::onReceived);

    client.start(QHostAddress(leServer->text()), (quint16)lePort->text().toUShort());
}

Polynom MainWindow::parsePolynomialFromInputs(){
    QString s = lePolynomialInput->text().trimmed();
    if (s.isEmpty()) return Polynom();
    CArray arr;
    QStringList parts = s.split(';', Qt::SkipEmptyParts);
    for(const QString &part : parts){
        QStringList comps = part.split(',', Qt::SkipEmptyParts);
        if(comps.size()>=2){
            double re = comps[0].toDouble();
            double im = comps[1].toDouble();
            arr.push_back(Complex{re,im});
        }
    }
    return Polynom(arr);
}

Polynom MainWindow::buildPolynomialFromRootsInput() {
    QString s = leRootsInput->text().trimmed();
    lastRoots.clear();
    if (s.isEmpty()) return Polynom();

    QStringList rawParts = s.split(QRegularExpression("[,;]"), Qt::SkipEmptyParts);
    std::vector<std::complex<double>> roots;
    roots.reserve(rawParts.size());

    for (const QString &part : rawParts) {
        QString tok = part.trimmed();
        if (tok.isEmpty()) continue;
        std::complex<double> r;
        bool ok = tryParseRoot(tok, r);
        if (!ok) {
            teLog->append(QString("Invalid root token: '%1'. Polynomial not created. Use formats: a+bi, a,b, or a").arg(tok));
            lastRoots.clear();
            return Polynom();
        }
        roots.push_back(r);
    }
    if (roots.empty()) return Polynom();

    bool degreeOk = false; int degreeVal = leDegree->text().trimmed().toInt(&degreeOk);
    if (degreeOk && degreeVal > 0 && degreeVal != (int)roots.size()) {
        teLog->append(QString("Degree (%1) does not match number of roots (%2). Polynomial not created.").arg(degreeVal).arg((int)roots.size()));
        lastRoots.clear();
        return Polynom();
    }

    std::vector<std::complex<double>> coeffs;
    coeffs.push_back(std::complex<double>(1.0, 0.0));
    for (const auto &r : roots) {
        std::vector<std::complex<double>> next(coeffs.size() + 1, std::complex<double>(0.0, 0.0));
        for (size_t i = 0; i < coeffs.size(); ++i) {
            next[i] += coeffs[i];
            next[i+1] += -coeffs[i] * r;
        }
        coeffs.swap(next);
    }

    CArray arr; arr.reserve(coeffs.size());
    for (int i = (int)coeffs.size() - 1; i >= 0; --i) {
        arr.push_back(Complex{coeffs[i].real(), coeffs[i].imag()});
    }

    lastRoots = roots;
    return Polynom(arr);
}

QJsonObject MainWindow::polynomToJson(const Polynom &p){
    QJsonObject out; QJsonArray arr;
    for(const Complex &c : p.coeff){ QJsonObject o; o["re"]=c.re; o["im"]=c.im; arr.append(o); }
    out["coeff"]=arr; return out;
}

Polynom MainWindow::jsonToPolynom(const QJsonObject &o){
    QJsonArray arr = o.value("coeff").toArray();
    CArray v; v.reserve(arr.size());
    for(const QJsonValue &val : arr){
        QJsonObject c = val.toObject();
        v.push_back(Complex{c.value("re").toDouble(), c.value("im").toDouble()});
    }
    return Polynom(v);
}

void MainWindow::onCreatePolynomial() {
    Polynom p = buildPolynomialFromRootsInput();
    if (p.coeff.empty()) {
        p = parsePolynomialFromInputs();
        if (p.coeff.empty()) {
            teLog->append("No polynomial created (empty or invalid input).");
            return;
        } else {
            lastRoots.clear();
        }
    }
    currentPoly = p;
    lblHighest->setText("Highest: " +
                        (currentPoly.coeff.empty()
                             ? QString("-")
                             : QString::number(currentPoly.coeff.back().re)
                                   + (currentPoly.coeff.back().im>=0?"+":"")
                                   + QString::number(currentPoly.coeff.back().im) + "i"));
    teLog->append("Created polynomial (from roots): " + currentPoly.toString());
}

void MainWindow::onSetHighest() {
    if (currentPoly.coeff.empty()) { QMessageBox::warning(this, "Warning", "No polynomial created"); return; }
    QString token = leHighestInput->text().trimmed();
    if (token.isEmpty()) {
        teLog->append("Highest coefficient input is empty. Provide value as 're,im' or 'a+bi'.");
        return;
    }
    std::complex<double> val;
    if (!tryParseRoot(token, val)) {
        teLog->append(QString("Cannot parse highest coefficient: '%1'").arg(token));
        return;
    }
    int idx = (int)currentPoly.coeff.size() - 1;
    currentPoly.coeff[idx].re = val.real();
    currentPoly.coeff[idx].im = val.imag();
    lblHighest->setText("Highest: " + QString::number(val.real()) + (val.imag()>=0?"+":"") + QString::number(val.imag()) + "i");
    teLog->append("Set highest coefficient to: " + QString::number(val.real()) + (val.imag()>=0?"+":"") + QString::number(val.imag()) + "i");
}

void MainWindow::onShowCoeffs(){
    if(currentPoly.coeff.empty()){ teLog->append("No polynomial created."); return; }
    teLog->append("Coefficients (lowest -> highest):");
    for(size_t i=0;i<currentPoly.coeff.size();++i){
        auto &c = currentPoly.coeff[i];
        teLog->append(QString::number(i) + ": " + QString::number(c.re) + (c.im>=0?"+":"") + QString::number(c.im) + "i");
    }
}

void MainWindow::onShowRoots() {
    if (currentPoly.coeff.empty()) { teLog->append("No polynomial created."); return; }
    int n = (int)currentPoly.coeff.size() - 1;
    if (n <= 0) { teLog->append("Polynomial has no roots."); return; }

    if (!lastRoots.empty() && (int)lastRoots.size() == n) {
        teLog->append("Polynomial roots (from input):");
        for (int i = 0; i < n; ++i) {
            std::complex<double> r = lastRoots[i];
            double re = r.real(); double im = r.imag();
            if (std::abs(re) < 1e-12) re = 0.0;
            if (std::abs(im) < 1e-12) im = 0.0;
            auto fmt = [](double v)->QString { if (std::isnan(v) || std::isinf(v)) return QString("nan"); if (std::abs(v) < 1e-9) return QString("0"); return QString::number(v, 'g', 12); };
            teLog->append(QString("Root %1: %2 %3 %4i").arg(i+1).arg(fmt(re)).arg(im>=0?"+":"").arg(fmt(im)));
        }
        return;
    }

    std::vector<std::complex<double>> coeffs;
    for (int i = n; i >= 0; --i) coeffs.push_back(std::complex<double>(currentPoly.coeff[i].re, currentPoly.coeff[i].im));
    std::complex<double> lead = coeffs[0];
    if (std::abs(lead) < 1e-18) { teLog->append("Leading coefficient is zero."); return; }
    for (auto &c : coeffs) c /= lead;
    double max_ai = 0.0;
    for (size_t i=1;i<coeffs.size();++i) max_ai = std::max(max_ai, std::abs(coeffs[i]));
    double radius = 1.0 + max_ai;
    const double PI = 3.14159265358979323846;
    std::vector<std::complex<double>> roots(n);
    for (int i=0;i<n;++i) roots[i] = std::polar(radius, 2.0*PI*i/n);
    const double EPS = 1e-12;
    bool converged=false;
    for (int iter=0; iter<2000; ++iter) {
        converged=true;
        for (int i=0;i<n;++i) {
            std::complex<double> x = roots[i];
            std::complex<double> pval = coeffs[0];
            for (size_t j=1;j<coeffs.size();++j) pval = pval*x + coeffs[j];
            std::complex<double> denom(1.0,0.0);
            for (int j=0;j<n;++j) if (j!=i) denom *= (x - roots[j]);
            if (std::abs(denom) < EPS) denom = std::complex<double>(EPS, EPS);
            std::complex<double> new_x = x - pval/denom;
            if (std::abs(new_x - x) > 1e-11) converged=false;
            roots[i] = new_x;
        }
        if (converged) break;
    }
    auto fmt = [](double v)->QString { if (std::isnan(v) || std::isinf(v)) return QString("nan"); if (std::abs(v) < 1e-9) return QString("0"); return QString::number(v, 'g', 12); };
    teLog->append("Polynomial roots (computed):");
    for (int i=0;i<n;++i) {
        double re = roots[i].real(); double im = roots[i].imag();
        if (std::abs(re) < 1e-12) re = 0.0;
        if (std::abs(im) < 1e-12) im = 0.0;
        teLog->append(QString("Root %1: %2 %3 %4i").arg(i+1).arg(fmt(re)).arg(im>=0?"+":"").arg(fmt(im)));
    }
    if (!converged) teLog->append("Note: root-finding did not fully converge (results shown).");
}

void MainWindow::onChangeRoot() {
    if (lastRoots.empty()) { teLog->append("No roots available to change."); return; }
    bool ok; int idx = leRootIndex->text().toInt(&ok);
    if (!ok || idx<=0 || idx > (int)lastRoots.size()) { QMessageBox::warning(this, "Warning", "Invalid root index"); return; }
    std::complex<double> r;
    if (!tryParseRoot(leNewRoot->text(), r)) { teLog->append("Cannot parse new root."); return; }
    lastRoots[idx-1] = r;
    QStringList parts;
    for (auto &rr : lastRoots) {
        parts << QString::number(rr.real()) + (rr.imag()>=0?"+":"") + QString::number(rr.imag()) + "i";
    }
    leRootsInput->setText(parts.join(";"));
    currentPoly = buildPolynomialFromRootsInput();
    teLog->append("Changed root and rebuilt polynomial.");
}

void MainWindow::onSend(){
    if(currentPoly.coeff.empty()){
        teLog->append("No polynomial to send. Create polynomial first.");
        return;
    }

    QString op = "evaluate";
    QJsonObject msg;
    msg["op"] = op;
    msg["polynom"] = polynomToJson(currentPoly);

    // ------------------ ADDED: pass current type ------------------
    msg["type"] = (typeCombo->currentText() == "Real" ? "real" : "complex");
    // --------------------------------------------------------------

    QString xstr = leX->text().trimmed();
    double re=0, im=0;
    if(!xstr.isEmpty()){
        QStringList parts = xstr.split(',', Qt::SkipEmptyParts);
        if(parts.size()>=1) re = parts[0].toDouble();
        if(parts.size()>=2) im = parts[1].toDouble();
    }
    QJsonObject x; x["re"]=re; x["im"]=im;
    msg["x"]=x;

    client.sendJson(msg);
    teLog->append("Sent: " + QString::fromUtf8(QJsonDocument(msg).toJson(QJsonDocument::Compact)));
}

void MainWindow::onReceived(const QJsonObject &obj){
    QString status = obj.value("status").toString();
    if(status!="ok"){ teLog->append("Server error"); return; }
    QString type = obj.value("type").toString();
    if(type=="polynom"){
        Polynom p = jsonToPolynom(obj);
        teLog->append("Result polynomial: " + p.toString());
    } else if(type=="value"){
        QJsonObject v = obj.value("value").toObject();
        double re = v.value("re").toDouble(); double im = v.value("im").toDouble();
        teLog->append(QString("Result value: %1 %2 %3i").arg(re).arg(im>=0?"+":"").arg(im));
    }
}
