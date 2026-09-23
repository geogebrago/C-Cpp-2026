#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int slp = 50;
const int gp = 50;
const int cl = 5;

int main()
{
    string s;
    cin >> s;

    int sz = (int)s.size();
    int pos = 0;
    int dr = 1;

    const int left = 0;
    const int right = gp - sz;

    while (true)
    {
        system("cls");

        for (int i = 0; i < pos; i++)
            putchar(' ');

        cout << s;

        for (int i = 0; i < gp - pos - sz; i++)
            putchar(' ');

        puts("|");

        for (int i = 1; i <= cl; ++i)
        {
            for (int j = 0; j < gp; ++j)
                putchar(' ');
            puts("|");
        }

        pos += dr;

        if (pos >= right)
            dr = -1;

        if (pos <= left)
            dr = 1;

        Sleep(slp);
    }
}