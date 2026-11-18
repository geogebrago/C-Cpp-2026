#ifndef CCPP2026_MOVE_H
#define CCPP2026_MOVE_H
#include <algorithm>
#include "estimate.h"
#include "wuziqi.h"
void undo(int r,int c)
{
    --alivestone;
    board[r][c]=EMPTY;
    for (int i=std::max(0,r-2);i<=std::min(boardSize-1,r+2);++i)
        for (int j=std::max(0,c-2);j<=std::min(boardSize-1,c+2);++j)
            --nearCount[i][j];
    refreshline(r, c);
}
void domove(int r,int c,int player)
{
    ++alivestone;
    board[r][c]=player;
    for (int i=std::max(0,r-2);i<=std::min(boardSize-1,r+2);++i)
        for (int j=std::max(0,c-2);j<=std::min(boardSize-1,c+2);++j)
            ++nearCount[i][j];
    refreshline(r, c);
}
#endif
