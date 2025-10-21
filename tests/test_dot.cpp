// DOT test main

#include <dot/test.h>
#include <iostream>
#include <conio.h>

int main()
{
    dot::test::run();

    std::cout << "\n < Press any key to quit > ... ";
    getch();

    return 0;
}

// Здесь должен быть Unicode
