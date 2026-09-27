#include<bits/stdc++.h>
#include<Windows.h>
using namespace std;

class Animal
{
    string name;
    string voice;
    int db;

public:
    Animal(string n, string v, int d)
    {
        name = n;
        voice = v;
        db = d;
    }

    string getName()
    {
        return name;
    }

    void speak()
    {
        cout << name << " : " << voice << "\n";
        Beep(db, 500);
    }
};

class Zoo
{
    vector<Animal*> animals;

public:
    void add(Animal* s)
    {
        for (auto x : animals)
        {
            if (x->getName() == s->getName())
            {
                printf("动物已存在，无法添加\n");
                delete s;
                return;
            }
        }

        animals.push_back(s);
        printf("添加成功\n");
    }

    void remove(string name)
    {
        for (auto it = animals.begin(); it != animals.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                delete *it;
                animals.erase(it);
                printf("删除成功\n");
                return;
            }
        }

        printf("动物不存在，无法删除\n");
    }

    void speak()
    {
        if (animals.empty())
        {
            printf("动物园里没有动物\n");
            return;
        }

        for (auto x : animals)
        {
            x->speak();
        }
    }

    ~Zoo()
    {
        for (auto x : animals)
        {
            delete x;
        }
    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Zoo zoo;

    zoo.add(new Animal("Cat", "MiaoMiao", 114));
    zoo.add(new Animal("Dog", "WangWang", 514));
    zoo.add(new Animal("Cow", "MooMoo", 1919));
    zoo.add(new Animal("Sheep", "BaaBaa", 810));
    while (1)
    {
        printf("\n");
        printf("====================\n");
        printf("    动物园管理系统\n");
        printf("====================\n");
        printf("1. 添加动物\n");
        printf("2. 删除动物\n");
        printf("3. 动物叫叫叫\n");
        printf("4. 退出\n");
        printf("请输入选项: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                string name, voice;
                int db;

                printf("请输入动物名称: ");
                cin >> name;

                printf("请输入动物叫声: ");
                cin >> voice;

                printf("请输入动物叫声频率: ");
                scanf("%d", &db);

                if (db < 37 || db > 32767)
                {
                    printf("频率范围应为 37 ~ 32767\n");
                    break;
                }

                zoo.add(new Animal(name, voice, db));
                break;
            }

            case 2:
            {
                string name;

                printf("请输入动物名称: ");
                cin >> name;

                zoo.remove(name);
                break;
            }

            case 3:
            {
                printf("动物叫叫叫\n");
                zoo.speak();
                break;
            }

            case 4:
            {
                printf("退出\n");
                return 0;
            }

            default:
            {
                printf("无效的选择，请输入 1 ~ 4\n");
                break;
            }
        }
    }

    return 0;
}