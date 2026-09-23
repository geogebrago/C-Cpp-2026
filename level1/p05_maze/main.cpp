#include<bits/stdc++.h>
#include <conio.h>
using namespace std;
const int N=1001;
int n,m,bx,by,ex,ey,ax,ay;
int dx[5]={0,1,-1,0,0};
int dy[5]={0,0,0,1,-1};
mt19937 rd(time(NULL));
bitset<N>vis[N];
struct nd
{
    int x,y,d;
};
vector<nd>s;
bool IN(int x,int y)
{
    return x>=1 && x<=n && y>=1 && y<=m;
}
void F_B()
{
    for (int i=1;i<=4;++i)
    {
        int tx=bx+dx[i],ty=by+dy[i];
        int nx=bx+2*dx[i],ny=by+2*dy[i];
        if (IN(nx,ny)&&vis[nx][ny]==0)
        {
            s.push_back({tx,ty,i});
        }
    }
}
void O_P()
{
    for (int i=1;i<=n;++i)
    {
        for (int j=1;j<=m;++j)
        {
            if (i==ex&&j==ey)
                putchar('$');
            else if (i==ax&&j==ay)
                putchar('@');
            else if (vis[i][j]==0)
                putchar('#');
            else
                putchar(' ');
        }
        puts("");
    }
}
void GT()
{
    char ch = _getch();
    system("cls");
    if (ch == 'w' || ch == 'W')
        ax--;
    else if (ch == 's' || ch == 'S')
        ax++;
    else if (ch == 'a' || ch == 'A')
        ay--;
    else if (ch == 'd' || ch == 'D')
        ay++;
    if (vis[ax][ay]==0)
    {
        if (ch == 'w' || ch == 'W')
            ax++;
        else if (ch == 's' || ch == 'S')
            ax--;
        else if (ch == 'a' || ch == 'A')
            ay++;
        else if (ch == 'd' || ch == 'D')
            ay--;
    }
    if (ax==ex&&ay==ey)
    {
        system("cls");
        puts("END");
        exit(0);
    }
}
int main()
{
    scanf("%d %d",&n,&m);
    if (n<=3||m<=3) return 0;
    ax=bx=rd()%n+1;
    ay=by=rd()%m+1;
    vis[bx][by]=1;
    F_B();
    while (!s.empty())
    {
        int sz=s.size(),num=rd()%sz;
        int x=s[num].x,y=s[num].y,d=s[num].d;
        int nx=x+dx[d],ny=y+dy[d];
        if (IN(nx,ny)&&vis[nx][ny]==0)
        {
            vis[x][y]=1,vis[nx][ny]=1;
            bx=nx,by=ny;
            F_B();
        }
        swap(s[num],s.back());
        s.pop_back();
    }
    int md=0;
    for (int i=1;i<=n;++i)
        for (int j=1;j<=m;++j)
            if (vis[i][j]==1&&abs(i-ax)+abs(j-ay)>md) md=abs(i-ax)+abs(j-ay),ex=i,ey=j;
    while (1)
    {
        O_P();
        GT();
    }
    return 0;
}//@ 人 #强 $终点