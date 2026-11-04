//0 empty -1 black 1 white
#include<iostream>
#include<Windows.h>
#include "../../safe_input.h"
using namespace std;
const int board_size=15;
const int wu=100000;
const int huosi=10000;
const int chongsi=1200;
const int huosan=1000;
const int chongsan=100;
const int huoer=10;
const int chonger=1;
const int INF=1e18;
int board[board_size][board_size],next[board_size][board_size],score[2];
int dr[4]={1,0,1,1},dc[4]={0,1,1,-1};
bool in(int x,int y)
{
    return x>=0&&x<board_size&&y>=0&&y<board_size;
}
bool win(int r,int c)
{
    int who=board[r][c];
    for (int d=0;d<4;++d)
    {
        int cnt=1;
        for (int k=1;k<=4;++k)
        {
            int nr=r+k*dr[d],nc=c+k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=who) break;
            cnt++;
        }
        for (int k=1;k<=4;++k)
        {
            int nr=r-k*dr[d],nc=c-k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=who) break;
            cnt++;
        }
        if (cnt>=5) return 1;
    }
    return 0;
}
int patternscore(int cnt,int huo)
{
    if (cnt>=5) return wu;
    if (cnt==4)
    {
        if (huo) return huosi;
        else return chongsi;
    }
    if (cnt==3)
    {
        if (huo) return huosan;
        else return chongsan;
    }
    if (cnt==2)
    {
        if (huo) return huoer;
        else return chonger;
    }
    return 0;
}
int
int point_value(int r,int c,int who)
{
    for (int d=0;d<4;++d)
    {
        int cnt=1,huo=0;
        for (int k=1;k<=4;++k)
        {
            int nr=r+k*dr[d],nc=c+k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=who)
            {
                if (in(nr,nc)&&board[nr][nc]==0) ++huo;
                break;
            }
            cnt++;
        }
        for (int k=1;k<=4;++k)
        {
            int nr=r-k*dr[d],nc=c-k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=who||cnt==5)
            {
                if (in(nr,nc)&&board[nr][nc]==0) ++huo;
                break;
            }
            cnt++;
        }
        score[who]+=patternscore(cnt,huo);
    }
}
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
            if (board[i][j]==0)
                printf(" ");
            else if (board[i][j]==-1)
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
void linestart(int r,int c,int d, int &r0,int &c0)
{
    switch (d)
    {
    case 0:
        {
            r0=0,c0=c;
            break;
        }
    case 1:
        {
            r0=r,c0=0;
            break;
        }
    case 2:
        {
            int tp=min(r,c);
            r0=r-tp,c0=c-tp;
            break;
        }
    case 3:
        {
            int tp=min(r,board_size-1-c);
            r0=r-tp,c0=c+tp;
            break;
        }
    }
}
int getLine(int r,int c,int d,int *line)
{
    for (size_t i=0;i<board.size();i++)
    {

    }
}
void initscore()
{
    int line[board_size];
    for (int d=0;d<4;++d)
    {
        for (int i=0;i<board_size;i++)
            for (int j=0;j<board_size;j++)
            {
                if (in(i+dr[d],j+dc[d]))
                {
                   continue;
                }
                int len=getLine(i,j,k,line);
            }
    }
}
int widthai(int ply)
{
    if (ply==1) return 14;
    if (ply==2) return 12;
    if (ply==3) return 10;
    if (ply==4) return 8;
    return 6;
}
void resetboard()
{
    score[0]=score[1]=0;
    for (int i=0;i<board_size;i++)
        for (int j=0;j<board_size;j++)
            board[i][j]=next[i][j]=0;

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
int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    while (1)
    {
        help();

        resetboard();
    }
    return 0;
}