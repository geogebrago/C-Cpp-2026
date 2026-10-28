#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#define MAX_LIMIT 1005
bool is_composite[MAX_LIMIT];
int primes[MAX_LIMIT];
int main()
{
    int limit = 1000, prime_count = 0;
    clock_t start = clock();
    if (limit < 2 || limit >= MAX_LIMIT)
        return 0;
    for (int i = 2; i <= limit; ++i)
    {
        if (!is_composite[i])
        {
            primes[++prime_count] = i;
            printf("%d ", i);
        }
        for (int j = 1; j <= prime_count && primes[j] <= limit / i; ++j)
        {
            is_composite[i * primes[j]] = true;
            if (i % primes[j] == 0)
                break;
        }
    }
    puts("");
    clock_t end = clock();
    printf("%.10lf", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
