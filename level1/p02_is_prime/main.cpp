#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "../../safe_input.h"
int main()
{
    long long number;
    if (!safe_input(number))
        return 0;
    if (number < 2)
    {
        printf("No");
        return 0;
    }

    for (long long divisor = 2; divisor <= number / divisor; divisor++)
        if (number % divisor == 0)
        {
            printf("No");
            return 0;
        }

    printf("Yes");
    return 0;
}