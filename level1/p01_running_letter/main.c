#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
static const unsigned long delay_ms = 50;
static const int line_width = 50;
static const int line_count = 5;

int main()
{
    char text[101];
    if (scanf("%49s", text) != 1)
        return 1;
    int length = (int)strlen(text);
    int position = 0;
    int direction = 1;

    const int right_bound = line_width - length;

    while (1)
    {
        system("cls");

        for (int i = 0; i < position; i++)
            putchar(' ');

        printf("%s", text);

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
