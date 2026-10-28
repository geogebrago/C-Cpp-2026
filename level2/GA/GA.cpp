#include <bits/stdc++.h>
#include <windows.h>
#include "../../safe_input.h"
using namespace std;
const int N = 20;
const int POPULATION_SIZE = 2000;
const int MAX_GENE_LENGTH = N * N;
const int GENERATIONS = 2000;
int ROWS, COLS, GENE_LENGTH;
int sx, sy, ex, ey;
char mp[N][N];
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
mt19937 rd(time(nullptr));
struct nd
{
    int s[MAX_GENE_LENGTH];
    int fitness;
    int x, y;
};
nd p[POPULATION_SIZE], np[POPULATION_SIZE];
bool valid(int x, int y)
{
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && mp[x][y] != '#';
}
bool input_map()
{
    if (!safe_input(ROWS, "请输入地图行数（最大20）：") || !safe_input(COLS, "请输入地图列数（最大20）："))
        return false;
    if (ROWS < 1 || ROWS > N || COLS < 1 || COLS > N)
    {
        cout << "地图尺寸错误！行数和列数必须在 1~20 之间。\n";
        return false;
    }
    int sc = 0, ec = 0;
    cout << "请输入地图（# 为墙，. 为道路，@ 为起点，$ 为终点）：\n";
    for (int i = 0; i < ROWS; ++i)
    {
        string s;
        if (!safe_input(s) || (int)s.size() != COLS)
        {
            cout << "第 " << i + 1 << " 行长度错误！\n";
            return false;
        }
        for (int j = 0; j < COLS; ++j)
        {
            if (s[j] != '#' && s[j] != '.' && s[j] != '@' && s[j] != '$')
            {
                cout << "地图中存在非法字符！\n";
                return false;
            }
            mp[i][j] = s[j];
            if (s[j] == '@')
            {
                sx = i;
                sy = j;
                ++sc;
            }
            else if (s[j] == '$')
            {
                ex = i;
                ey = j;
                ++ec;
            }
        }
    }
    if (sc != 1 || ec != 1)
    {
        cout << "地图必须恰好包含一个起点和一个终点！\n";
        return false;
    }
    GENE_LENGTH = ROWS * COLS;
    return true;
}
bool can_move(int x, int y, int d)
{
    return valid(x + dx[d], y + dy[d]);
}
bool has_other_move(int x, int y, int last_dir)
{
    if (last_dir == -1)
        return false;
    int rev = (last_dir + 2) % 4;
    for (int d = 0; d < 4; ++d)
    {
        if (d == rev)
            continue;
        if (can_move(x, y, d))
            return true;
    }
    return false;
}
int calc_fitness(nd &a)
{
    int x = sx, y = sy;
    int last_dir = -1;
    int fitness = 0;
    for (int i = 0; i < GENE_LENGTH; ++i)
    {
        int d = a.s[i];
        if (d < 0 || d >= 4)
        {
            fitness -= 100;
            continue;
        }
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (!valid(nx, ny))
        {
            fitness -= 100;
            continue;
        }
        if (last_dir != -1)
        {
            int rev = (last_dir + 2) % 4;
            if (d == rev && has_other_move(x, y, last_dir))
            {
                fitness -= 20;
                continue;
            }
        }
        x = nx;
        y = ny;
        last_dir = d;
        fitness += 10;
        int dis = abs(x - ex) + abs(y - ey);
        fitness -= dis;
        if (x == ex && y == ey)
        {
            a.x = x;
            a.y = y;
            a.fitness = 1000000 + (GENE_LENGTH - i) * 1000 + fitness;
            return a.fitness;
        }
    }
    a.x = x;
    a.y = y;
    a.fitness = fitness;
    return fitness;
}
int select_one()
{
    int best = rd() % POPULATION_SIZE;
    for (int i = 1; i < 3; ++i)
    {
        int id = rd() % POPULATION_SIZE;
        if (p[id].fitness > p[best].fitness)
            best = id;
    }
    return best;
}
void init_one(nd &a)
{
    int x = sx, y = sy;
    int last_dir = -1;
    for (int i = 0; i < GENE_LENGTH; ++i)
    {
        int cand[4], cnt = 0;
        int rev = -1;
        if (last_dir != -1)
            rev = (last_dir + 2) % 4;
        bool other = has_other_move(x, y, last_dir);
        for (int d = 0; d < 4; ++d)
        {
            if (!can_move(x, y, d))
                continue;
            if (other && d == rev)
                continue;
            cand[cnt++] = d;
        }
        if (cnt == 0)
        {
            a.s[i] = rd() % 4;
            continue;
        }
        int best[4], best_cnt = 0;
        int mindis = INT_MAX;
        for (int j = 0; j < cnt; ++j)
        {
            int d = cand[j];
            int nx = x + dx[d];
            int ny = y + dy[d];
            int dis = abs(nx - ex) + abs(ny - ey);
            if (dis < mindis)
            {
                mindis = dis;
                best_cnt = 0;
                best[best_cnt++] = d;
            }
            else if (dis == mindis)
            {
                best[best_cnt++] = d;
            }
        }
        int d;
        if (rd() % 100 < 70)
            d = best[rd() % best_cnt];
        else
            d = cand[rd() % cnt];
        a.s[i] = d;
        x += dx[d];
        y += dy[d];
        last_dir = d;
    }
    a.fitness = 0;
}
void print_path(nd &a)
{
    char out[N][N];
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            out[i][j] = mp[i][j];
    int x = sx, y = sy;
    int last_dir = -1;
    int steps = 0;
    out[x][y] = '@';
    for (int i = 0; i < GENE_LENGTH; ++i)
    {
        int d = a.s[i];
        if (d < 0 || d >= 4)
            continue;
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (!valid(nx, ny))
            continue;
        if (last_dir != -1)
        {
            int rev = (last_dir + 2) % 4;
            if (d == rev && has_other_move(x, y, last_dir))
                continue;
        }
        x = nx;
        y = ny;
        last_dir = d;
        ++steps;
        if (out[x][y] == '.')
            out[x][y] = '*';
        if (x == ex && y == ey)
            break;
    }
    out[sx][sy] = '@';
    out[ex][ey] = '$';
    cout << "\n寻路结果：\n";
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
            cout << out[i][j];
        cout << '\n';
    }
    cout << "实际移动步数：" << steps << '\n';
    if (x == ex && y == ey)
        cout << "成功到达终点！\n";
    else
        cout << "本次未能到达终点。\n";
}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (!input_map())
        return 0;
    clock_t begin = clock();
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        init_one(p[i]);
        calc_fitness(p[i]);
    }
    int best_id = 0;
    for (int gen = 0; gen < GENERATIONS; ++gen)
    {
        best_id = 0;
        for (int i = 1; i < POPULATION_SIZE; ++i)
        {
            if (p[i].fitness > p[best_id].fitness)
                best_id = i;
        }
        if (gen % 50 == 0 || p[best_id].fitness >= 1000000)
        {
            cout << "第 " << gen
                 << " 代，最高适应度："
                 << p[best_id].fitness << '\n';
        }
        if (p[best_id].fitness >= 1000000)
            break;
        np[0] = p[best_id];
        int mutation_rate = max(1, 5 - gen * 4 / GENERATIONS);
        for (int i = 1; i < POPULATION_SIZE; i += 2)
        {
            int a = select_one();
            int b = select_one();
            np[i] = p[a];
            if (i + 1 < POPULATION_SIZE)
                np[i + 1] = p[b];
            if (GENE_LENGTH > 1 && rd() % 100 < 80)
            {
                int pos = rd() % (GENE_LENGTH - 1) + 1;
                for (int j = pos; j < GENE_LENGTH; ++j)
                    swap(np[i].s[j], np[i + 1 < POPULATION_SIZE ? i + 1 : i].s[j]);
            }
            for (int j = 0; j < GENE_LENGTH; ++j)
            {
                if (rd() % 100 < mutation_rate)
                    np[i].s[j] = rd() % 4;
                if (i + 1 < POPULATION_SIZE && rd() % 100 < mutation_rate)
                    np[i + 1].s[j] = rd() % 4;
            }
        }
        for (int i = 0; i < POPULATION_SIZE; ++i)
            calc_fitness(np[i]);
        for (int i = 0; i < POPULATION_SIZE; ++i)
            p[i] = np[i];
    }
    best_id = 0;
    for (int i = 1; i < POPULATION_SIZE; ++i)
    {
        if (p[i].fitness > p[best_id].fitness)
            best_id = i;
    }
    cout << "\n最终最高适应度：" << p[best_id].fitness << '\n';
    print_path(p[best_id]);
    clock_t end = clock();
    cout << "所用的时间：" << double(end - begin) / CLOCKS_PER_SEC << " 秒\n";
    return 0;
}