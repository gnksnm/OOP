#pragma once
#include <vector>

// Общий шаблон массива (обертка над std::vector)
// Сохраняем совместимость: в проекте ранее использовался тип CArray = std::vector<Complex>.
// Теперь мы объявим шаблонный Array<T> и сделаем typedef-ы в конце файла.
template<typename T>
struct Array {
    std::vector<T> data;
    Array() = default;
    explicit Array(size_t n): data(n) {}
    size_t size() const { return data.size(); }
    void resize(size_t n) { data.resize(n); }
    void reserve(size_t n) { data.reserve(n); }
    void push_back(const T &v){ data.push_back(v); }
    T& operator[](size_t i){ return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    typename std::vector<T>::iterator begin(){ return data.begin(); }
    typename std::vector<T>::iterator end(){ return data.end(); }
    typename std::vector<T>::const_iterator begin() const { return data.begin(); }
    typename std::vector<T>::const_iterator end() const { return data.end(); }
};

// Типы для совместимости с существующим кодом:
#include "complex.h"
using CArray = Array<Complex>;
using RArray = Array<double>;
