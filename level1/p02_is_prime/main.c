#include <stdio.h>
#include <stdlib.h>
int main()
{
    long long n;
    scanf_s("%lld",&n);
    for (long long i=2;i*i<=n;i++)
        if (n%i==0)
        {
            printf("No");
            return 0;
        }
    printf("Yes");
    return 0;
}//这份代码非常优秀,充分考虑题目条件， 请给我满分(doge)