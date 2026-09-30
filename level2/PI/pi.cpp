#include <bits/stdc++.h>
#include <windows.h>
#include <psapi.h>
using namespace std;
const int B=1e9,N=10055,M=1205,C=4500;
int memory_clock()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        return pmc.PeakWorkingSetSize / 1024;
    }
    return -1;
}
struct nd
{
    int n,a[N];
    nd(int x=0)
    {
        n=0;
        memset(a,0,sizeof(a));
        while (x)
        {
            a[++n]=x%B;
            x/=B;
        }
    }
    nd operator+(nd &b)
    {
        nd c(0);
        c.n=max(n,b.n);
        for (int i=1;i<=c.n;i++)
        {
            c.a[i]+=a[i]+b.a[i];
            if (c.a[i]>=B)
            {
                c.a[i]-=B;
                c.a[i+1]++;
            }
        }
        if (c.a[c.n+1])
            c.n++;
        return c;
    }
    void operator+=(nd &b)
    {
        n=max(n,b.n);
        for (int i=1;i<=n;i++)
        {
            a[i]+=b.a[i];
            if (a[i]>=B)
            {
                a[i]-=B;
                a[i+1]++;
            }
        }
        if (a[n+1])
            n++;
    }
    void operator-=(nd &b)
    {
        for (int i=1;i<=n;i++)
        {
            a[i]-=b.a[i];
            if (a[i]<0)
            {
                a[i]+=B;
                a[i+1]--;
            }
        }
        while (n>1 && !a[n])
            n--;
    }
    nd operator-(nd &b)
    {
        nd c(0);
        c.n=n;
        for (int i=1;i<=c.n;i++)
        {
            c.a[i]+=a[i]-b.a[i];
            if (c.a[i]<0)
            {
                c.a[i]+=B;
                c.a[i+1]--;
            }
        }
        while (c.n>1 && !c.a[c.n])
            c.n--;
        return c;
    }
    nd operator*(int x)
    {
        nd c(0);
        c.n=n;
        long long r=0;
        for (int i=1;i<=c.n;i++)
        {
            r += 1LL * a[i] * x;
            c.a[i] = r % B;
            r /= B;
        }
        while (r)
        {
            c.a[++c.n] = r % B;
            r /= B;
        }
        return c;
    }
    nd operator/(int x)
    {
        nd c(0);
        c.n=n;
        long long r=0;
        for (int i=c.n;i>=1;i--)
        {
            r=1LL*r*B+a[i];
            c.a[i]=r/x;
            r%=x;
        }
        while (c.n>1 && !c.a[c.n])
            c.n--;
        return c;
    }
    void operator/=(int x)
    {
        long long r=0;
        for (int i=n;i>=1;i--)
        {
            r=1LL*r*B+a[i];
            a[i]=r/x;
            r%=x;
        }
        while (n>1 && !a[n])
            n--;
    }
    void operator*=(int x)
    {
        long long r=0;
        for (int i=1;i<=n;i++)
        {
            r += 1LL * a[i] * x;
            a[i] = r % B;
            r /= B;
        }
        while (r)
        {
            a[++n] = r % B;
            r /= B;
        }
    }
};
nd arctan(int x)
{
    nd c(0),t(0);
    t.n=M;
    t.a[M]=1;
    t/=x;
    for (int i=0;t.n&&i<C;++i)
    {
        if (i%2) c-=t;
        else c+=t;
        t*=(2*i+1);
        t/=x,t/=x;
        t/=(2*i+3);
    }
    return c;
}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    nd a=arctan(18)*48,b=arctan(57)*32,c=arctan(239)*20;
    nd pi=a+b-c;
    for (int i=pi.n;i>=1;i--)
    {
        if (i==pi.n)
        {
           string x=to_string(pi.a[i]);
            cout<<x[0]<<".";
            x.erase(x.begin());
            for (auto s:x)
                putchar(s);
        }
        else printf("%09d",pi.a[i]);
    }
    printf("\n时间: %.6lfs\n",(double)clock()/CLOCKS_PER_SEC);
    printf("内存占用: %d KB",memory_clock());
    return 0;
}