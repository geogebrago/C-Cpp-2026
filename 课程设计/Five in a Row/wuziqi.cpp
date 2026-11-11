// EMPTY=-1, BLACK=0, WHITE=1
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
int board[board_size][board_size],candidate_count[board_size][board_size],score[2];
int dr[4]={1,0,1,1},dc[4]={0,1,1,-1};
double linecathe[4][board_size][board_size][2];
int viscount;
bool timeflag;
int alivestone;
int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
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
                cin>>input;
                if (input=="Q"||input=="q")
                {
                    printf("已退出");
                    exit(0);
                }
                if (input=="U"||input=="u")
                {
                    if (!history.empty())
                    {
                        int r=history.back().first,c=history.back().second;
                        undo(r,c);
                        history.pop_back();
                        turn=1-turn;
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
            turn=1-turn;
        }
        resetboard();
    }
    return 0;
}