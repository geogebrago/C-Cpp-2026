#include <bits/stdc++.h>
#include <Windows.h>
using namespace std;

const int N = 35, P = 1000, L = 80, R = 20, C = 20;

int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};
mt19937 rd(time(NULL));

struct nd
{
    int s[L], x, y, fit;
} p[P], q[P];

char s[N][N + 1] =
    {
        "####################",
        "#@.................#",
        "###....###########.#",
        "#..................#",
        "#.######....########",
        "#..................#",
        "#####....#########.#",
        "#..................#",
        "#.########....######",
        "#..................#",
        "###.....##########.#",
        "#..................#",
        "#.##########....####",
        "#..................#",
        "######....########.#",
        "#..................#",
        "#.###########...####",
        "#..................#",
        "##################.$",
        "####################"};

void prt(int id)
{
    int xx = 1, yy = 1;

    for (int i = 0; i < L; i++)
    {
        if (p[id].s[i] == 0)
        {
            xx++;
            cout << "下 ";
        }
        else if (p[id].s[i] == 1)
        {
            yy++;
            cout << "右 ";
        }
        else if (p[id].s[i] == 2)
        {
            xx--;
            cout << "上 ";
        }
        else if (p[id].s[i] == 3)
        {
            yy--;
            cout << "左 ";
        }

        if (xx == 18 && yy == 19)
        {
            cout << "到达终点" << endl;
            return;
        }
    }
}

int dst(int x, int y)
{
    return abs(x - 18) + abs(y - 19);
}

int calc(nd &a)
{
    a.x = 1, a.y = 1;

    int fit = 0;

    for (int i = 0; i < L; i++)
    {
        int nx = a.x + dx[a.s[i]];
        int ny = a.y + dy[a.s[i]];

        if (nx < 0 || nx >= R || ny < 0 || ny >= C || s[nx][ny] == '#')
        {
            fit -= 100;
            continue;
        }

        int od = dst(a.x, a.y);

        a.x = nx;
        a.y = ny;

        if (s[nx][ny] == '$')
        {
            return P + L - i;
        }

        fit += od - dst(a.x, a.y);
    }

    fit -= dst(a.x, a.y);

    return fit;
}

nd &cham()
{
    nd &x = p[rd() % P], &y = p[rd() % P], &z = q[rd() % P];

    if (x.fit < y.fit)
        swap(x, y);

    if (x.fit < z.fit)
        swap(x, z);

    return x;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    for (int i = 0; i < P; i++)
    {
        int cx = 1, cy = 1;

        for (int j = 0; j < L; j++)
        {
            int v[4] = {0}, ok = 0;

            for (int k = 0; k < 4; k++)
            {
                int nx = cx + dx[k];
                int ny = cy + dy[k];

                if (nx >= 0 && nx < R && ny >= 0 && ny < C &&
                    (s[nx][ny] == '.' || s[nx][ny] == '@'))
                {
                    v[ok++] = k;
                }
            }

            if (!ok)
            {
                p[i].s[j] = rd() % 4;
                continue;
            }

            int bst = v[0];

            for (int k = 1; k < ok; k++)
            {
                int ax = cx + dx[v[k]];
                int ay = cy + dy[v[k]];

                int bx = cx + dx[bst];
                int by = cy + dy[bst];

                if (dst(ax, ay) < dst(bx, by))
                    bst = v[k];
            }

            p[i].s[j] = (rd() % 100 < 80) ? bst : v[rd() % ok];

            cx += dx[p[i].s[j]];
            cy += dy[p[i].s[j]];
        }
    }

    bool flag = false;

    for (int g = 0; g < P; g++)
    {
        for (int i = 0; i < P; i++)
            p[i].fit = calc(p[i]);

        int id = 0;

        for (int i = 1; i < P; i++)
        {
            if (p[i].fit > p[id].fit)
                id = i;
        }

        if (p[id].fit >= P)
        {
            cout << "找到路径，路径为：";
            prt(id);
            flag = true;
            break;
        }

        q[0] = p[id];

        int muta = max(1, 8 - g * 7 / P);

        for (int i = 1; i < P; i += 2)
        {
            nd &x = cham(), &y = cham();

            q[i] = x;

            if (i + 1 < P)
                q[i + 1] = y;

            int k = rd() % L;

            if (i + 1 < P)
            {
                for (int j = k; j < L; j++)
                {
                    swap(q[i].s[j], q[i + 1].s[j]);
                }
            }

            for (int j = 0; j < L; j++)
            {
                if (rd() % 100 < muta)
                {
                    q[i].s[j] = rd() % 4;
                }

                if (i + 1 < P && rd() % 100 < muta)
                {
                    q[i + 1].s[j] = rd() % 4;
                }
            }
        }

        swap(p, q);
    }

    if (!flag)
        printf("未找到路径\n");

    return 0;
}