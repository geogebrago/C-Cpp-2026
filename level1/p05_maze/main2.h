#ifndef CCPP2026_MAIN2_H
#define CCPP2026_MAIN2_H

#include <stdbool.h>

#define MAX_SIZE 1001

extern int rows, cols;
extern int end_x, end_y;
extern int player_x, player_y;
extern bool visited[MAX_SIZE][MAX_SIZE];

void print_board(void);

#endif
