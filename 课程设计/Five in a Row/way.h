#ifndef CCPP2026_WAY_H
#define CCPP2026_WAY_H
#include <algorithm>
#include <vector>
#include "basic.h"
#include "estimate.h"

bool moveCompare(const Move &a, const Move &b)
{
    return a.score > b.score;
}

std::vector<Move> generateMoves()
{
    std::vector<Move> moves;
    if (alivestone==0)
    {
        moves.push_back({boardSize/2,boardSize/2,0});
        return moves;
    }
    for (int i=0;i<boardSize;i++)
        for (int j=0;j<boardSize;j++)
            if (nearCount[i][j]>0&&board[i][j]==EMPTY)
                moves.push_back({i,j,0});
    if (moves.empty())
    {
        for (int i=0;i<boardSize;i++)
            for (int j=0;j<boardSize;j++)
                if (board[i][j]==EMPTY)
                    moves.push_back({i,j,0});
    }
    return moves;
}

int pointValue(int r, int c, int player)
{
    int value = 0;
    for (int d=0; d<4; ++d)
    {
        int count = 1;
        int open = 0;
        for (int direction : {-1, 1})
        {
            for (int k=1; k<=4; ++k)
            {
                int nr = r + direction * k * dr[d];
                int nc = c + direction * k * dc[d];
                if (!in(nr,nc) || board[nr][nc] != player)
                {
                    if (in(nr,nc) && board[nr][nc] == EMPTY)
                        ++open;
                    break;
                }
                ++count;
            }
        }
        value += patternscore(count, open);
    }
    return value;
}

void orderMoves(std::vector<Move>& moves, int player)
{
    int opponent = player == BLACK ? WHITE : BLACK;
    for (Move& move : moves)
    {
        move.score = std::max(static_cast<double>(pointValue(move.r, move.c, player)),
                              pointValue(move.r, move.c, opponent) * defense);
    }
    std::sort(moves.begin(), moves.end(), moveCompare);
}

int widthAt(int ply)
{
    if (ply <= 1) return 14;
    if (ply == 2) return 12;
    if (ply == 3) return 10;
    if (ply == 4) return 8;
    return 6;
}
#endif
