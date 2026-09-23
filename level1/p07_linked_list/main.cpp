 #include<bits/stdc++.h>
using namespace std;
struct nd
{
    int v;
    nd *nt;
    nd(int w)
    {
        v=w;
        nt=nullptr;
    }
};
nd *crt()
{
    int a[]={1,1,4,5,1,4};
    nd *hd=nullptr;
    nd *tl=nullptr;
    for (int w:a)
    {
        nd *p=new nd(w);
        if (hd==nullptr)
        {
            hd=tl=p;
        }
        else
        {
            tl->nt=p;
            tl=p;
        }
    }
    return hd;
}
void prt(nd *hd)
{
    nd *p=hd;
    while (p!=nullptr)
    {
        printf("%d ",p->v);
        p=p->nt;
    }
    puts("");
}
nd *rsv(nd *hd)
{
    nd *pre=nullptr;
    nd *cur=hd;
    while (cur!=nullptr)
    {
        nd *nt=cur->nt;
        cur->nt=pre;
        pre=cur;
        cur=nt;
    }
    return pre;
}
void del(nd *&hd)
{
    while (hd!=nullptr)
    {
        nd *tp=hd;
        hd=hd->nt;
        delete tp;
    }
}
nd *fd(nd *hd,int w)
{
    nd *p=hd;
    while (p!=nullptr)
    {
        if (p->v==5) return p;
        p=p->nt;
    }
    return nullptr;
}
int main()
{
    nd *hd=crt();
    prt(hd);
    hd=rsv(hd);
    prt(hd);
    nd *fi=fd(hd,5);
    nd *se=fd(fi,5);
    del(hd);
     return 0;
}