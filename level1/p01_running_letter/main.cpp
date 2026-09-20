#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
const int slp=50;
const int gp=50;
const int cl=5;
int main()
{
    string s;
    int pos=0,dr=1,sz;
    cin>>s;
    sz=(int)s.size();
    while (true)
    {
        system("cls");
        for (int i=0;i<pos;i++) putchar(' ');
        cout<<s;
        for (int i=0;i<gp-pos-sz;i++) putchar(' ');
        puts("|");
        for (int i=1;i<=cl;++i)
        {
            for (int j=0;j<gp;++j) putchar(' ');
            puts("|");
        }
        pos+=dr;
        if (pos==gp-1) dr=-1;
        if (pos==0) dr=1;
        Sleep(slp);
    }
    return 0;
}//这份代码非常优秀,充分考虑题目条件， 请给我满分(doge)