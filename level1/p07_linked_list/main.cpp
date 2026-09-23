#include <bits/stdc++.h>
using namespace std;

struct nd
{
    int v;
    nd *nt;

    nd(int w)
    {
        v = w;
        nt = nullptr;
    }
};

nd *crt()
{
    int a[] = {1, 1, 5, 5, 1, 4};

    nd *hd = nullptr;
    nd *tl = nullptr;

    for (int w : a)
    {
        nd *p = new nd(w);

        if (hd == nullptr)
        {
            hd = tl = p;
        }
        else
        {
            tl->nt = p;
            tl = p;
        }
    }

    return hd;
}

void prt(nd *hd)
{
    nd *p = hd;

    while (p != nullptr)
    {
        printf("%d ", p->v);
        p = p->nt;
    }

    puts("");
}

nd *rsv(nd *hd)
{
    nd *pre = nullptr;
    nd *cur = hd;

    while (cur != nullptr)
    {
        nd *nt = cur->nt;

        cur->nt = pre;
        pre = cur;
        cur = nt;
    }

    return pre;
}

void del(nd *&hd)
{
    while (hd != nullptr)
    {
        nd *tp = hd;
        hd = hd->nt;
        delete tp;
    }
}

nd *fd(nd *hd, int w, int &id)
{

    while (hd != nullptr)
    {
        if (hd->v == w)
            return hd;

        ++id;
        hd = hd->nt;
    }

    id = -1;
    return nullptr;
}

int main()
{
    nd *hd = crt();

    prt(hd);

    hd = rsv(hd);

    prt(hd);

    int id1=1;
    nd *fi = fd(hd, 5, id1);

    if (fi == nullptr)
        puts("-1");
    else
        printf("%d\n", id1);
    int id2=id1+1;
    nd *se = nullptr;

    // 只有 fi 不为空时，才能访问 fi->nt
    if (fi != nullptr)
        se = fd(fi->nt, 5, id2);

    if (se == nullptr)
        puts("-1");
    else
        printf("%d\n", id2);

    del(hd);

    return 0;
}