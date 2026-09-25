#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <limits.h>

const int N=105,M=1005;

int x,tot;
double y;
char r[N];

struct nd
{
    char id[N];
    int num;
    double w;
}s[N];

void clear_input()
{
    while (getchar()!='\n');
}

void load()
{
    FILE *fp=fopen(
        "C:/Users/26346/CLionProjects/C-Cpp-2026/level1/p08_warehouse/goods.txt",
        "r"
    );

    if (fp==NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    while (tot<N-1&&fscanf(fp,"%104s %lf %d",r+1,&y,&x)==3)
    {
        if (y<=0||x<0)
            continue;

        ++tot;
        strcpy(s[tot].id+1,r+1);
        s[tot].num=x;
        s[tot].w=y;
    }

    fclose(fp);
}

int fd(char *tp)
{
    for (int i=1;i<=tot;i++)
        if (strcmp(tp,s[i].id+1)==0)
            return i;

    return -1;
}

void e_nd()
{
    for (int i=1;i<=tot;i++)
        printf("%-15s %-15.2f %-15d\n",
               s[i].id+1,s[i].w,s[i].num);
}

void show()
{
    printf("\n=========存货列表·===========\n");

    if (tot==0)
    {
        printf("当前没有商品\n");
        return;
    }

    printf("%-15s %-15s %-15s\n","型号","单价","数量");
    printf("---------------------------------------------\n");

    e_nd();
}

void i_n()
{
    printf("\n========== 入库 ==========\n");

    if (tot>=N-1)
    {
        printf("商品种类已达到上限！\n");
        return;
    }

    printf("请输入商品型号：");

    if (scanf("%104s",r+1)!=1)
    {
        printf("输入错误！\n");
        clear_input();
        return;
    }

    int pos=fd(r+1);

    if (pos==-1)
    {
        ++tot;
        strcpy(s[tot].id+1,r+1);

        while (1)
        {
            printf("请输入商品数量：\n");

            if (scanf("%d",&s[tot].num)!=1)
            {
                printf("请输入整数！\n");
                clear_input();
                continue;
            }

            if (s[tot].num>0)
                break;

            printf("商品数量必须大于0！\n");
        }

        while (1)
        {
            printf("请输入商品价格：\n");

            if (scanf("%lf",&s[tot].w)!=1)
            {
                printf("请输入数字！\n");
                clear_input();
                continue;
            }

            if (s[tot].w>0)
                break;

            printf("商品价格必须大于0！\n");
        }
    }
    else
    {
        printf("当前库存：%d\n",s[pos].num);

        while (1)
        {
            printf("请输入商品入库数量：\n");

            if (scanf("%d",&x)!=1)
            {
                printf("请输入整数！\n");
                clear_input();
                continue;
            }

            if (x>0)
                break;

            printf("入库数量必须大于0！\n");
        }

        if (s[pos].num>INT_MAX-x)
        {
            printf("库存数量过大！\n");
            return;
        }

        s[pos].num+=x;
    }

    printf("已入库\n");
}

void o_t()
{
    printf("\n========== 出库 ==========\n");

    int pos=-1;

    while (1)
    {
        printf("请输入商品型号：");

        if (scanf("%104s",r+1)!=1)
        {
            printf("输入错误！\n");
            clear_input();
            continue;
        }

        pos=fd(r+1);

        if (pos!=-1)
            break;

        printf("不存在该商品！\n");
    }

    printf("当前库存：%d\n",s[pos].num);

    while (1)
    {
        printf("请输入商品出库数量：\n");

        if (scanf("%d",&x)!=1)
        {
            printf("请输入整数！\n");
            clear_input();
            continue;
        }

        if (0<x&&x<=s[pos].num)
            break;

        printf("出库数量必须大于0且不能超过当前库存！\n");
    }

    s[pos].num-=x;

    printf("已出库\n");
}

void save()
{
    FILE *fp=fopen(
        "C:/Users/26346/CLionProjects/C-Cpp-2026/level1/p08_warehouse/goods.txt",
        "w"
    );

    if (fp==NULL)
    {
        printf("文件打开失败，数据保存失败！\n");
        return;
    }

    for (int i=1;i<=tot;i++)
        fprintf(fp,"%s %.2f %d\n",
                s[i].id+1,s[i].w,s[i].num);

    fclose(fp);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    load();

    while (1)
    {
        printf("\n====================\n");
        printf("     简单进销存\n");
        printf("====================\n");
        printf("1. 显示存货列表\n");
        printf("2. 入库\n");
        printf("3. 出库\n");
        printf("4. 退出程序\n");
        printf("请选择：\n");

        if (scanf("%d",&x)!=1)
        {
            printf("请输入1~4的数字！\n");
            clear_input();
            continue;
        }

        if (x==1)
        {
            show();
        }
        else if (x==2)
        {
            i_n();
        }
        else if (x==3)
        {
            o_t();
        }
        else if (x==4)
        {
            save();
            printf("程序已退出！\n");
            return 0;
        }
        else
        {
            printf("请输入1~4！\n");
        }
    }

    return 0;
}
