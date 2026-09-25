#include<bits/stdc++.h>
using namespace std;

template<typename T>
class dnager_array
{
private:
    T *a;
    int n;

public:
    dnager_array(int sz)
    {
        if (sz<=0)
        {
            n=0;
            a=nullptr;
            return;
        }

        n=sz;
        a=new T[n];
    }

    ~dnager_array()
    {
        delete[] a;
    }

    T &operator[](int i)
    {
        if (i<0||i>=n)
        {
            printf("下标越界\n");
            exit(1);
        }

        return a[i];
    }
};

int main()
{
    dnager_array<int> a(3);

    a[0]=114514;
    a[1]=1919810;

    printf("%d %d\n",a[0],a[1]);

    dnager_array<bool> b(3);

    b[0]=true;
    b[1]=false;

    cout<<b[0]<<" "<<b[1]<<endl;

    return 0;
}