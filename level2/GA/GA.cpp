#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int N = 20;
const int POPULATION_SIZE = 2000;
const int MAX_GENE_LENGTH = N * N;
const int GENERATIONS = 2000;

int ROWS, COLS, GENE_LENGTH;
int start_x, start_y, end_x, end_y;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

mt19937 rd(time(NULL));
vector<string> maze;

struct nd
{
    int s[MAX_GENE_LENGTH];
    int x, y, fitness;
};

nd p[POPULATION_SIZE], np[POPULATION_SIZE];

int dis(int x, int y)
{
    return abs(x - end_x) + abs(y - end_y);
}

bool valid(int x, int y)
{
    return x >= 0 && x < ROWS &&
           y >= 0 && y < COLS &&
           maze[x][y] != '#';
}

bool input_map()
{
    cout << "请输入地图行数和列数：\n";

    if (!(cin >> ROWS >> COLS))
    {
        cout << "输入错误！\n";
        return false;
    }

    if (ROWS < 1 || ROWS > N || COLS < 1 || COLS > N)
    {
        cout << "地图大小错误！行数和列数必须在1~20之间。\n";
        return false;
    }

    maze.resize(ROWS);
    int sc = 0, ec = 0;

    cout << "请输入地图（#墙 .路 @起点 $终点）：\n";

    for (int i = 0; i < ROWS; ++i)
    {
        if (!(cin >> maze[i]) || maze[i].size() != COLS)
        {
            cout << "地图输入错误！每行必须恰好有"
                 << COLS << "个字符。\n";
            return false;
        }

        for (int j = 0; j < COLS; ++j)
        {
            char ch = maze[i][j];

            if (ch == '@')
            {
                start_x = i;
                start_y = j;
                ++sc;
            }
            else if (ch == '$')
            {
                end_x = i;
                end_y = j;
                ++ec;
            }
            else if (ch != '#' && ch != '.')
            {
                cout << "地图包含非法字符！\n";
                return false;
            }
        }
    }

    if (sc != 1 || ec != 1)
    {
        cout << "地图必须有且只有一个起点和一个终点！\n";
        return false;
    }

    GENE_LENGTH = ROWS * COLS;
    return true;
}

int calc_fitness(nd &a)
{
    a.x = start_x;
    a.y = start_y;
    int fitness = 0;

    for (int i = 0; i < GENE_LENGTH; ++i)
    {
        int d = a.s[i];

        if (d < 0 || d >= 4)
        {
            fitness -= 100;
            continue;
        }

        int nx = a.x + dx[d];
        int ny = a.y + dy[d];

        if (!valid(nx, ny))
        {
            fitness -= 100;
            continue;
        }

        int old_dis = dis(a.x, a.y);

        a.x = nx;
        a.y = ny;

        if (a.x == end_x && a.y == end_y)
            return 1000000 + (GENE_LENGTH - i) * 1000;

        fitness += old_dis - dis(a.x, a.y);
    }

    fitness -= dis(a.x, a.y);
    return fitness;
}

nd &select_one()
{
    nd *a = &p[rd() % POPULATION_SIZE];
    nd *b = &p[rd() % POPULATION_SIZE];
    nd *c = &p[rd() % POPULATION_SIZE];

    nd *best = a->fitness >= b->fitness ? a : b;
    best = best->fitness >= c->fitness ? best : c;

    return *best;
}

void print_path(const nd &a)
{
    vector<string> result = maze;
    int x = start_x, y = start_y;
    int steps = 0;

    for (int i = 0; i < GENE_LENGTH; ++i)
    {
        int d = a.s[i];

        if (d < 0 || d >= 4)
            continue;

        int nx = x + dx[d];
        int ny = y + dy[d];

        if (!valid(nx, ny))
            continue;

        x = nx;
        y = ny;
        ++steps;

        if (result[x][y] == '.')
            result[x][y] = '*';

        if (x == end_x && y == end_y)
        {
            cout << "找到路径！\n";
            cout << "路径步数：" << steps << '\n';

            for (int i = 0; i < ROWS; ++i)
                cout << result[i] << '\n';

            return;
        }
    }

    cout << "当前最优个体没有到达终点。\n";

    for (int i = 0; i < ROWS; ++i)
        cout << result[i] << '\n';
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (!input_map())
        return 0;

    clock_t start_time = clock();

    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        int x = start_x, y = start_y;

        for (int j = 0; j < GENE_LENGTH; ++j)
        {
            int dir[4], cnt = 0;

            for (int k = 0; k < 4; ++k)
            {
                int nx = x + dx[k];
                int ny = y + dy[k];

                if (valid(nx, ny))
                    dir[cnt++] = k;
            }

            int d;

            if (cnt == 0)
                d = rd() % 4;
            else
            {
                d = dir[0];

                for (int k = 1; k < cnt; ++k)
                {
                    int t = dir[k];

                    if (dis(x + dx[t], y + dy[t]) <
                        dis(x + dx[d], y + dy[d]))
                        d = t;
                }

                if (rd() % 100 >= 80)
                    d = dir[rd() % cnt];
            }

            p[i].s[j] = d;

            int nx = x + dx[d];
            int ny = y + dy[d];

            if (valid(nx, ny))
            {
                x = nx;
                y = ny;
            }
        }
    }

    int best_fitness = INT_MIN;
    nd best{};

    for (int generation = 0; generation < GENERATIONS; ++generation)
    {
        for (int i = 0; i < POPULATION_SIZE; ++i)
            p[i].fitness = calc_fitness(p[i]);

        int best_id = 0;

        for (int i = 1; i < POPULATION_SIZE; ++i)
        {
            if (p[i].fitness > p[best_id].fitness)
                best_id = i;
        }

        if (p[best_id].fitness > best_fitness)
        {
            best_fitness = p[best_id].fitness;
            best = p[best_id];
        }

        if (best_fitness >= 1000000)
            break;

        np[0] = p[best_id];

        int mutation_rate = max(1, 5 - generation * 4 / GENERATIONS);

        for (int i = 1; i < POPULATION_SIZE; i += 2)
        {
            nd &a = select_one();
            nd &b = select_one();

            np[i] = a;

            if (i + 1 < POPULATION_SIZE)
                np[i + 1] = b;

            int pos = rd() % GENE_LENGTH;

            if (i + 1 < POPULATION_SIZE)
            {
                for (int j = pos; j < GENE_LENGTH; ++j)
                    swap(np[i].s[j], np[i + 1].s[j]);
            }

            for (int j = 0; j < GENE_LENGTH; ++j)
            {
                if (rd() % 100 < mutation_rate)
                    np[i].s[j] = rd() % 4;

                if (i + 1 < POPULATION_SIZE &&
                    rd() % 100 < mutation_rate)
                    np[i + 1].s[j] = rd() % 4;
            }
        }

        for (int i = 0; i < POPULATION_SIZE; ++i)
            p[i] = np[i];
    }

    if (best_fitness >= 1000000)
        print_path(best);
    else
    {
        cout << "未找到路径，可以尝试调整种群或迭代次数。\n";

        cout << "当前最优路线：\n";
        print_path(best);
    }

    clock_t end_time = clock();

    cout << fixed << setprecision(3);
    cout << "运行时间："
         << (double)(end_time - start_time) / CLOCKS_PER_SEC
         << " 秒\n";

    return 0;
}