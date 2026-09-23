#include <stdio.h>

int main()
{
    long long n;
    scanf_s("%lld", &n);

    if (n < 2)
    {
        printf("No");
        return 0;
    }

    for (long long i = 2; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            printf("No");
            return 0;
        }
    }

    printf("Yes");
    return 0;
}