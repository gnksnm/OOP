#pragma once
#include "number.h"
class Array
{
private:
	number *array;
	int length;
public:
    Array(int length = 0);
    ~Array();
    number get_element(int index);
    int get_length();
    void fill_array();
    bool resize_array(int length);
    bool change_element(int index, number value);
    void print_array();
    number average_value();
    number SKO();
    void shaker_sort(bool reverse = false);
};
