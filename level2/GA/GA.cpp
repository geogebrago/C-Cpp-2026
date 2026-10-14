/*#include <bits/stdc++.h>
#include <Windows.h>
using namespace std;

const int P=2000, L=100, MAXG=1000, R=20, C=20;
const int GX=18, GY=19;                // G 的真实坐标
mt19937 rd(time(0));

char mp[22][22]=
{
    "S...#...#.......#...",
    ".#.#.#.#.#.#.#.#.#..",
    "..#...#...#...#.....",
    "#.#.#.#.#.#.#.#.#.#.",
    "....#...#...#...#...",            // ← 打开 (4,1)，让 S 能走出去
    ".#.#.#.#.#.#.#.#.#..",
    "..#...#...#...#.....",
    "#.#.#.#.#.#.#.#.#.#.",
    ".#..#...#...#...#...",
    ".#.#.#.#.#.#.#.#.#..",
    "..#...#...#...#.....",
    "#.#.#.#.#.#.#.#.#.#.",
    ".#..#...#...#...#...",
    ".#.#.#.#.#.#.#.#.#..",
    "..#...#...#...#.....",
    "#.#.#.#.#.#.#.#.#.#.",
    ".#..#...#...#...#...",
    ".#.#.#.#.#.#.#.#.#..",
    "..#...#...#...#....G",
    "#..................."
};

int dx[4]={-1,1,0,0}, dy[4]={0,0,-1,1};
int bd[22][22];                        // 每个可走格到 G 的 BFS 距离

void bfs_from_G(){
    memset(bd,-1,sizeof(bd));
    queue<pair<int,int>> Q;
    Q.push({GX,GY}); bd[GX][GY]=0;
    while(!Q.empty()){
        auto t=Q.front(); Q.pop();
        int x=t.first, y=t.second;
        for(int k=0;k<4;k++){
            int nx=x+dx[k], ny=y+dy[k];
            if(nx<0||nx>=R||ny<0||ny>=C||mp[nx][ny]=='#'||bd[nx][ny]>=0) continue;
            bd[nx][ny]=bd[x][y]+1; Q.push({nx,ny});
        }
    }
}

bool connectable(){                    // 检查 S 是否能到 G
    bool v[22][22]={0};
    queue<pair<int,int>> Q; Q.push({0,0}); v[0][0]=1;
    while(!Q.empty()){
        auto t=Q.front(); Q.pop();
        for(int k=0;k<4;k++){
            int nx=t.first+dx[k], ny=t.second+dy[k];
            if(nx<0||nx>=R||ny<0||ny>=C||mp[nx][ny]=='#'||v[nx][ny]) continue;
            v[nx][ny]=1; Q.push({nx,ny});
        }
    }
    return v[GX][GY];
}

struct Node{int s[L], x, y, fit;};
Node p[P], q[P];

int calc(Node &a){
    a.x=0; a.y=0;
    int fit=0;
    for(int i=0;i<L;i++){
        int nx=a.x+dx[a.s[i]], ny=a.y+dy[a.s[i]];
        if(nx<0||nx>=R||ny<0||ny>=C||mp[nx][ny]=='#'){ fit-=5; continue; }
        a.x=nx; a.y=ny;
        if(mp[a.x][a.y]=='G') return 100000+L-i;
        int d = bd[a.x][a.y]; if(d<0) d=500;       // 到 G 的 BFS 距离
        fit += 20 - 3*d;
    }
    int d = bd[a.x][a.y]; if(d<0) d=500;
    fit -= 20*d;
    return fit;
}

Node &sel(){
    Node &x=p[rd()%P], &y=p[rd()%P], &z=p[rd()%P];
    if(x.fit<y.fit) swap(x,y);
    if(x.fit<z.fit) swap(x,z);
    return x;
}

void print(Node &a){
    int x=0,y=0; cout<<"路径：";
    for(int i=0;i<L;i++){
        int nx=x+dx[a.s[i]], ny=y+dy[a.s[i]];
        if(nx<0||nx>=R||ny<0||ny>=C||mp[nx][ny]=='#') continue;
        x=nx; y=ny;
        char c[4]={'U','D','L','R'}; cout<<c[a.s[i]];
        if(mp[x][y]=='G') break;
    }
    cout<<"\n";
}

int main(){
    SetConsoleOutputCP(CP_UTF8); SetConsoleCP(CP_UTF8);

    if(!connectable()){ cout<<"G 不可达！地图有 bug\n"; return 0; }
    bfs_from_G();

    // 初始化：随机游走 + 偏向 BFS 距离小的邻居，避免一开始就撞墙
    for(int i=0;i<P;i++)
    {
        int cx=0, cy=0;
        for(int j=0;j<L;j++){
            int v[4], n=0;
            for(int k=0;k<4;k++){
                int nx=cx+dx[k], ny=cy+dy[k];
                if(nx>=0&&nx<R&&ny>=0&&ny<C&&mp[nx][ny]!='#') v[n++]=k;
            }
            if(!n){ p[i].s[j]=rd()%4; continue; }
            // 70% 概率选 BFS 距离更小的方向，30% 随机
            int best=v[0];
            for(int t=1;t<n;t++){
                int ax=cx+dx[v[t]], ay=cy+dy[v[t]];
                int bx=cx+dx[best], by=cy+dy[best];
                if(bd[ax][ay]>=0 && (bd[bx][by]<0 || bd[ax][ay]<bd[bx][by])) best=v[t];
            }
            p[i].s[j] = (rd()%100<70) ? best : v[rd()%n];
            cx += dx[p[i].s[j]];
            cy += dy[p[i].s[j]];
        }
    }

    for(int g=0;g<MAXG;g++){
        for(int i=0;i<P;i++) p[i].fit=calc(p[i]);

        int id=0;
        for(int i=1;i<P;i++) if(p[i].fit>p[id].fit) id=i;
        if(p[id].fit>=100000){ cout<<"第 "<<g<<" 代找到路径\n"; print(p[id]); return 0; }

        q[0]=p[id];
        int mr = max(1, 8 - g*7/MAXG);   // 自适应变异率 8%→1%

        for(int i=1;i<P;i+=2){
            Node &x=sel(), &y=sel();
            q[i]=x; if(i+1<P) q[i+1]=y;
            int k=rd()%L;
            for(int j=k;j<L;j++) if(i+1<P) swap(q[i].s[j],q[i+1].s[j]);
            for(int j=0;j<L;j++){
                if((int)(rd()%100)<mr) q[i].s[j]=rd()%4;
                if(i+1<P&&(int)(rd()%100)<mr) q[i+1].s[j]=rd()%4;
            }
        }
        swap(p,q);
    }
    cout<<"没有找到路径\n";
    return 0;
}*/