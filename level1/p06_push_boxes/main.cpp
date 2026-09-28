#include<bits/stdc++.h>
#include<Windows.h>
#include<conio.h>
using namespace std;

const int N=105;
int x,y,tot,nb,nc,ax,ay,now,lev;
int n,m,bx[N],by[N],cx[N],cy[N],sc[4];
int dx[5]={0,1,-1,0,0},dy[5]={0,0,0,1,-1};
char s[N][N];

bool I_N(int x,int y)
{
    return x>=1&&x<=n&&y>=1&&y<=m;
}

int F_B(int x,int y)
{
    for(int i=1;i<=nb;++i)
    {
        if(bx[i]==x&&by[i]==y)
            return i;
    }
    return 0;
}

bool F_C(int x,int y)
{
    for(int i=1;i<=nc;++i)
    {
        if(cx[i]==x&&cy[i]==y)
            return true;
    }
    return false;
}

bool CK()
{
    int a=0,b=0,c=0;

    for(int i=1;i<=n;++i)
    {
        for(int j=1;j<=m;++j)
        {
            if(s[i][j]=='@')
                ++a;
            else if(s[i][j]=='.')
                ++b;
            else if(s[i][j]=='$')
                ++c;
            else if(s[i][j]!='#'&&s[i][j]!=' ')
                return false;
        }
    }

    if(a!=1||b!=c||b==0)
        return false;

    return true;
}

bool LD(const char *file)
{
    FILE *fp=fopen(file,"r");

    if(fp==NULL)
    {
        cout<<"文件打开失败\n";
        return false;
    }

    memset(s,' ',sizeof(s));

    n=0;
    m=0;

    char r[N];

    while(fgets(r,N,fp)!=NULL)
    {
        int len=strlen(r);

        while(len>0&&(r[len-1]=='\n'||r[len-1]=='\r'))
            r[--len]='\0';

        /*if(n==0&&len>=3&&
           (unsigned char)r[0]==0xEF&&
           (unsigned char)r[1]==0xBB&&
           (unsigned char)r[2]==0xBF)
        {
            memmove(r,r+3,len-2);
            len-=3;
        }*/

        if(len==0)
            continue;

        ++n;

        if(n>=N||len>=N)
        {
            fclose(fp);
            cout<<"地图太大\n";
            return false;
        }

        m=max(m,len);

        for(int j=1;j<=len;++j)
            s[n][j]=r[j-1];
    }

    fclose(fp);

    if(n==0||m==0)
    {
        cout<<"地图为空\n";
        return false;
    }

    if(!CK())
    {
        cout<<"地图格式错误\n";
        return false;
    }

    return true;
}

void sol()
{
    nb=0;
    nc=0;
    now=0;
    ax=ay=0;

    for(int i=1;i<=n;++i)
    {
        for(int j=1;j<=m;++j)
        {
            if(s[i][j]=='@')
            {
                ax=i;
                ay=j;
                s[i][j]=' ';
            }
            else if(s[i][j]=='.')
            {
                bx[++nb]=i;
                by[nb]=j;
                s[i][j]=' ';
            }
            else if(s[i][j]=='$')
            {
                cx[++nc]=i;
                cy[nc]=j;
            }
        }
    }
}

void pm()
{
    system("cls");

    cout<<"==============================\n";
    cout<<"         推箱子小游戏\n";
    cout<<"==============================\n";
    cout<<"关卡: "<<lev<<"    步数: "<<now<<"\n\n";

    for(int i=1;i<=n;++i)
    {
        for(int j=1;j<=m;++j)
        {
            if(i==ax&&j==ay)
                putchar('@');
            else if(F_B(i,j))
                putchar('.');
            else
                putchar(s[i][j]);
        }
        puts("");
    }

    cout<<"\nWASD移动  R重开  Q返回\n";
}

bool WIN()
{
    for(int i=1;i<=nb;++i)
    {
        if(!F_C(bx[i],by[i]))
            return false;
    }

    return true;
}

int GT()
{
    char ch=_getch();

    if(ch=='q'||ch=='Q')
        return 0;

    if(ch=='r'||ch=='R')
        return 2;

    int d=-1;

    if(ch=='w'||ch=='W')
        d=2;
    else if(ch=='s'||ch=='S')
        d=1;
    else if(ch=='a'||ch=='A')
        d=4;
    else if(ch=='d'||ch=='D')
        d=3;

    if(d==-1)
        return 1;

    int nx=ax+dx[d];
    int ny=ay+dy[d];

    if(!I_N(nx,ny)||s[nx][ny]=='#')
        return 1;

    int k=F_B(nx,ny);

    if(k)
    {
        int nnx=nx+dx[d];
        int nny=ny+dy[d];

        if(!I_N(nnx,nny)||s[nnx][nny]=='#'||F_B(nnx,nny))
            return 1;

        bx[k]=nnx;
        by[k]=nny;
    }

    ax=nx;
    ay=ny;
    ++now;

    return 1;
}

void SV()
{
    if(sc[lev]==0||now<sc[lev])
        sc[lev]=now;

    FILE *fp=fopen(
        "C:\\Users\\26346\\CLionProjects\\C-Cpp-2026\\level1\\p06_push_boxes\\score.txt",
        "w"
    );

    if(fp==NULL)
        return;

    for(int i=1;i<=3;++i)
        fprintf(fp,"%d\n",sc[i]);

    fclose(fp);
}

void GL()
{
    char file[N];

    if(lev==1)
    {
        strcpy(file,
            "C:\\Users\\26346\\CLionProjects\\C-Cpp-2026\\level1\\p06_push_boxes\\yi.txt");
    }
    else if(lev==2)
    {
        strcpy(file,
            "C:\\Users\\26346\\CLionProjects\\C-Cpp-2026\\level1\\p06_push_boxes\\er.txt");
    }
    else
    {
        strcpy(file,
            "C:\\Users\\26346\\CLionProjects\\C-Cpp-2026\\level1\\p06_push_boxes\\san.txt");
    }

    if(!LD(file))
    {
        _getch();
        return;
    }

    sol();
}

void G1()
{
    while(1)
    {
        pm();

        if(WIN())
        {
            cout<<"\n恭喜通关！\n";
            cout<<"本关用了 "<<now<<" 步\n";

            SV();

            cout<<"按任意键返回\n";
            _getch();
            return;
        }

        int k=GT();

        if(k==0)
            return;

        if(k==2)
            GL();
    }
}

void LS()
{
    FILE *fp=fopen(
        "C:\\Users\\26346\\CLionProjects\\C-Cpp-2026\\level1\\p06_push_boxes\\score.txt",
        "r"
    );

    if(fp==NULL)
    {
        for(int i=1;i<=3;++i)
            sc[i]=0;

        return;
    }

    for(int i=1;i<=3;++i)
    {
        if(fscanf(fp,"%d",&sc[i])!=1)
            sc[i]=0;
    }

    fclose(fp);
}

void prt()
{
    cout<<"==============================\n";
    cout<<"         推箱子小游戏\n";
    cout<<"==============================\n";
    cout<<"1.关卡\n";
    cout<<"2.成绩\n";
    cout<<"3.退出\n";

    cin>>x;

    switch(x)
    {
    case 1:
        {
            cout<<"请输入关卡1~3\n";
            cin>>lev;

            if(lev<1||lev>3)
            {
                cout<<"?\n";
                break;
            }

            GL();
            G1();
            break;
        }

    case 2:
        {
            cout<<"==============================\n";
            cout<<"           成绩\n";
            cout<<"==============================\n";

            for(int i=1;i<=3;++i)
            {
                if(sc[i])
                    cout<<"关卡 "<<i<<" : "<<sc[i]<<" 步\n";
                else
                    cout<<"关卡 "<<i<<" : 暂无成绩\n";
            }

            cout<<"\n按任意键返回\n";
            _getch();
            break;
        }

    case 3:
        {
            cout<<"已退出\n";
            exit(0);
        }

    default:
        {
            cout<<"?\n";
            break;
        }
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    LS();

    while(1)
        prt();

    return 0;
}