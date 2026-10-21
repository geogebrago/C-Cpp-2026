#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main()
{
    char s[100], *end;
    if (!fgets(s, sizeof(s), stdin))
        return 0;

    errno = 0;
    long long number = strtoll(s, &end, 10);

    while (isspace((unsigned char)*end))
        end++;

    if (end == s || *end || errno || number < 2)
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