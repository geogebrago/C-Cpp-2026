#ifndef WUZIQI_H
#define WUZIQI_H
#include <chrono>
#include <vector>

const int timeLimit = 1000;
#define EMPTY -1
#define BLACK 0
#define WHITE 1
const double defense = 1.1;
const int boardSize = 15;
const double infScore = 1e12;
const double winScore = 1e10;
const int maxDepth = 5;
const int maxMoves = 16;

extern int board[boardSize][boardSize];
extern int nearCount[boardSize][boardSize];
extern double score[2];
extern int dr[4];
extern int dc[4];
extern double lineScore[4][boardSize][boardSize][2];
extern int alivestone;

struct Move { int r, c; double score; };

inline long long nodes = 0;
inline bool timeUp = false;
inline std::chrono::steady_clock::time_point startTime;
inline int limitMs = timeLimit;

inline long long elapsed()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startTime).count();
}

bool in(int x, int y);
bool win(int r, int c);

#endif
