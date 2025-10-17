#include "application.h"
#include "array.h"
#include <locale>
#include "polinom.h"

int main() {
    setlocale(LC_ALL, "Russian");
    Application app = Application();
    app.main_menu();
    return 0;
}