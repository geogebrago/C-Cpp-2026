#include <bits/stdc++.h>
#include <conio.h>
#include <Windows.h>
using namespace std;

const int N = 1001;

int n, m;
int bx, by;  // 迷宫生成当前位置
int ex, ey;  // 终点
int ax, ay;  // 玩家位置

int dx[5] = {0, 1, -1, 0, 0};
int dy[5] = {0, 0, 0, 1, -1};

mt19937 rd(time(NULL));

bitset<N> vis[N];

struct nd
{
    int x, y, d;
};

vector<nd> s;

// 判断坐标是否在迷宫范围内
bool I_N(int x, int y)
{
    return x >= 1 && x <= n &&
           y >= 1 && y <= m;
}

// 将当前位置周围可以继续扩展的位置加入候选边
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

// 输出迷宫
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

// 获取玩家输入并移动
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

    // 如果输入的是 WASD
    if (d != -1)
    {
        int nx = ax + dx[d];
        int ny = ay + dy[d];

        // 先判断边界，再访问数组
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

    // 防止迷宫尺寸非法以及数组越界
    if (n <= 3 || m <= 3 || n >= N || m >= N)
        return 0;

    // 随机选择起点
    bx = rd() % n + 1;
    by = rd() % m + 1;

    // 玩家初始位置和迷宫生成起点相同
    ax = bx;
    ay = by;

    vis[bx][by] = 1;

    F_B();

    // 随机 Prim 生成迷宫
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

    // 找距离玩家最远的可走位置作为终点
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