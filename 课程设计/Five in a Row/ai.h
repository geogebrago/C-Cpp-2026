#ifndef CCPP2026_AI_H
#define CCPP2026_AI_H
#include<bits/stdc++.h>
using namespace std;
struct AIResult
{
    int r, c;
    int depth;
    long long nodes;
    double score;
    double seconds;
};
AIResult aiMove(int aiColor)
{
    AIResult res;
    res.r = res.c = -1;
    res.depth = 0;
    res.nodes = 0;
    res.score = 0.0;
    res.seconds = 0.0;
    startClock = clock();
    nodes = 0;
    timeUp = false;
    vector<Move> moves = generateMoves();
    if (moves.empty())
        return res;
    if (moves.size() == 1)
    {
        res.r = moves[0].r;
        res.c = moves[0].c;
        res.seconds = (clock() - startClock) / (double)CLOCKS_PER_SEC;
        return res;
    }
    orderMoves(moves, aiColor);
    if (moves.size() > maxMoves)
        moves.resize(maxMoves);
    const bool aiIsBlack = (aiColor == BLACK);
    int bestR = moves[0].r, bestC = moves[0].c;
    double bestKey = 0.0;
    auto rootKey = [&](int r, int c, int depth, double alpha, double beta) -> double
    {
        domove(r, c, aiColor);
        double key;
        if (win(r, c))
        {
            key = winScore + depth;
        }
        else
        {
            double a2 = aiIsBlack ? alpha : -beta;
            double b2 = aiIsBlack ? beta : -alpha;
            double v = minmax(1 - aiColor, depth - 1, a2, b2, 1);
            key = aiIsBlack ? v : -v;
        }
        undo(r, c);
        return key;
    };

    for (int depth = 1; depth <= maxDepth; ++depth)
    {
        for (size_t i = 0; i < moves.size(); ++i)
            if (moves[i].r == bestR && moves[i].c == bestC)
            {
                swap(moves[0], moves[i]);
                break;
            }

        double localBest = -infScore;
        int localR = bestR, localC = bestC;
        bool haveFirst = false;
        bool aborted = false;

        for (size_t i = 0; i < moves.size(); ++i)
        {
            if (depth > 1 && (clock() - startClock) * 1000LL / CLOCKS_PER_SEC >= limitMs)
            {
                timeUp = true;
                aborted = true;
                break;
            }

            const int r = moves[i].r, c = moves[i].c;
            double key;

            if (!haveFirst)
            {
                key = rootKey(r, c, depth, -infScore, infScore);
                if (timeUp)
                {
                    aborted = true;
                    break;
                }
                haveFirst = true;
                localBest = key;
                localR = r;
                localC = c;
                continue;
            }

            if (depth == 1)
            {
                key = rootKey(r, c, depth, -infScore, infScore);
            }
            else
            {
                key = rootKey(r, c, depth, localBest, localBest + 1e-6);
                if (!timeUp && key > localBest)
                    key = rootKey(r, c, depth, localBest, infScore);
            }

            if (timeUp)
            {
                aborted = true;
                break;
            }

            if (key > localBest)
            {
                localBest = key;
                localR = r;
                localC = c;
            }
        }

        if (!aborted && haveFirst)
        {
            bestR = localR;
            bestC = localC;
            bestKey = localBest;
            res.depth = depth;
        }

        if (bestKey >= winScore)
            break;
        if (bestKey <= -winScore)
            break;
        if (aborted || (clock() - startClock) * 1000LL / CLOCKS_PER_SEC >= limitMs)
            break;
    }

    res.r = bestR;
    res.c = bestC;
    res.nodes = nodes;
    res.score = bestKey;
    res.seconds = (clock() - startClock) / (double)CLOCKS_PER_SEC;
    return res;
}
#endif
