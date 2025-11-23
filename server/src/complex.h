#pragma once
#include <QString>

// Простой struct Complex с базовыми операциями
struct Complex {
    double re{0.0}, im{0.0};
    Complex() = default;
    Complex(double r, double i): re(r), im(i) {}

    QString toString() const { return QString::number(re) + (im>=0?"+":"") + QString::number(im) + "i"; }
};

// Операции для удобства в шаблонных функциях
inline Complex operator+(const Complex &a, const Complex &b){
    return Complex(a.re + b.re, a.im + b.im);
}
inline Complex operator-(const Complex &a, const Complex &b){
    return Complex(a.re - b.re, a.im - b.im);
}
inline Complex operator*(const Complex &a, const Complex &b){
    return Complex(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re);
}
// Деление (не проверяет делитель на ноль — как и в учебном коде)
inline Complex operator/(const Complex &a, const Complex &b){
    double den = b.re*b.re + b.im*b.im;
    return Complex((a.re*b.re + a.im*b.im)/den, (a.im*b.re - a.re*b.im)/den);
}
