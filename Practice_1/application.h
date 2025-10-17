#pragma once
#include "array.h"
#include "number.h"
#include "polinom.h"
class Application {

public:
    Application();
    void show_menu();
    int exec();
    void main_menu();
    void polinom_app();
    void polinom_menu();
};