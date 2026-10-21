#include<bits/stdc++.h>
using namespace std;

template<typename T>
class safe_array
{
private:
    T *a;
    int n;

public:
    safe_array(int sz)
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

    ~safe_array()
    {
        delete[] a;
    }
    safe_array(const safe_array &)=delete;
    safe_array &operator=(const safe_array &)=delete;
    T &operator[](int i)
    {
        if (i<0||i>=n)
        {
            cout<<"下标越界"<<endl;
            exit(1);
        }

        return a[i];
    }
};

int main()
{
    safe_array<int> a(3);
    a[0]=114514;
    a[1]=1919810;

    cout<<a[0]<<" "<<a[1]<<endl;

    safe_array<bool> b(3);

    b[0]=true;
    b[1]=false;

    cout<<b[0]<<" "<<b[1]<<endl;

    return 0;
}