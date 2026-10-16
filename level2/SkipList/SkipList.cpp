#include<bits/stdc++.h>
using namespace std;
const int M_L = 32,N=100005;
mt19937 rd(time(NULL));
struct nd
{
    int key,lv;
    nd **fwd;
    int *span;
    nd(int k,int lvl)
    {
        key=k,lv=lvl;
        fwd=new nd*[lvl];
        span=new int[lvl];
        fill(fwd,fwd+lvl,nullptr);
        fill(span,span+lvl,0);
    }
    ~nd()
    {
        delete[] fwd;
        delete[] span;
    }
};
class skiplist
{
private:
    nd *hd;
    int lv;
    int rdlv()
    {
        int l=1;
        while(rd()&1&&l<M_L)
            ++l;
        return l;
    }
public:
    skiplist()
    {
        hd=new nd(0,M_L);
        lv=1;
    }
    ~skiplist()
    {
        nd *cur=hd->fwd[0];
        while(cur)
        {
            nd *nt=cur->fwd[0];
            delete cur;
            cur=nt;
        }
        delete hd;
    }
    void bd(int *s,int n)
    {
        sort(s,s+n);
        lv=max(M_L,__lg(n)+1);
        int *lv1=new int[n];
        for (int i=0;i<n;++i)
        {
            int k=1;
            while (k<lv&&i%(1<<k)==0)
                ++k;
            lv1[i]=k;
        }
        nd **nds=new nd*[n];
        for (int i=0;i<n;++i)
            nds[i]=new nd(s[i],lv1[i]);
        for (int i=0;i<lv;++i)
        {
            hd->fwd[i]=nullptr;
            hd->span[i]=0;
        }
        hd->fwd[0]=nds[0];
        for (int i=0;i<n;++i)
        {
            nds[i]->fwd[0]=(i+1<n)?nds[i+1]:nullptr;
            nds[i]->span[0]=1;
        }
        hd->span[0]=1;
        for (int i=1;i<lv;++i)
        {
            nd *pre=hd;
            int prerk=0;
            nd *cur=hd->fwd[0];
            int currk=0;
            while(cur)
            {
                ++currk;
                if (cur->lv>i)
                {
                    pre->fwd[i]=cur;
                    pre->span[i]=currk-prerk;
                    prerk=currk;
                    pre=cur;
                }
                cur=cur->fwd[0];
            }
        }
        delete[] lv1;
        delete[] nds;
    }
    void ins(int x)
    {

    }
    void del(int x)
    {

    }
    int rk(int x)
    {

    }
};
int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    skiplist sl;
    int *s=new int[n];
    for (int i=0;i<n;++i)
        scanf("%d",&s[i]);
    sl.bd(s,n);
    delete[] s;
    while (m--)
    {
        int opt,x;
        scanf("%d%d",&opt,&x);
        switch (opt)
        {
            case 1:
                {
                    sl.ins(x);
                    break;
                }
            case 2:
                {
                    sl.del(x);
                    break;
                }
            case 3:
                {
                    int res=sl.rk(x)+1;
                    break;
                }
            case 4:
                {
                    break;
                }
            case 5:
                {

                    break;
                }
            default:
                {
                    break;
                }
        }
    }
    return 0;
}