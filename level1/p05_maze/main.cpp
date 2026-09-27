#include <bits/stdc++.h>
#include <conio.h>
#include <Windows.h>
using namespace std;

const int N = 1001;

int n, m;
int bx, by;
int ex, ey;
int ax, ay;   

int dx[5] = {0, 1, -1, 0, 0};
int dy[5] = {0, 0, 0, 1, -1};

mt19937 rd(time(NULL));

bitset<N> vis[N];

struct nd
{
    int x, y, d;
};

vector<nd> s;

bool I_N(int x, int y)
{
    return x >= 1 && x <= n &&
           y >= 1 && y <= m;
}

void F_B()
{
    for (int i = 1; i <= 4; ++i)
    {
        int tx = bx + dx[i];
        int ty = by + dy[i];

        int nx = bx + 2 * dx[i];
        int ny = by + 2 * dy[i];

        if (I_N(nx, ny) && !vis[nx][ny])
        {
            s.push_back({tx, ty, i});
        }
    }
}

void O_P()
{
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (i == ex && j == ey)
                putchar('$');
            else if (i == ax && j == ay)
                putchar('@');
            else if (!vis[i][j])
                printf("#");
            else
                putchar(' ');
        }
        puts("");
    }
}

void GT()
{
    char ch = _getch();

    int d = -1;

    if (ch == 'w' || ch == 'W')
        d = 2;
    else if (ch == 's' || ch == 'S')
        d = 1;
    else if (ch == 'a' || ch == 'A')
        d = 4;
    else if (ch == 'd' || ch == 'D')
        d = 3;

    if (d != -1)
    {
        int nx = ax + dx[d];
        int ny = ay + dy[d];

        if (I_N(nx, ny) && vis[nx][ny])
        {
            ax = nx;
            ay = ny;
        }
    }

    system("cls");

    if (ax == ex && ay == ey)
    {
        puts("END");
        exit(0);
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    scanf("%d %d", &n, &m);

    if (n <= 3 || m <= 3 || n >= N || m >= N)
        return 0;

    bx = rd() % n + 1;
    by = rd() % m + 1;

    ax = bx;
    ay = by;

    vis[bx][by] = 1;

    F_B();

    while (!s.empty())
    {
        int sz = s.size();
        int num = rd() % sz;

        int x = s[num].x;
        int y = s[num].y;
        int d = s[num].d;

        int nx = x + dx[d];
        int ny = y + dy[d];

        if (I_N(nx, ny) && !vis[nx][ny])
        {
            vis[x][y] = 1;
            vis[nx][ny] = 1;

            bx = nx;
            by = ny;

            F_B();
        }

        swap(s[num], s.back());
        s.pop_back();
    }

    int md = -1;
    ex = ax;
    ey = ay;

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (vis[i][j])
            {
                int dist = abs(i - ax) + abs(j - ay);

                if (dist > md)
                {
                    md = dist;
                    ex = i;
                    ey = j;
                }
            }
        }
    }

    while (true)
    {
        O_P();
        GT();
    }

    return 0;
}