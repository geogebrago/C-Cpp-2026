#include <bits/stdc++.h>
#include <windows.h>
#include <psapi.h>

using namespace std;

constexpr int DIGIT_BASE = 1e9;
constexpr int MAX_DIGITS = 10055;
constexpr int PRECISION_POSITION = 1205;
constexpr int MAX_ITERATIONS = 4500;

static int get_memory_usage_kb()
{
    PROCESS_MEMORY_COUNTERS memory_counters;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memory_counters, sizeof(memory_counters)))
    {
        return static_cast<int>(memory_counters.PeakWorkingSetSize / 1024);
    }
    return -1;
}

namespace
{
    struct BigInteger
    {
        int length;
        int digits[MAX_DIGITS];

        BigInteger(int value = 0) : length(0), digits{0}
        {
            while (value)
            {
                digits[++length] = value % DIGIT_BASE;
                value /= DIGIT_BASE;
            }
        }

        void operator+=(const BigInteger& other)
        {
            length = max(length, other.length);
            for (int i = 1; i <= length; ++i)
            {
                digits[i] += other.digits[i];
                if (digits[i] >= DIGIT_BASE)
                {
                    digits[i] -= DIGIT_BASE;
                    digits[i + 1]++;
                }
            }
            while (digits[length + 1])
                ++length;
        }

        void operator-=(const BigInteger& other)
        {
            for (int i = 1; i <= length; ++i)
            {
                digits[i] -= other.digits[i];
                if (digits[i] < 0)
                {
                    digits[i] += DIGIT_BASE;
                    digits[i + 1]--;
                }
            }
            while (length > 1 && digits[length] == 0)
                --length;
        }

        BigInteger operator*(int multiplier) const
        {
            BigInteger result(0);
            result.length = length;

            if (length == 0 || multiplier == 0)
                return result;

            long long carry = 0;
            for (int i = 1; i <= length; ++i)
            {
                carry += 1LL * digits[i] * multiplier;
                result.digits[i] = static_cast<int>(carry % DIGIT_BASE);
                carry /= DIGIT_BASE;
            }
            while (carry)
            {
                result.digits[++result.length] = static_cast<int>(carry % DIGIT_BASE);
                carry /= DIGIT_BASE;
            }
            return result;
        }

        void operator/=(int divisor)
        {
            if (divisor == 0)
                return;

            long long remainder = 0;
            for (int i = length; i >= 1; --i)
            {
                remainder = 1LL * remainder * DIGIT_BASE + digits[i];
                digits[i] = static_cast<int>(remainder / divisor);
                remainder %= divisor;
            }
            while (length > 1 && digits[length] == 0)
                --length;
        }

        void operator*=(int multiplier)
        {
            if (multiplier == 0)
            {
                memset(digits, 0, sizeof(digits));
                length = 1;
                digits[1] = 0;
                return;
            }

            long long carry = 0;
            for (int i = 1; i <= length; ++i)
            {
                carry += 1LL * digits[i] * multiplier;
                digits[i] = static_cast<int>(carry % DIGIT_BASE);
                carry /= DIGIT_BASE;
            }
            while (carry)
            {
                digits[++length] = static_cast<int>(carry % DIGIT_BASE);
                carry /= DIGIT_BASE;
            }
        }
    };
}

static BigInteger arctangent(int denominator)
{
    BigInteger result(0);
    BigInteger term(0);

    term.length = PRECISION_POSITION;
    term.digits[PRECISION_POSITION] = 1;
    term /= denominator;

    for (int index = 0; term.length != 0 && index < MAX_ITERATIONS; ++index)
    {
        if (index % 2 == 0)
            result += term;
        else
            result -= term;

        term *= (2 * index + 1);
        term /= denominator;
        term /= denominator;
        term /= (2 * index + 3);
    }

    return result;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    double start_time = clock();

    BigInteger term1 = arctangent(18);
    BigInteger term2 = arctangent(57) * 32;
    BigInteger term3 = arctangent(239) * 20;

    BigInteger pi_value = term1;
    pi_value += term2;
    pi_value -= term3;

    for (int digit_index = pi_value.length; digit_index >= 1; --digit_index)
    {
        if (digit_index == pi_value.length)
        {
            string first_digit_str = to_string(pi_value.digits[digit_index]);
            cout << first_digit_str[0] << ".";
            cout << first_digit_str.substr(1);
        }
        else
        {
            printf("%09d", pi_value.digits[digit_index]);
        }
    }

    double end_time = clock();
    printf("\n时间: %.6lfs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    printf("内存占用: %d KB", get_memory_usage_kb());

    return 0;
}
