#pragma once
#include "array.h"
#include <QString>
#include <type_traits>
#include <sstream>

template<typename Number>
struct PolynomT {
    Array<Number> coeff; // lowest-first: coeff[0] = const term
    PolynomT() = default;
    explicit PolynomT(const Array<Number> &c): coeff(c) {}
    int degree() const { return static_cast<int>(coeff.size()) - 1; }

    static PolynomT add(const PolynomT &a, const PolynomT &b) {
        size_t n = std::max(a.coeff.size(), b.coeff.size());
        Array<Number> c(n);
        for (size_t i=0;i<n;++i){
            Number A{}; Number B{};
            if (i < a.coeff.size()) A = a.coeff[i];
            if (i < b.coeff.size()) B = b.coeff[i];
            c[i] = A + B;
        }
        return PolynomT(c);
    }

    // Умножение (конволюция коэффициентов)
    static PolynomT mul(const PolynomT &a, const PolynomT &b){
        size_t n = a.coeff.size(), m = b.coeff.size();
        Array<Number> c(n + m - 1);
        for (size_t i=0;i<c.size();++i) c[i] = Number{};
        for (size_t i=0;i<n;++i){
            for (size_t j=0;j<m;++j){
                c[i+j] = c[i+j] + a.coeff[i] * b.coeff[j];
            }
        }
        return PolynomT(c);
    }

    // Оценка полинома в точке x (Horner)
    Number eval(const Number &x) const {
        if (coeff.size()==0) return Number{};
        Number res = coeff[coeff.size()-1];
        for (int i = (int)coeff.size()-2; i>=0; --i){
            res = res * x + coeff[i];
        }
        return res;
    }

    // Привычное текстовое представление. Форматирование — под Number:
    QString toString() const {
        QString s;
        for (int i = (int)coeff.size()-1; i>=0; --i){
            if (i!=(int)coeff.size()-1) s += " + ";
            // Попытка привести Number к строке:
            if constexpr (std::is_same<Number, double>::value) {
                s += "(" + QString::number(coeff[i]) + ")";
            } else {
                // Ожидаем, что Number имеет метод toString() или QString конструктор
                // Для нашего Complex выше есть toString()
                s += "(" + coeff[i].toString() + ")";
            }
            if (i>0) s += "*x";
            if (i>1) s += "^" + QString::number(i);
        }
        return s;
    }
};

// Для обратной совместимости с остальным кодом проекта:
#include "complex.h"
using Polynom = PolynomT<Complex>;
using RealPolynom = PolynomT<double>;
