#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#define N 10000005

bool vis[N];
int pri[N];

int main()
{
    int n, tot = 0;
    scanf_s("%d", &n);
    double start = clock();
    if (n < 2 || n >= N)
        return 0;
    for (int i = 2; i <= n; ++i)
    {
        if (!vis[i])
        {
            pri[++tot] = i;
            printf("%d ", i);
        }

        for (int j = 1; j <= tot && pri[j] <= n / i; ++j)
        {
            vis[i * pri[j]] = true;

            if (i % pri[j] == 0)
                break;
        }
    }
    puts("");
    double end= clock();
    printf("%.10lf",(end-start)/CLOCKS_PER_SEC);
    return 0;
}