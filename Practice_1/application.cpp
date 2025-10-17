#include "application.h"

#include "iostream"

using namespace std;


Application::Application() {

}

void Application::main_menu() {
    char choise;
    while (true) {
        cout << "1.Работа с массивом\n";
        cout << "2.Работа с полиномом\n";
        cout << "3.Выход\n";
        cin >> choise;
        switch (choise) {
        case '1':
            exec();
            break;
        case '2':
            polinom_app();
            break;
        case '3':
            return;
        default:
            cout << "\nНеверный ввод\n";
            return;
        }
    }
}

void Application::polinom_menu() {
    cout << "1) Создать полином\n" <<
        "2) Изменить старший коэффициент\n" <<
        "3) Изменить один из корней\n" <<
        "4) Вычислить значение в заданной точке\n" <<
        "5) Вывести полином\n" <<
        "6) Назад\n";
}

void Application::polinom_app() {
    Polinom* polinom=nullptr;
    number highest_coefficient, value,x;
    int form;
    char choise;
    int power;
    while (true) {
        polinom_menu();
        cin >> choise;
        switch (choise) {
        case '1':
            cout << "Введите степень полинома: ";
            cin >> power;
            delete polinom;
            polinom = new Polinom(power);
            polinom->print_polinom(1);
            cout << '=';
            polinom->print_polinom(0);
            cout << endl;
            break;
        case '2':
            polinom->set_highest_coefficient();
            polinom->print_polinom(1);
            cout << '=';
            polinom->print_polinom(0);
            cout << endl;
            break;
        case '3':
            int index; 
            cout << "Введите индекс корня: ";
            cin >> index;
            cout << endl << "Введите новое значение: ";
            cin >> value;
            polinom->set_root(index, value);
            polinom->print_polinom(1);
            cout << '=';
            polinom->print_polinom(0);
            cout << endl;
            break;
        case '4':
            cout << "Введите значение x: ";
            cin >> x;
            cout<<"Значение полинома в точке "<<x<<" = " << polinom->count_polinom(x) << endl;
            break;
        case '5':
            cout << "В какой форме вывести полином?\n1.Форма с коэффициентами\n2.Форма с корнями\n";
            cin >> form;
            polinom->print_polinom(form - 1);
            cout << endl;
            break;
        case'6':
            return;
        }
    }
}

void Application::show_menu() {
    cout << "1. Создать массив\n";
    cout << "2. Заполнить массив\n";
    cout << "3. Изменить размерность массива\n";
    cout << "4. Вывести элементы массива\n";
    cout << "5. Посчитать среднее\n";
    cout << "6. Посчитать СКО\n";
    cout << "7. Отсортировать\n";
    cout << "8. Изменить элемент\n";
    cout << "9. Вывести конкретный элемент\n";
    cout << "0. Выйти\n\n";
}


int Application::exec() {
    unsigned char choice, sort_reverse;
    int len, newLen, index;
    number value;
    Array* array = new Array(0);

    while (true) {
        show_menu();
        cout << "Выберите: "; cin >> choice;
        number eight_el=array->get_element(8);
        switch (choice) {

        case '1':
            cout << "Введите длину массива: "; 
            cin >> len;
            if (cin.fail()) {
                cin.clear();
                len = -1;
                cin.ignore();
            }
            array = new Array(len);
            break;

        case '2':
            cout << "Введите " << array->get_length() << " элементов массива через пробел\n";
            array->fill_array();
            cout << "Массив заполнен\n";
            array->print_array();
            break;

        case '3':
            cout << "Введите новую длину массива: "; cin >> newLen;
            array->resize_array(newLen);
            cout << "Длина массива успешно изменена\n";
            array->print_array();
            break;

        case '4':
            array->print_array();
            break;

        case '5':
            cout << "Среднее значение: " << array->average_value() << "\n";
            break;

        case '6':
            cout << "СКО:" << array->SKO() << "\n";
            break;

        case '7':
            cout << "Сортировка:\n";
            cout << "1. По возрастанию\n";
            cout << "2. По убыванию\n";
            cin >> sort_reverse;
            switch (sort_reverse) {
            case '1':
                array->shaker_sort();
                break;

            case '2':
                array->shaker_sort(true);
                break;
            }
            cout << "Сортировка завершена\n";
            array->print_array();
            break;

        case '8':
            cout << "Введите\n";
            cout << "Индекс элемента: "; cin >> index;
            cout << "Новое значение: "; cin >> value; cout << "\n";

            array->change_element(index, value);
            cout << "Значение элемента заменено\n";
            array->print_array();
            break;

        case '9':
            cout << "Введите индекс элемента: "; cin >> index;
            if ((index <= (array->get_length()) - 1)&&(index>=0)) {
                cout << "Значение " << index << " элемента: " << array->get_element(index) << "\n";
            }
            else {
                cout<<"Нет элемента с таким индексом";
            }
            break;

        case '0':
            return 0;

        default:
            cout << "Неверный номер\n";

        }
        cout << "\n";
    }
}


