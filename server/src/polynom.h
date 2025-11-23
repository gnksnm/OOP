#pragma once
#include "array.h"
#include <QString>

struct Polynom {
    CArray coeff; // lowest-first: coeff[0] = const term
    Polynom() = default;
    explicit Polynom(const CArray &c): coeff(c) {}
    int degree() const { return (int)coeff.size() - 1; }
    static Polynom add(const Polynom &a, const Polynom &b) {
        CArray c(std::max(a.coeff.size(), b.coeff.size()));
        for (size_t i=0;i<c.size();++i){
            Complex A{0,0}, B{0,0};
            if (i < a.coeff.size()) A = a.coeff[i];
            if (i < b.coeff.size()) B = b.coeff[i];
            c[i] = Complex(A.re + B.re, A.im + B.im);
        }
        return Polynom(c);
    }
    static Polynom mul(const Polynom &a, const Polynom &b) {
        if (a.coeff.empty() || b.coeff.empty()) return Polynom();
        CArray c(a.coeff.size() + b.coeff.size() - 1, Complex{0,0});
        for (size_t i=0;i<a.coeff.size();++i)
            for (size_t j=0;j<b.coeff.size();++j){
                double re = a.coeff[i].re * b.coeff[j].re - a.coeff[i].im * b.coeff[j].im;
                double im = a.coeff[i].re * b.coeff[j].im + a.coeff[i].im * b.coeff[j].re;
                c[i+j].re += re; c[i+j].im += im;
            }
        return Polynom(c);
    }
    Polynom derivative() const {
        if (coeff.size()<=1) return Polynom({Complex{0,0}});
        CArray d(coeff.size()-1);
        for (size_t i=1;i<coeff.size();++i) d[i-1] = Complex{coeff[i].re * (double)i, coeff[i].im * (double)i};
        return Polynom(d);
    }
    Complex evaluate(const Complex &x) const {
        // Horner, lowest-first: compute from highest
        Complex res{0,0};
        for (int i = (int)coeff.size()-1; i>=0; --i){
            double re = res.re * x.re - res.im * x.im + coeff[i].re;
            double im = res.re * x.im + res.im * x.re + coeff[i].im;
            res.re = re; res.im = im;
        }
        return res;
    }
    QString toString() const {
        QString s;
        for (int i = (int)coeff.size()-1; i>=0; --i){
            if (i!=(int)coeff.size()-1) s += " + ";
            s += "(" + QString::number(coeff[i].re) + (coeff[i].im>=0?"+":"") + QString::number(coeff[i].im) + "i)";
            if (i>0) s += "*x";
            if (i>1) s += "^" + QString::number(i);
        }
        return s;
    }
};