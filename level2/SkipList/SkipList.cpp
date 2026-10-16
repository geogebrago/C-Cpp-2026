#include <bits/stdc++.h>
using namespace std;

const int M_L = 32, N = 100005;
mt19937 rd(time(NULL));

struct nd
{
    int key, lv;
    nd **fwd;
    int *span;
    nd *in_fwd[4];
    int in_sp[4];

    nd(int k, int lvl)
    {
        key = k, lv = lvl;
        if (lvl <= 4)
        {
            fwd = in_fwd;
            span = in_sp;
            for (int i = 0; i < lvl; ++i)
            {
                fwd[i] = nullptr;
                span[i] = 0;
            }
        }
        else
        {
            fwd = new nd *[lvl];
            span = new int[lvl];
            fill(fwd, fwd + lvl, nullptr);
            fill(span, span + lvl, 0);
        }
    }

    ~nd()
    {
        if (lv > 4)
        {
            delete[] fwd;
            delete[] span;
        }
    }
};

class skiplist
{
private:
    nd *hd;
    int lv;

    int rdlv()
    {
        unsigned int r = rd();
        int zeros = __builtin_ctz(~r);
        int l = zeros + 1;
        return l;
    }


public:
    skiplist()
    {
        hd = new nd(0, M_L);
        lv = 1;
    }

    ~skiplist()
    {
        nd *cur = hd->fwd[0];
        while (cur)
        {
            nd *nt = cur->fwd[0];
            delete cur;
            cur = nt;
        }
        delete hd;
    }

    void bd(int *s, int n)
    {
        if (n == 0)
        {
            lv = 1;
            fill(hd->fwd,hd->fwd+M_L,nullptr);
            fill(hd->span,hd->span+M_L,0);
            return;
        }

        sort(s, s + n);
        lv = min(M_L, __lg(n) + 1);

        int *lv1 = new int[n];
        lv1[0] = lv;
        for (int i = 1; i < n; ++i)
        {
            int k = __builtin_ctz((unsigned int)i) + 1;
            lv1[i] = k < lv ? k : lv;
        }

        nd **nds = new nd *[n];
        for (int i = 0; i < n; ++i)
            nds[i] = new nd(s[i], lv1[i]);

        for (int i = 0; i < M_L; ++i)
        {
            hd->fwd[i] = nullptr;
            hd->span[i] = 0;
        }

        hd->fwd[0] = nds[0];
        for (int i = 0; i < n; ++i)
        {
            nds[i]->fwd[0] = (i + 1 < n) ? nds[i + 1] : nullptr;
            nds[i]->span[0] = 1;
        }
        hd->span[0] = 1;

        for (int i = 1; i < lv; ++i)
        {
            nd *pre = hd;
            int prerk = 0;
            nd *cur = hd->fwd[0];
            int currk = 0;
            while (cur)
            {
                ++currk;
                if (cur->lv > i)
                {
                    pre->fwd[i] = cur;
                    pre->span[i] = currk - prerk;
                    prerk = currk;
                    pre = cur;
                }
                cur = cur->fwd[0];
            }
        }

        delete[] lv1;
        delete[] nds;
    }

    void ins(int x)
    {
        nd *upt[M_L];
        int rk[M_L];
        nd *p = hd;

        for (int i = lv - 1; i >= 0; --i)
        {
            rk[i] = (i == lv - 1) ? 0 : rk[i + 1];
            while (p->fwd[i] && p->fwd[i]->key < x)
            {
                rk[i] += p->span[i];
                p = p->fwd[i];
            }
            upt[i] = p;
        }

        int nlv = rdlv();
        if (nlv > lv)
        {
            for (int i = lv; i < nlv; ++i)
            {
                upt[i] = hd;
                rk[i] = 0;
                hd->span[i] = 0;
                hd->fwd[i] = nullptr;
            }
            lv = nlv;
        }

        nd *new_node = new nd(x, nlv);
        for (int i = 0; i < nlv; ++i)
        {
            new_node->fwd[i] = upt[i]->fwd[i];
            if (upt[i]->fwd[i])
                new_node->span[i] = upt[i]->span[i] - (rk[0] - rk[i]);
            else
                new_node->span[i] = 0;

            upt[i]->fwd[i] = new_node;
            upt[i]->span[i] = rk[0] - rk[i] + 1;
        }

        for (int i = nlv; i < lv; ++i)
            ++upt[i]->span[i];
    }

    void del(int x)
    {
        nd *upt[M_L];
        nd *p = hd;

        for (int i = lv - 1; i >= 0; --i)
        {
            while (p->fwd[i] && p->fwd[i]->key < x)
                p = p->fwd[i];
            upt[i] = p;
        }

        p = p->fwd[0];
        if (p && p->key == x)
        {
            for (int i = 0; i < lv; ++i)
            {
                if (upt[i]->fwd[i] == p)
                {
                    upt[i]->span[i] += p->span[i] - 1;
                    upt[i]->fwd[i] = p->fwd[i];
                }
                else
                {
                    --upt[i]->span[i];
                }
            }
            delete p;

            while (lv > 1 && hd->fwd[lv - 1] == nullptr)
                --lv;
        }
    }

    int rk(int x)
    {
        nd *p = hd;
        int r = 0;
        for (int i = lv - 1; i >= 0; --i)
        {
            while (p->fwd[i] && p->fwd[i]->key < x)
            {
                r += p->span[i];
                p = p->fwd[i];
            }
        }
        return r + 1;
    }

    int kth(int x)
    {
        nd *p = hd;
        int r = 0;
        for (int i = lv - 1; i >= 0; --i)
        {
            while (p->fwd[i] && r + p->span[i] <= x)
            {
                r += p->span[i];
                p = p->fwd[i];
            }
        }
        return p->key;
    }

    int pre(int x)
    {
        nd *p = hd;
        for (int i = lv - 1; i >= 0; --i)
        {
            while (p->fwd[i] && p->fwd[i]->key < x)
                p = p->fwd[i];
        }
        if (p == hd)
            return -1;
        return p->key;
    }

    int nt(int x)
    {
        nd *p = hd;
        for (int i = lv - 1; i >= 0; --i)
        {
            while (p->fwd[i] && p->fwd[i]->key <= x)
                p = p->fwd[i];
        }
        if (p->fwd[0] == nullptr)
            return -1;
        return p->fwd[0]->key;
    }
};

int main()
{
    int n, m;
     scanf("%d", &n);
    scanf("%d", &m);
    skiplist sl;
     int *s = new int[n];
    for (int i = 0; i < n; ++i)
        scanf("%d", &s[i]);
    sl.bd(s, n);
    delete[] s;

    while (m--)
    {
        int opt, x;
        scanf("%d%d", &opt, &x);
        switch (opt)
        {
        case 1:
            sl.ins(x);
            break;
        case 2:
            sl.del(x);
            break;
        case 3:
            printf("%d\n", sl.rk(x));
            break;
        case 4:
            printf("%d\n", sl.kth(x));
            break;
        case 5:
            printf("%d\n", sl.pre(x));
            break;
        case 6:
            printf("%d\n", sl.nt(x));
            break;
        }
    }
    return 0;
}