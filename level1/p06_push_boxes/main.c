#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define MAX_SIZE 1005
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static int menu_choice, player_x, player_y, steps, level;
static int rows, cols;
static int target_x[MAX_SIZE], target_y[MAX_SIZE];
static int box_x[MAX_SIZE], box_y[MAX_SIZE];
static int target_count, box_count;
static int scores[4];
static int direction_x[5] = {0, 1, -1, 0, 0};
static int direction_y[5] = {0, 0, 0, 1, -1};
static char grid[MAX_SIZE][MAX_SIZE];
static char source_path[MAX_SIZE];

static int clear_input_line()
{
    int ch;

    while ((ch = getchar()) != '\n')
    {
        if (ch == EOF)
            return 0;
    }

    return 1;
}

static int get_path(char *path, const char *name)
{
    char temp[MAX_SIZE];
    int len;

    if (source_path[0] == '\0')
        len = snprintf(temp, MAX_SIZE, "%s", name);
    else
        len = snprintf(temp, MAX_SIZE, "%s\\%s", source_path, name);

    if (len < 0 || len >= MAX_SIZE)
        return 0;

    DWORD path_length =
        GetFullPathNameA(temp, MAX_SIZE, path, NULL);

    return path_length > 0 && path_length < MAX_SIZE;
}


static int get_score_path(char *path, const char *name)
{
    int len;

    if (source_path[0] == '\0')
        len = snprintf(path, MAX_SIZE, "%s", name);
    else
        len = snprintf(path, MAX_SIZE, "%s\\%s",
                       source_path, name);

    return len >= 0 && len < MAX_SIZE;
}


static int is_inside(int x, int y)
{
    return x >= 1 && x <= rows && y >= 1 && y <= cols;
}


static int find_target(int x, int y)
{
    for (int i = 1; i <= target_count; ++i)
    {
        if (target_x[i] == x && target_y[i] == y)
            return i;
    }

    return 0;
}


static int find_box(int x, int y)
{
    for (int i = 1; i <= box_count; ++i)
    {
        if (box_x[i] == x && box_y[i] == y)
            return 1;
    }

    return 0;
}


static int check_map()
{
    int count_player = 0;
    int count_target = 0;
    int count_box = 0;

    for (int i = 1; i <= rows; ++i)
    {
        for (int j = 1; j <= cols; ++j)
        {
            if (grid[i][j] == '@')
                ++count_player;
            else if (grid[i][j] == '.')
                ++count_target;
            else if (grid[i][j] == '$')
                ++count_box;
            else if (grid[i][j] != '#' &&
                     grid[i][j] != ' ')
            {
                return 0;
            }

            if (count_target >= MAX_SIZE ||
                count_box >= MAX_SIZE)
            {
                return 0;
            }
        }
    }

    if (count_player != 1 ||
        count_target != count_box ||
        count_target == 0)
    {
        return 0;
    }

    return 1;
}


static int load_level_file(const char *file)
{
    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        printf("文件打开失败: %s\n", file);
        return 0;
    }

    memset(grid, ' ', sizeof(grid));
    rows = 0;
    cols = 0;

    char line[MAX_SIZE + 2];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int len = (int)strlen(line);
        int has_newline =
            len > 0 && line[len - 1] == '\n';

        while (len > 0 &&
               (line[len - 1] == '\n' ||
                line[len - 1] == '\r'))
        {
            line[--len] = '\0';
        }

        if (rows == 0 &&
            len >= 3 &&
            (unsigned char)line[0] == 0xEF &&
            (unsigned char)line[1] == 0xBB &&
            (unsigned char)line[2] == 0xBF)
        {
            memmove(line, line + 3, len - 2);
            len -= 3;
        }

        if (len == 0)
            continue;

        if (len >= MAX_SIZE ||
            rows + 1 >= MAX_SIZE)
        {
            fclose(fp);

            printf("地图太大，行数和列数最多为%d。\n",
                   MAX_SIZE - 1);

            return 0;
        }

        if (!has_newline && !feof(fp))
        {
            fclose(fp);
            printf("地图某一行过长或文件格式错误。\n");
            return 0;
        }

        ++rows;
        cols = MAX(cols, len);

        for (int j = 1; j <= len; ++j)
            grid[rows][j] = line[j - 1];
    }

    if (ferror(fp))
    {
        fclose(fp);
        printf("读取地图文件失败。\n");
        return 0;
    }

    fclose(fp);

    if (rows == 0 || cols == 0)
    {
        printf("地图为空\n");
        return 0;
    }

    if (!check_map())
    {
        printf("地图格式错误：请检查字符、玩家数量、");
        printf("箱子和目标数量及容量。\n");
        return 0;
    }

    return 1;
}


static void setup_level()
{
    target_count = 0;
    box_count = 0;
    steps = 0;
    player_x = 0;
    player_y = 0;

    for (int i = 1; i <= rows; ++i)
    {
        for (int j = 1; j <= cols; ++j)
        {
            if (grid[i][j] == '@')
            {
                player_x = i;
                player_y = j;
                grid[i][j] = ' ';
            }
            else if (grid[i][j] == '.')
            {
                target_x[++target_count] = i;
                target_y[target_count] = j;
                grid[i][j] = ' ';
            }
            else if (grid[i][j] == '$')
            {
                box_x[++box_count] = i;
                box_y[box_count] = j;
                grid[i][j] = ' ';
            }
        }
    }
}


static void print_board()
{
    printf("\033[2J\033[H");

    printf("==============================\n");
    printf("         推箱子小游戏\n");
    printf("==============================\n");
    printf("关卡: %d    步数: %d\n\n", level, steps);

    for (int i = 1; i <= rows; ++i)
    {
        for (int j = 1; j <= cols; ++j)
        {
            if (i == player_x && j == player_y)
                putchar('@');
            else if (find_box(i, j) &&
                     find_target(i, j))
                putchar('*');
            else if (find_box(i, j))
                putchar('$');
            else if (find_target(i, j))
                putchar('.');
            else
                putchar(grid[i][j]);
        }

        putchar('\n');
    }

    printf("\nWASD上下左右移动  R重开  Q返回\n");
}


static int check_win()
{
    for (int i = 1; i <= target_count; ++i)
    {
        if (!find_box(target_x[i], target_y[i]))
            return 0;
    }

    return 1;
}


static int handle_input()
{
    int ch = _getch();

    if (ch == 'q' || ch == 'Q')
        return 0;

    if (ch == 'r' || ch == 'R')
        return 2;

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

        if (ch == 72)
            d = 2;
        else if (ch == 80)
            d = 1;
        else if (ch == 75)
            d = 4;
        else if (ch == 77)
            d = 3;
    }

    if (d == -1)
        return 1;

    int next_x = player_x + direction_x[d];
    int next_y = player_y + direction_y[d];

    if (!is_inside(next_x, next_y) ||
        grid[next_x][next_y] == '#')
    {
        return 1;
    }

    int box_index = 0;

    for (int i = 1; i <= box_count; ++i)
    {
        if (box_x[i] == next_x &&
            box_y[i] == next_y)
        {
            box_index = i;
            break;
        }
    }

    if (box_index)
    {
        int box_next_x =
            next_x + direction_x[d];

        int box_next_y =
            next_y + direction_y[d];

        if (!is_inside(box_next_x, box_next_y) ||
            grid[box_next_x][box_next_y] == '#' ||
            find_box(box_next_x, box_next_y))
        {
            return 1;
        }

        box_x[box_index] = box_next_x;
        box_y[box_index] = box_next_y;
    }

    player_x = next_x;
    player_y = next_y;
    ++steps;

    return 1;
}


static void save_score()
{
    if (scores[level] == 0 ||
        steps < scores[level])
    {
        scores[level] = steps;
    }

    char file[MAX_SIZE];
    char temp_file[MAX_SIZE];

    if (!get_score_path(file, "score.txt") ||
        !get_score_path(temp_file, "score.tmp"))
    {
        printf("成绩文件路径过长，无法保存。\n");
        return;
    }

    FILE *fp = fopen(temp_file, "w");

    if (fp == NULL)
    {
        printf("无法创建成绩临时文件，保存失败。\n");
        return;
    }

    int ok = 1;

    for (int i = 1; i <= 3; ++i)
    {
        if (fprintf(fp, "%d\n", scores[i]) < 0)
        {
            ok = 0;
            break;
        }
    }

    if (fclose(fp) != 0)
        ok = 0;

    if (!ok)
    {
        remove(temp_file);
        printf("写入成绩失败。\n");
        return;
    }

    if (!MoveFileExA(temp_file, file,
                     MOVEFILE_REPLACE_EXISTING |
                     MOVEFILE_WRITE_THROUGH))
    {
        remove(temp_file);
        printf("保存成绩失败，错误代码：%lu\n",
               GetLastError());
        return;
    }

    printf("成绩已保存。\n");
}


static int load_game_level()
{
    char file[MAX_SIZE];
    int path_ok;

    if (level == 1)
        path_ok = get_path(file, "yi.txt");
    else if (level == 2)
        path_ok = get_path(file, "er.txt");
    else
        path_ok = get_path(file, "san.txt");

    if (!path_ok)
    {
        printf("地图文件路径过长。\n");
        _getch();
        return 0;
    }

    if (!load_level_file(file))
    {
        printf("请确认地图文件位于程序同目录: %s\n",
               file);
        _getch();
        return 0;
    }

    setup_level();
    return 1;
}


static void play_level()
{
    while (1)
    {
        print_board();

        if (check_win())
        {
            printf("\n恭喜通关！\n");
            printf("本关用了 %d 步\n", steps);

            save_score();

            printf("按任意键返回\n");
            _getch();
            return;
        }

        int key = handle_input();

        if (key == 0)
            return;

        if (key == 2)
        {
            if (!load_game_level())
                return;
        }
    }
}


static void load_scores()
{
    char file[MAX_SIZE];

    if (!get_score_path(file, "score.txt"))
    {
        for (int i = 1; i <= 3; ++i)
            scores[i] = 0;

        return;
    }

    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        for (int i = 1; i <= 3; ++i)
            scores[i] = 0;

        return;
    }

    for (int i = 1; i <= 3; ++i)
    {
        if (fscanf(fp, "%d", &scores[i]) != 1 ||
            scores[i] < 0)
        {
            scores[i] = 0;
        }
    }

    fclose(fp);
}


static int print_menu()
{
    printf("==============================\n");
    printf("         推箱子小游戏\n");
    printf("==============================\n");
    printf("1.关卡\n");
    printf("2.成绩\n");
    printf("3.退出\n");

    if (scanf("%d", &menu_choice) != 1)
    {
        if (feof(stdin))
            return 0;

        if (!clear_input_line())
            return 0;

        printf("请输入有效的数字。\n");
        return 1;
    }

    if (!clear_input_line())
        return 0;

    switch (menu_choice)
    {
    case 1:
    {
        printf("请输入关卡1~3\n");

        if (scanf("%d", &level) != 1)
        {
            if (feof(stdin))
                return 0;

            if (!clear_input_line())
                return 0;

            printf("请输入有效的关卡编号。\n");
            return 1;
        }

        if (!clear_input_line())
            return 0;

        if (level < 1 || level > 3)
        {
            printf("?\n");
            break;
        }

        if (load_game_level())
            play_level();

        break;
    }

    case 2:
    {
        printf("==============================\n");
        printf("           成绩\n");
        printf("==============================\n");

        for (int i = 1; i <= 3; ++i)
        {
            if (scores[i])
                printf("关卡 %d : %d 步\n",
                       i, scores[i]);
            else
                printf("关卡 %d : 暂无成绩\n", i);
        }

        printf("\n按任意键返回\n");
        _getch();

        break;
    }

    case 3:
    {
        printf("已退出\n");
        return 0;
    }

    default:
    {
        printf("?\n");
        break;
    }
    }

    return 1;
}


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    DWORD source_length =
        GetFullPathNameA(
            __FILE__,
            MAX_SIZE,
            source_path,
            NULL
        );

    if (source_length == 0 ||
        source_length >= MAX_SIZE)
    {
        source_path[0] = '\0';
    }
    else
    {
        char *last_slash =
            strrchr(source_path, '\\');

        char *last_forward_slash =
            strrchr(source_path, '/');

        if (last_forward_slash != NULL &&
            (last_slash == NULL ||
             last_forward_slash > last_slash))
        {
            last_slash = last_forward_slash;
        }

        if (last_slash != NULL)
            *last_slash = '\0';
        else
            source_path[0] = '\0';
    }

    load_scores();

    while (print_menu());

    printf("输入结束，程序退出。\n");

    return 0;
}
