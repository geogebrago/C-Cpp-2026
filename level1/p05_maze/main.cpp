#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "main2.h"
#include "../../safe_input.h"
int rows, cols;
int current_x, current_y;
int end_x, end_y;
int player_x, player_y;
int direction_x[5] = {0, 1, -1, 0, 0};
int direction_y[5] = {0, 0, 0, 1, -1};
bool visited[MAX_SIZE][MAX_SIZE];
typedef struct
{
    int x, y, direction;
} Candidate;
Candidate *frontier = NULL;
int frontier_size = 0;
int frontier_capacity = 0;
bool is_inside(int x, int y)
{
    return x >= 1 && x <= rows && y >= 1 && y <= cols;
}
void frontier_push(Candidate candidate)
{
    if (frontier_size == frontier_capacity)
    {
        frontier_capacity = frontier_capacity == 0 ? 16 : frontier_capacity * 2;
        Candidate *new_frontier = (Candidate *)realloc(frontier, frontier_capacity * sizeof(Candidate));
        if (new_frontier == NULL)
        {
            exit(1);
        }
        frontier = new_frontier;
    }
    frontier[frontier_size++] = candidate;
}
void frontier_pop_back()
{
    frontier_size--;
}
void add_frontier()
{
    for (int i = 1; i <= 4; ++i)
    {
        int tx = current_x + direction_x[i];
        int ty = current_y + direction_y[i];
        int nx = current_x + 2 * direction_x[i];
        int ny = current_y + 2 * direction_y[i];
        if (is_inside(nx, ny) && !visited[nx][ny])
        {
            Candidate candidate;
            candidate.x = tx;
            candidate.y = ty;
            candidate.direction = i;
            frontier_push(candidate);
        }
    }
}
void handle_input()
{
    int ch = _getch();
    int d = -1;
    if (ch == 'w' || ch == 'W')
        d = 2;
    else if (ch == 's' || ch == 'S')
        d = 1;
    else if (ch == 'a' || ch == 'A')
        d = 4;
    else if (ch == 'd' || ch == 'D')
        d = 3;
    else if (ch == 224)
    {
        ch = _getch();
        if (ch == 72)//上
            d = 2;
        else if (ch == 80)//下
            d = 1;
        else if (ch == 75)//左
            d = 4;
        else if (ch == 77)//右
            d = 3;
    }
    if (d != -1)
    {
        int nx = player_x + direction_x[d];
        int ny = player_y + direction_y[d];
        if (is_inside(nx, ny) && visited[nx][ny])
        {
            player_x = nx;
            player_y = ny;
        }
    }
    system("cls");
    if (player_x == end_x && player_y == end_y)
    {
        puts("END");
        exit(0);
    }
}
int main()
{
    srand((unsigned)time(NULL));
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (!safe_input(rows, "请输入生成的迷宫的行数（大于3且小于1000）：\n") || !safe_input(cols, "请输入生成的迷宫的列数（大于3且小于1000）：\n"))
        return 0;
    if (rows <= 3 || cols <= 3 || rows >= MAX_SIZE || cols >= MAX_SIZE)
        return 0;
    current_x = rand() % rows + 1;
    current_y = rand() % cols + 1;
    player_x = current_x;
    player_y = current_y;
    visited[current_x][current_y] = true;
    add_frontier();
    while (frontier_size > 0)
    {
        int sz = frontier_size;
        int num = rand() % sz;
        int x = frontier[num].x;
        int y = frontier[num].y;
        int d = frontier[num].direction;
        int nx = x + direction_x[d];
        int ny = y + direction_y[d];
        if (is_inside(nx, ny) && !visited[nx][ny])
        {
            visited[x][y] = true;
            visited[nx][ny] = true;
            current_x = nx;
            current_y = ny;
            add_frontier();
        }
        Candidate temp = frontier[num];
        frontier[num] = frontier[frontier_size - 1];
        frontier[frontier_size - 1] = temp;
        frontier_pop_back();
    }
    free(frontier);
    frontier = NULL;
    int max_distance = -1;
    end_x = player_x;
    end_y = player_y;
    for (int i = 1; i <= rows; ++i)
    {
        for (int j = 1; j <= cols; ++j)
        {
            if (visited[i][j])
            {
                int distance = abs(i - player_x) + abs(j - player_y);
                if (distance > max_distance)
                {
                    max_distance = distance;
                    end_x = i;
                    end_y = j;
                }
            }
        }
    }
    while (true)
    {
        print_board();
        handle_input();
    }
    return 0;
}