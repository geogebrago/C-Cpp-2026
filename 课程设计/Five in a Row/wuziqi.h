#ifndef WUZIQI_H
#define WUZIQI_H

const int TIMELIMIT = 1000;
#define EMPTY -1
#define BLACK 0
#define WHITE 1
const double DEFENSE = 1.1;
const int board_size = 15;
const int INF = 1000000000;

extern int board[board_size][board_size];
extern int candidate_count[board_size][board_size];
extern int score[2];
extern int dr[4];
extern int dc[4];
extern double linecathe[4][board_size][board_size][2];
extern int viscount;
extern bool timeflag;
extern int alivestone;

bool in(int x, int y);
bool win(int r, int c);

#endif
