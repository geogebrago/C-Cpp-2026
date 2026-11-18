#include<iostream>
#include<Windows.h>
#include "../../safe_input.h"
#include "wuziqi.h"
#include "basic.h"
#include "estimate.h"
#include "win.h"
#include "move.h"
#include "way.h"
#include "minmax.h"
#include "ai.h"
#include "UI.h"
using namespace std;
int board[boardSize][boardSize],nearCount[boardSize][boardSize];
double score[2];
int dr[4]={1,0,1,1},dc[4]={0,1,1,-1};
double lineScore[4][boardSize][boardSize][2];
int alivestone;
int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    resetboard();
    while (1)
    {
        help();
        int human=BLACK,ai=WHITE;
        int turn=BLACK;
        vector< pair<int,int> > history;
        while (1)
        {
            print_board();
            if (turn==human)
            {
                printf("请输入落子位置：");
                string input;
                if (!safe_input(input))
                {
                    printf("已退出");
                    exit(0);
                }
                input=trim(input);
                if (input=="Q"||input=="q")
                {
                    printf("已退出");
                    exit(0);
                }
                if (input=="U"||input=="u")
                {
                    int steps=0;
                    while (steps<2&&!history.empty())
                    {
                        int r=history.back().first,c=history.back().second;
                        history.pop_back();
                        undo(r,c);
                        ++steps;
                    }
                    if (steps==0) printf("没有可以悔的棋\n");
                    turn=human;
                    continue;
                }
                if (input=="H"||input=="h")
                {
                    vector<Move> tips=generateMoves();
                    if (tips.empty()) printf("没有可下的点\n");
                    else
                    {
                        orderMoves(tips,human);
                        printf("建议落子：%d+%d\n",tips[0].r+1,tips[0].c+1);
                    }
                    continue;
                }
                int r,c;
                if (sscanf(input.c_str(),"%d+%d",&r,&c)!=2||!in(r-1,c-1)||board[r-1][c-1]!=EMPTY)
                {
                    printf("输入错误，请重新输入\n");
                    continue;
                }
                --r;--c;
                domove(r,c,human);
                history.push_back({r,c});
            }
            else
            {
                AIResult res=aiMove(ai);
                if (res.r==-1||res.c==-1)
                {
                    printf("AI 没有可下的点，游戏结束\n");
                    break;
                }
                domove(res.r,res.c,ai);
                history.push_back({res.r,res.c});
            }
            if (win(history.back().first,history.back().second))
            {
                print_board();
                if (turn==human) printf("你赢了！\n");
                else printf("AI 赢了！\n");
                break;
            }
            if (alivestone>=boardSize*boardSize)
            {
                print_board();
                printf("平局！\n");
                break;
            }
            turn=1-turn;
        }
        resetboard();
    }
    return 0;
}