#pragma once
#include "array.h"
class Polinom {
private:
	Array roots;
	number highest_coefficient;
	Array coefficients;
	void fill_coefficients();
public:
	Polinom();
	Polinom(int power);
	~Polinom();
	number get_coefficient(int n);
	void set_highest_coefficient();
	void set_root(int index, number root);
	void print_polinom(bool var);
	number count_polinom(number x);
};