#include "polinom.h"
#include <iostream>
#include "complex.h"

Polinom::Polinom() {
	highest_coefficient = 0;
}
Polinom::Polinom(int power) : roots(power) {
	if (power < 0) { cout << "Неверная степень полинома"; }
	cout << "Введите n-коэффициент: ";
	cin >> highest_coefficient;
	if (power) { cout << "Введите остатки: "; }
	roots.fill_array();
	fill_coefficients();
}

Polinom::~Polinom() {
}

number Polinom::get_coefficient(int n) {
	return coefficients.get_element(n);
}

void Polinom::fill_coefficients() {
	int degree = roots.get_length();
	coefficients.resize_array(degree + 1); 
	coefficients.change_element(0, highest_coefficient);
	number jcoef,j1coef;
	for (int i = 0; i < degree; ++i) {
		number root = roots.get_element(i);
		for (int j = coefficients.get_length(); j > 0; j--) {
			jcoef = coefficients.get_element(j);
			j1coef = coefficients.get_element(j-1)*root; 
			coefficients.change_element(j, jcoef-j1coef);
		}
	}
}

void Polinom::set_highest_coefficient() {
	cout << "Введите старший коэффициент: ";
	cin >> highest_coefficient;
	coefficients.change_element(0, highest_coefficient);
	for (int i = 1; i < coefficients.get_length(); i++) {
		coefficients.change_element(i, 0);
	}
	fill_coefficients();
}

void Polinom::set_root(int index, number root) {
	roots.change_element(index-1, root);
	for (int i = 1; i < coefficients.get_length();i++) {
		coefficients.change_element(i, 0);
	}
	fill_coefficients();
}

void Polinom::print_polinom(bool var) {
	if (var) {
		cout << '('<<highest_coefficient<<')';
			for (int i = 0; i < roots.get_length(); i++) {
				cout << "(x-(" << roots.get_element(i) << "))";
			}
	}
	else {
		if (coefficients.get_length()) {
			for (int i = 0; i < coefficients.get_length() - 2; i++) {
				cout << '(' << coefficients.get_element(i) << ")x^" << coefficients.get_length() - 1 - i << '+';
			}
			if (coefficients.get_length() > 1) {
				cout << '(' << coefficients.get_element(coefficients.get_length() - 2) << ")x+";
			}
			cout << '(' << coefficients.get_element(coefficients.get_length() - 1) << ')';
		}
		else cout << highest_coefficient;
	}
}
number Polinom::count_polinom(number x) {
	number result(0, 0);
	for (int i = 0; i < coefficients.get_length(); i++) {
		result = result * x;
		result+=coefficients.get_element(i);
	}
	return result;
}