#ifndef CCPP2026_UI_H
#define CCPP2026_UI_H
#include<bits/stdc++.h>
using namespace std;
void print_board()
{
    printf("   ");
    for (int j=0;j<boardSize;j++)
        printf("%2d ", (j+1)%100);
    puts("");

    for (int i=0;i<boardSize;i++)
    {
        printf("%2d ", (i+1)%100);
        for (int j=0;j<boardSize;j++)
        {
            if (board[i][j]==EMPTY)
                printf(" . ");
            else if (board[i][j]==BLACK)
                printf(" X ");
            else
                printf(" O ");
        }
        puts("");
    }

    printf("行+列 下一步\n");
    printf("Q 退出\n");
    printf("U 悔棋\n");
    printf("H 帮助落子\n");
}
void help()
{
    while (1)
    {
        printf("菜单\n");
        printf("1. 开始游戏\n");
        printf("2. 退出游戏\n");
        int op;
        if (!safe_input(op))
            exit(1);
        switch (op)
        {
        case 1:
            resetboard();
            return;
        case 2:
            {
                printf("已退出");
                exit(0);
            }
        default:
            printf("选项无效，请重新选择\n");
            break;
        }
    }
}
#endif
