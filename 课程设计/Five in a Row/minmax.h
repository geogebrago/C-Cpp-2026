#ifndef CCPP2026_MINMAX_H
#define CCPP2026_MINMAX_H
#include <vector>

#include "estimate.h"
#include "move.h"
#include "way.h"
#include "wuziqi.h"
double minmax(int player, int depth, double alpha, double beta, int ply)
{
    ++nodes;
    if (timeUp)
        return evaluateBoard();
    if (depth <= 0)
        return evaluateBoard();

    vector<Move> moves = generateMoves();
    if (moves.empty())
        return evaluateBoard();

    orderMoves(moves, player);
    int width = widthAt(ply);
    if ((int)moves.size() > width)
        moves.resize(width);

    const bool isMax = (player == BLACK);
    double best = isMax ? -infScore : infScore;

    for (size_t i = 0; i < moves.size(); ++i)
    {
        const int r = moves[i].r, c = moves[i].c;

        domove(r, c, player);
        double val;
        if (win(r, c))
        {
            val = isMax ? (winScore + depth) : -(winScore + depth);
        }
        else
        {
            val = minmax(1 - player, depth - 1, alpha, beta, ply + 1);
        }
        undo(r, c);

        if (timeUp)
            return best;

        if (isMax)
        {
            if (val > best)
                best = val;
            if (best > alpha)
                alpha = best;
        }
        else
        {
            if (val < best)
                best = val;
            if (best < beta)
                beta = best;
        }

        if (best >= winScore && isMax)
            return best;
        if (best <= -winScore && !isMax)
            return best;

        if (alpha >= beta)
            break;

        if ((nodes & 511) == 0 && (clock() - startClock) * 1000LL / CLOCKS_PER_SEC >= limitMs)
        {
            timeUp = true;
            return best;
        }
    }
    return best;
}
#endif
