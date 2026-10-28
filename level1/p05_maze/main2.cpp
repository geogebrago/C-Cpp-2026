#include <stdio.h>
#include "main2.h"
void print_board(void)
{
    for (int i = 1; i <= rows; ++i)
    {
        for (int j = 1; j <= cols; ++j)
        {
            if (i == end_x && j == end_y)
                putchar('$');
            else if (i == player_x && j == player_y)
                putchar('@');
            else if (!visited[i][j])
                putchar('#');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}