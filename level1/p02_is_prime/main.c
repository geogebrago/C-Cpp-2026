#include <stdio.h>

int main()
{
    long long number;
    scanf("%lld", &number);
    if (number < 2)
    {
        printf("No");
        return 0;
    }

    for (long long divisor = 2; divisor <= number / divisor; ++divisor)
    {
        if (number % divisor == 0)
        {
            printf("No");
            return 0;
        }
    }

    printf("Yes");
    return 0;
}