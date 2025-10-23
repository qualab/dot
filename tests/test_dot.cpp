// Запуск всех написанных наборов тестов

#include <dot/test.h>
#include <iostream>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
   SetConsoleOutputCP(CP_UTF8);
#endif

    dot::test::run();

    std::cout << "\n < Нажмите любую клавишу > ... ";
    getch();

    return 0;
}

// Здесь должен быть Unicode
