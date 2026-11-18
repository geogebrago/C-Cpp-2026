
#ifndef CCPP2026_WIN_H
#define CCPP2026_WIN_H
#include "basic.h"
bool win(int r,int c)
{
    if (!in(r,c)) return 0;
    int player=board[r][c];
    if (player==EMPTY) return 0;
    for (int d=0;d<4;++d)
    {
        int cnt=1;
        for (int k=1;k<=4;++k)
        {
            int nr=r+k*dr[d],nc=c+k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=player) break;
            cnt++;
        }
        for (int k=1;k<=4;++k)
        {
            int nr=r-k*dr[d],nc=c-k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=player) break;
            cnt++;
        }
        if (cnt>=5) return 1;
    }
    return 0;
}
#endif
