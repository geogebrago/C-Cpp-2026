//
// Created by 26346 on 2026/11/11.
//

#ifndef CCPP2026_UI_H
#define CCPP2026_UI_H
#include<bits/stdc++.h>
using namespace std;
void print_board()
{
    printf(" ");
    for (int j=0;j<board_size;j++)
    {
        printf("%d", (j+1)%10);
    }
    puts("");

    for (int i=0;i<board_size;i++)
    {
        printf("%d", (i+1)%10);
        for (int j=0;j<board_size;j++)
        {
            if (board[i][j]==EMPTY)
                printf(" ");
            else if (board[i][j]==BLACK)
                printf("⚫︎");
            else
                printf("o");
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
    printf("菜单\n");
    printf("1. 开始游戏\n");
    printf("2. 退出游戏\n");
    int op;
    if (!safe_input(op))
        exit(1);
    switch (op)
    {
    case 1:
        {
            initscore();
        }
    case 2:
        {
            printf("已退出");
            exit(0);
        }
    }
}
#endif //CCPP2026_UI_H
