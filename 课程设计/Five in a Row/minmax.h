//
// Created by 26346 on 2026/11/11.
//

#ifndef CCPP2026_MINMAX_H
#define CCPP2026_MINMAX_H
#include <vector>

#include "estimate.h"
#include "move.h"
#include "wuziqi.h"
const int infscore=2e9;
struct Move
{
    int r,c;
    int score;
};
int minmax(int player,int depth,int aplha,int beta,int ply)//???
{
    ++viscount;
    if (timeflag||depth<=0) return evaluateBoard();
    vector<Move> moves=generateMoves();
    if (moves.empty()) return evaluateBoard();
    orderMoves(moves,player);
    int wid=widthai(ply);
    if (moves.size()>wid) moves.resize(wid);
    bool pd=player==BLACK?1:0;
    for (int i=0;i<moves.size();++i)
    {
        int r=moves[i].r,c=moves[i].c;
        domove(r,c,player);
        if (win(r,c))
        {
            undo(r,c);
            return player==BLACK?infscore:-infscore;
        }
        int v=minmax(1-player,depth-1,-beta,-aplha,ply+1);
        v=-v;
        undo(r,c);
        if (v>=beta) return beta;
        if (v>aplha) aplha=v;
    }
    return 0;
}
#endif //CCPP2026_MINMAX_H
