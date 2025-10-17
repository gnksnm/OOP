#include "complex.h"

TComplex::TComplex() {
	re = im = 0;
}
TComplex::TComplex(double re) {
	this->re = re;
	im = 0;
}
TComplex::TComplex(double re, double im) {
	this->re = re;
	this->im = im;
}
double TComplex::get_re() {
	return this->re;
}
double TComplex::get_im() {
	return this->im;
}
double TComplex::module(TComplex* number) {
	double a = pow(number->get_re(), 2);
	double b = pow(number->get_im(), 2);
	return pow((a+b), 0.5);
}
TComplex TComplex::operator+(TComplex& val) const{
	return TComplex(re + val.re, im + val.im);
}
TComplex TComplex::operator-(TComplex& val) const {
	return TComplex(re - val.re, im - val.im);
}
TComplex TComplex::operator*(TComplex& val) const {
	return TComplex(re * val.re - im * val.im, re * val.im + im * val.re);
}
TComplex TComplex::operator/(TComplex& val) const {
	double real = (re * val.re + im * val.im) / (pow(val.re, 2) + pow(val.im, 2));
	double imag = (im * val.re - re * val.im) / (pow(val.re, 2) + pow(val.im, 2));
	return TComplex(real, imag);
}
TComplex TComplex::operator+=(TComplex val) {
	re += val.re;
	im += val.im;
	return TComplex(re, im);
}
TComplex TComplex::operator-=(TComplex val) {
	re -= val.re;
	im -= val.re;
	return TComplex(re, im);
}
TComplex TComplex::operator*=(TComplex val) {
	double re1 = re;
	double im1 = im;
	re = re1 * val.re - im1 * val.im;
	im = re * val.im + im * val.re;
	return TComplex(re, im);
}
TComplex TComplex::operator/=(TComplex val) {
	double re1 = re;
	double im1 = im;
	re = (re * val.re + im * val.im) / (pow(val.re, 2) + pow(val.im, 2));
	im = (im * val.re - re * val.im) / (pow(val.re, 2) + pow(val.im, 2));
	return TComplex(re, im);
}
TComplex TComplex::operator=(TComplex val) {
	re = val.re;
	im = val.im;
	return TComplex(re, im);
}
TComplex TComplex::operator-(double val) {
	re = re - val;
	return TComplex(re, im);
}
TComplex TComplex::operator+=(double val) {
	re = re + val;
	return TComplex(re, im);
}
TComplex TComplex::operator/=(double val) {
	double re1 = re;
	double im1 = im;
	re = (re1 * val) / (val * val);
	im = (im1 * val) / (val * val);
	return TComplex(re, im);
}
TComplex TComplex::operator=(double val) {
	return TComplex(val,0);
}
bool TComplex::operator==(TComplex val) {
	if (re == val.re && im == val.im) {
		return true;
	}
	else return false;
}
bool TComplex::operator!=(TComplex val) {
	if (re != val.re || im != val.im) {
		return true;
	}
	else return false;
}
bool TComplex::operator<(TComplex& val){
	if (module(this)<module(&val)) {
		return true;
	}
	if (module(this) == module(&val)) {
		double this_arg = (this->re != 0) ? atan(this->im / this->re) : (this->im > 0 ? 3.14 / 2 : -3.14 / 2);
		double val_arg = (val.re != 0) ? atan(val.im / val.re) : (val.im > 0 ? 3.14 / 2 : -3.14 / 2);

		return this_arg < val_arg;
	}
	return false;
}
bool TComplex::operator>(TComplex& val) {
	if (module(this) > module(&val)) {
		return true;
	}
	if (module(this) == module(&val)) {
		double thisArg = (this->re != 0) ? atan(this->im / this->re) : (this->im > 0 ? 3.14 / 2 : -3.14 / 2);
		double valArg = (val.re != 0) ? atan(val.im / val.re) : (val.im > 0 ? 3.14 / 2 : -3.14 / 2);

		return thisArg > valArg;
	}
	return false;
}
TComplex pow(TComplex complex, double val) {
	double corn;
	if (complex.re > 0) corn = atan(complex.im / complex.re);
	else if (complex.re < 0 && complex.im >= 0) corn = 3.14 - atan(complex.im / complex.re);
	else if (complex.re < 0 && complex.im < 0) corn = -3.14 + atan(complex.im / complex.re);
	else if (complex.re == 0 && complex.im > 0) corn = 3.14 / 2;
	else corn = -3.14 / 2;
	return TComplex(pow(complex.module(&complex), val) * cos(corn * val), pow(complex.module(&complex), val) * sin(corn * val));
}
std::istream& operator>>(std::istream& in, TComplex& number) {
	in >> number.re >> number.im;
	return in;
}
std::ostream& operator<<(std::ostream& os, const TComplex& number) {
	if (number.re != 0 && number.im < 0) {
		os << number.re << number.im << 'i';
	}
	else if (number.re == 0 && number.im != 0) {
		os << number.im << 'i';
	}
	else if (number.re != 0 && number.im == 0) {
		os << number.re;
	}
	else if (number.re != 0 && number.im > 0) {
		os << number.re << '+' << number.im<<'i';
	}
	else os << '0';
	return os;
}
