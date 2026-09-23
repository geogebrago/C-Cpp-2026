#include <stdio.h>

void move(int n, char fr, char tp, char to)
{
    if (n <= 0)
        return;

    if (n == 1)
    {
        printf("%c -> %c\n", fr, to);
        return;
    }

    move(n - 1, fr, to, tp);

    printf("%c -> %c\n", fr, to);

    move(n - 1, tp, fr, to);
}

int main()
{
    const int N = 3;
    move(N, 'A', 'B', 'C');

    return 0;
}