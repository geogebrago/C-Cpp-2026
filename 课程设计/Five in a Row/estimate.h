#ifndef CCPP2026_ESTIMATE_H
#define CCPP2026_ESTIMATE_H
#include <algorithm>
#include "basic.h"
#include "wuziqi.h"

const int wu=100000;
const int huosi=10000;
const int chongsi=1200;
const int huosan=1000;
const int chongsan=100;
const int huoer=10;
const int chonger=1;
int patternscore(int cnt,int huo)
{
    if (cnt>=5) return wu;
    if (huo<=0) return 0;
    if (cnt==4)
    {
        if (huo>=2) return huosi;
        return chongsi;
    }
    if (cnt==3)
    {
        if (huo>=2) return huosan;
        return chongsan;
    }
    if (cnt==2)
    {
        if (huo>=2) return huoer;
        return chonger;
    }
    return 0;
}
int getLine(int r,int c,int d,int *line)
{
    int length = 0;
    while (in(r,c))
    {
        line[length++] = board[r][c];
        r += dr[d];
        c += dc[d];
    }
    return length;
}
double linescore(const int *line,int len,int player)
{
    double tot=0;
    int i=0;
    while (i<len)
    {
        if (line[i]!=player)
        {
            ++i;
            continue;
        }
        int j=i;
        while (j<len&&line[j]==player) ++j;
        int cnt=j-i;
        int huo=0;
        if (i>0&&line[i-1]==EMPTY) ++huo;
        if (j<len&&line[j]==EMPTY) ++huo;
        tot+=patternscore(cnt,huo);
        if (cnt<4&&j+1<len&&line[j]==EMPTY&&line[j+1]==player)
        {
            int k=j+1;
            while (k<len&&line[k]==player) ++k;
            int cnt2=k-j-1;
            if (cnt2<4)
            {
                int all=cnt+cnt2+1;
                if (all>=5)
                {
                    tot+=chongsi;
                }
                else
                {
                    int huo2=0;
                    if (i>0&&line[i-1]==EMPTY) ++huo2;
                    if (k<len&&line[k]==EMPTY) ++huo2;
                    tot+=patternscore(all,huo2);
                }
                i=k;
                continue;
            }
        }
        i=j;
    }
    return tot;
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
            int tp=std::min(r,c);
            r0=r-tp,c0=c-tp;
            break;
        }
    case 3:
        {
            int tp=std::min(r,boardSize-1-c);
            r0=r-tp,c0=c+tp;
            break;
        }
    }
}
void refreshline(int r,int c)
{
    int line[boardSize];
    for (int d=0;d<4;++d)
    {
        int r0,c0;
        linestart(r,c,d,r0,c0);
        int len = getLine(r0,c0,d,line);
        for (int player=0;player<2;++player)
        {
            double v=linescore(line,len,player);
            score[player]+=v-lineScore[d][r][c][player];
            for (int offset=0; offset<len; ++offset)
            {
                int rr = r0 + offset * dr[d];
                int cc = c0 + offset * dc[d];
                lineScore[d][rr][cc][player] = v;
            }
        }
    }
}
void initscore()
{
    int line[boardSize];
    score[0]=score[1]=0;
    for (int d=0;d<4;++d)
    {
        for (int i=0;i<boardSize;i++)
            for (int j=0;j<boardSize;j++)
            {
                int r0,c0;
                linestart(i,j,d,r0,c0);
                int len=getLine(r0,c0,d,line);
                double vb=linescore(line,len,BLACK);
                double vw=linescore(line,len,WHITE);
                lineScore[d][i][j][0]=vb;
                lineScore[d][i][j][1]=vw;
                if (i==r0&&j==c0)
                {
                    score[0]+=vb;
                    score[1]+=vw;
                }
            }
    }
}
double evaluateBoard()
{
    return score[0] - score[1] * defense;
}
void resetboard()
{
    score[0]=score[1]=0;
    for (int i=0;i<boardSize;i++)
    {
        for (int j=0;j<boardSize;j++)
        {
            board[i][j]=EMPTY;
            nearCount[i][j]=0;
        }
    }
    alivestone=0;
    initscore();
}
#endif
