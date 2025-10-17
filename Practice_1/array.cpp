#include "array.h"
#include <iostream>

Array::Array(int len){
	if (len < 0){
		len = 0;
		std::cout << "Неверно введена длина\n"
			"длина = 0\n";
	}
	this->length = len;
	array = new number[len];
	for (int i = 0; i < this->length; i++) {
		array[i] = 0;
	}
}

Array::~Array() {
	delete array;
}

int Array::get_length() {
	return this->length;
}

void Array::fill_array() {
	for (int i = 0; i < length; i++) {
		std::cin >> *(array + i);
	}
}

bool Array::resize_array(int new_length) {
	if (new_length < 0) {
		return false;
	}
	number* new_array = new number[new_length];
	for (int i = 0; i < length; i++) {
		*(new_array + i) = *(array + i);
	}
	for (int i = length; i < new_length; i++) {
		*(new_array + i) = 0;
	}
	length = new_length;
	delete[]array;
	array = new_array;
	return true;
}

bool Array::change_element(int index, number value) {
	if (index >= length && index <= 0) {
		return false;
	}
	*(array + index) = value;
	return true;
}

number Array::get_element(int index) {
	if (index >= length && index <= 0) {
		return false;
	}
	return *(array + index);
}

void Array::print_array() {
	std::cout << "Массив: ";
	for (int i = 0; i < length; i++) {
		std::cout << *(array + i) << " ";
	}
	std::cout << "\n";
}

number Array::average_value() {
	number sum = 0;
	if (length == 0) {
		return 0;
	}
	for (int i = 0; i < length; i++) {
		sum += *(array + i);
	}
	sum /= ((number)length);
	return sum;
}

number Array::SKO() {
	number avg = average_value();
	number sko_value = 0;
	if (length == 0) {
		return 0;
	}
	for (int i = 0; i < length; i++) {
		sko_value += pow(*(array + i) - avg, 2);
	}
	sko_value /= ((number)length - 1);
	sko_value = pow(sko_value, 0.5);
	return sko_value;
}

void Array::shaker_sort(bool reverse) {
	bool swapped = true;
	int start = 0;
	int end = length - 1;

	while (swapped) {
		swapped = false;

		if (!reverse) {
			for (int i = start; i < end; ++i) {
				if (array[i] > array[i + 1]) {
					std::swap(array[i], array[i + 1]);
					swapped = true;
				}
			}
		}
		else {
			for (int i = start; i < end; ++i) {
				if (array[i] < array[i + 1]) {
					std::swap(array[i], array[i + 1]);
					swapped = true;
				}
			}
		}

		if (!swapped) {
			break;
		}

		swapped = false;
		--end;

		if (!reverse) {
			for (int i = end - 1; i >= start; --i) {
				if (array[i] > array[i + 1]) {
					std::swap(array[i], array[i + 1]);
					swapped = true;
				}
			}
		}
		else {
			for (int i = end - 1; i >= start; --i) {
				if (array[i] < array[i + 1]) {
					std::swap(array[i], array[i + 1]);
					swapped = true;
				}
			}
		}

		++start;
	}
}