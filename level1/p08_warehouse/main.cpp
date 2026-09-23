#include <bits/stdc++.h>
#include<Windows.h>
using namespace std;

struct Goods
{
    string id;      // 货物型号
    int quantity;   // 数量
};

vector<Goods> goods;

// 显示库存
void show()
{
    cout << "\n===== 存货列表 =====\n";

    if (goods.empty())
    {
        cout << "当前没有库存\n";
        return;
    }

    cout << left << setw(15) << "型号"
         << setw(10) << "数量" << '\n';

    for (const auto &g : goods)
    {
        cout << left << setw(15) << g.id
             << setw(10) << g.quantity << '\n';
    }
}

// 查找货物
int findGoods(const string &id)
{
    for (int i = 0; i < (int)goods.size(); ++i)
    {
        if (goods[i].id == id)
            return i;
    }

    return -1;
}

// 入库
void in()
{
    string id;
    int quantity;

    cout << "请输入货物型号：";
    cin >> id;

    cout << "请输入入库数量：";
    cin >> quantity;

    if (quantity <= 0)
    {
        cout << "数量必须大于 0\n";
        return;
    }

    int pos = findGoods(id);

    if (pos == -1)
    {
        // 新货物
        goods.push_back({id, quantity});
    }
    else
    {
        // 已经存在，直接增加数量
        goods[pos].quantity += quantity;
    }

    cout << "入库成功\n";
}

// 出库
void out()
{
    string id;
    int quantity;

    cout << "请输入货物型号：";
    cin >> id;

    int pos = findGoods(id);

    if (pos == -1)
    {
        cout << "没有找到该货物\n";
        return;
    }

    cout << "当前库存：" << goods[pos].quantity << '\n';

    cout << "请输入出库数量：";
    cin >> quantity;

    if (quantity <= 0)
    {
        cout << "数量必须大于 0\n";
        return;
    }

    if (quantity > goods[pos].quantity)
    {
        cout << "库存不足\n";
        return;
    }

    goods[pos].quantity -= quantity;

    cout << "出库成功\n";
}

// 从文件读取库存
void load()
{
    ifstream fin("warehouse.txt");

    if (!fin)
    {
        // 文件不存在，说明可能是第一次运行
        return;
    }

    Goods g;

    while (fin >> g.id >> g.quantity)
    {
        goods.push_back(g);
    }

    fin.close();
}

// 保存库存到文件
void save()
{
    ofstream fout("warehouse.txt");

    for (const auto &g : goods)
    {
        fout << g.id << ' '
             << g.quantity << '\n';
    }

    fout.close();
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // 程序启动时读取库存
    load();
    while (true)
    {
        cout << "\n====================\n";
        cout << "     简单进销存\n";
        cout << "====================\n";
        cout << "1. 显示存货列表\n";
        cout << "2. 入库\n";
        cout << "3. 出库\n";
        cout << "4. 退出程序\n";
        cout << "请选择：";

        int choice;
        cin >> choice;
        system("cls");
        if (choice == 1)
        {
            show();
        }
        else if (choice == 2)
        {
            in();
        }
        else if (choice == 3)
        {
            out();
        }
        else if (choice == 4)
        {
            // 退出前保存库存
            save();
            cout << "库存已保存，程序退出\n";
            break;
        }
        else
        {
            cout << "输入错误，请重新选择\n";
        }
    }

    return 0;
}