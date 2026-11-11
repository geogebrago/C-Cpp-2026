//
// Created by 26346 on 2026/11/11.
//

#ifndef CCPP2026_AI_H
#define CCPP2026_AI_H
#include<bits/stdc++.h>
using namespace std;//?????
struct AIResult
{
    int r, c; // AI 选择的落子位置
    int depth; // 搜索深度
    long long nodes; // 搜索节点数
    double score; // AI 视角分数
    double seconds; // 耗时（秒）
};
AIResult aiMove(int aiColor)
{
    AIResult res;
    res.r = res.c = -1;
    res.depth = 0;
    res.nodes = 0;
    res.score = 0.0;
    res.seconds = 0.0;
    clock_t getstart=clock();
    nodeCount = 0;
    timeUpFlag = false;
    vector<Move> moves = generateMoves();
    if (moves.empty())
        return res; // 没有可下的点
    if (moves.size() == 1)
    { // 空盘：天元；或只剩一个空点
        res.r = moves[0].r;
        res.c = moves[0].c;
        res.seconds = (clock() - getstart) / 1000.0;
        return res;
    }
    orderMoves(moves, aiColor);
    if (moves.size() > rootWidth)
        moves.resize(rootWidth);
    const bool aiIsBlack = (aiColor == BLACK);
    int bestR = moves[0].r, bestC = moves[0].c;
    double bestKey = infscore;
    auto rootKey = [&](int r, int c, int depth, double alpha, double beta) -> double
    {
        doMove(r, c, aiColor);
        double key;
        if (checkWin(r, c))
        {
            key = WIN_SCORE + depth; // AI 赢棋，AI 视角恒为正
        }
        else
        {
            // 把 AI 视角窗口换成黑方视角窗口交给 minmax
            double a2 = aiIsBlack ? alpha : -beta;
            double b2 = aiIsBlack ? beta : -alpha;
            double v = minmax(1 - aiColor, depth - 1, a2, b2, 1);
            key = aiIsBlack ? v : -v;
        }
        undo(r, c);
        return key;
    };

    for (int depth = 1; depth <= MAX_DEPTH; ++depth)
    {
        // 上一层的最佳着法放到最前面，PVS 的第一个着法最可能一路 fail-high
        for (size_t i = 0; i < moves.size(); ++i)
            if (moves[i].r == bestR && moves[i].c == bestC)
            {
                swap(moves[0], moves[i]);
                break;
            }

        double localBest = -INF_SCORE;
        int localR = bestR, localC = bestC;
        bool haveFirst = false;
        bool aborted = false;

        for (size_t i = 0; i < moves.size(); ++i)
        {
            if (depth > 1 && elapsedMs() >= timeLimitMs)
            {
                timeUpFlag = true;
                aborted = true;
                break;
            }

            const int r = moves[i].r, c = moves[i].c;
            double key;

            if (!haveFirst)
            {
                // 第一个着法：完整窗口，拿到本层的基准分
                key = rootKey(r, c, depth, -INF_SCORE, INF_SCORE);
                if (timeUpFlag)
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
                // 第 1 层只有一次估值，PVS 省不了什么，直接全窗口
                key = rootKey(r, c, depth, -INF_SCORE, INF_SCORE);
            }
            else
            {
                // 零窗口侦察：只问"能不能比现在最好的一手更强"
                key = rootKey(r, c, depth, localBest, localBest + 0.5);
                if (!timeUpFlag && key > localBest)
                    key = rootKey(r, c, depth, localBest, INF_SCORE); // 真更好：全窗口复核
            }

            if (timeUpFlag)
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
        { // 完整搜完的一层才算数
            bestR = localR;
            bestC = localC;
            bestKey = localBest;
            res.depth = depth;
        }

        if (bestKey >= WIN_SCORE)
            break; // 已经能赢
        if (bestKey <= -WIN_SCORE)
            break; // 已经必输
        if (aborted || elapsedMs() >= timeLimitMs)
            break;
    }

    res.r = bestR;
    res.c = bestC;
    res.nodes = nodeCount;
    res.score = aiIsBlack ? bestKey : -bestKey; // 换回黑方视角
    res.seconds = elapsedMs() / 1000.0;
    return res;
}
#endif //CCPP2026_AI_H
