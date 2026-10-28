#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "../../safe_input.h"
static const unsigned long delay_ms = 50;
static const int line_width = 50;
static const int line_count = 5;

int main()
{
    std::string text;
    if (!safe_input(text))
        return 1;
    int length = (int)text.length();
    int position = 0;
    int direction = 1;

    const int right_bound = line_width - length;

    while (1)
    {
        system("cls");

        for (int i = 0; i < position; i++)
            putchar(' ');

        printf("%s", text.c_str());

        for (int i = 0; i < line_width - position - length; i++)
            putchar(' ');

        puts("|");

        for (int i = 1; i <= line_count; ++i)
        {
            for (int j = 0; j < line_width; ++j)
                putchar(' ');
            puts("|");
        }

        position += direction;

        if (position >= right_bound)
            direction = -1;

        if (position <= 0)
            direction = 1;

        Sleep(delay_ms);
    }
}
