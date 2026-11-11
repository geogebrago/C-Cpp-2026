//
// Created by 26346 on 2026/11/11.
//

#ifndef CCPP2026_WAY_H
#define CCPP2026_WAY_H
#include "basic.h"
#include "estimate.h"
bool cp(const Move &a,const Move &b)
{
    return a.value>b.value;
}
vector<Move> generatemove()
{
    vector<Move>moves;
    if (alivestone==0)
    {
        moves.push_back({board_size/2,board_size/2,0});
        return moves;
    }
    for (int i=0;i<board_size;i++)
        for (int j=0;j<board_size;j++)
            if (candidate_count[i][j]>0&&board[i][j]==EMPTY)
                moves.push_back({i,j,0});
    if (moves.empty())
    {
        for (int i=0;i<board_size;i++)
            for (int j=0;j<board_size;j++)
                if (board[i][j]==EMPTY)
                    moves.push_back({i,j,0});
    }
    return moves;
}
void ordermove(vector<Move>& moves,int player)
{
    for (int i=0;i<moves.size();++i)
    {
        double v1=point_value(moves[i].r,moves[i].c,player);
        double v2=point_value(moves[i].r,moves[i].c,player==BLACK?WHITE:BLACK)*DEFENSE;
        moves[i].value=max(v1,v2);
    }
    sort(moves.begin(),moves.end(),cp);
}
int point_value(int r,int c,int player)
{
    for (int d=0;d<4;++d)
    {
        int cnt=1,huo=0;
        for (int k=1;k<=4;++k)
        {
            int nr=r+k*dr[d],nc=c+k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=player)
            {
                if (in(nr,nc)&&board[nr][nc]==EMPTY) ++huo;
                break;
            }
            cnt++;
        }
        for (int k=1;k<=4;++k)
        {
            int nr=r-k*dr[d],nc=c-k*dc[d];
            if (!in(nr,nc)||board[nr][nc]!=player||cnt==5)
            {
                if (in(nr,nc)&&board[nr][nc]==EMPTY) ++huo;
                break;
            }
            cnt++;
        }
        score[player == BLACK ? 0 : 1]+=patternscore(cnt,huo);
    }
    return score[player == BLACK ? 0 : 1];
}
int widthai(int ply)
{
    if (ply==1) return 14;
    if (ply==2) return 12;
    if (ply==3) return 10;
    if (ply==4) return 8;
    return 6;
}
#endif //CCPP2026_WAY_H
