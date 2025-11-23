#pragma once
#include <QString>

struct Complex {
    double re{0.0}, im{0.0};
    Complex() = default;
    Complex(double r, double i): re(r), im(i) {}
    QString toString() const { return QString::number(re) + (im>=0?"+":"") + QString::number(im) + "i"; }
};