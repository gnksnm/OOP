#pragma once
#include <iostream>

using namespace std;
class TComplex {
private:
	double re;
	double im;
public:
  
	TComplex();
	TComplex(double re);
	TComplex(double re, double im);
	double get_im();
	double get_re();
	double module(TComplex* number);
	TComplex operator+(TComplex& val) const;
	TComplex operator-(TComplex& val) const;
	TComplex operator/(TComplex& val) const;
	TComplex operator*(TComplex& val) const;
	TComplex operator+=(TComplex val);
	TComplex operator-=(TComplex val);
	TComplex operator/=(TComplex val);
	TComplex operator*=(TComplex val);
	TComplex operator=(TComplex val);
	TComplex operator-(double val);
	TComplex operator+=(double val);
	TComplex operator/=(double val);
	TComplex operator=(double val);
	bool operator==(TComplex val);
	bool operator!=(TComplex val);
	bool operator<(TComplex& val);
	bool operator>(TComplex& val);
	friend TComplex pow(TComplex complex, double n);
	friend istream& operator >> (istream& in, TComplex& c);
	friend ostream& operator << (ostream& os, const TComplex& c);

};