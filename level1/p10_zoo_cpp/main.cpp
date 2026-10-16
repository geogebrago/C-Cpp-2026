#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

class Animal
{
public:
    virtual void speak() = 0;
    virtual ~Animal() = default;
};

namespace {
    class Dog : public Animal
    {
    public:
        void speak() override
        {
            cout << "汪汪！" << endl;
            Beep(800, 200);
            Beep(600, 200);
        }
    };

    class Cat : public Animal
    {
    public:
        void speak() override
        {
            cout << "喵喵！" << endl;
            Beep(1000, 150);
            Beep(800, 150);
        }
    };

    class Bird : public Animal
    {
    public:
        void speak() override
        {
            cout << "啾啾！" << endl;
            Beep(1200, 100);
            Beep(1000, 100);
        }
    };

    class WolfDog : public Animal
    {
    public:
        void speak() override
        {
            cout << "嗷嗷！" << endl;
            Beep(500, 200);
            Beep(400, 200);
        }
    };
}

class Zoo
{
private:
    vector<Animal*> animals;

public:
    void add(Animal* animal)
    {
        if (animal != nullptr)
        {
            animals.push_back(animal);
        }
    }

    void speakAll()
    {
        for (Animal* animal : animals)
        {
            if (animal != nullptr)
            {
                animal->speak();
            }
        }
    }
};

int main()
{
    Zoo zoo;

    Dog dog;
    Cat cat;
    Bird bird;
    WolfDog wolfDog;

    zoo.add(&dog);
    zoo.add(&cat);
    zoo.add(&bird);
    zoo.add(&wolfDog);

    zoo.speakAll();

    return 0;
}


/*#include<bits/stdc++.h>
#include<Windows.h>
using namespace std;

const int MIN_DB=37;
const int MAX_DB=32767;

class Animal
{
    string name;
    string voice;
    int db;

public:
    Animal(string n,string v,int d)
    {
        name=n;
        voice=v;
        db=d;
    }

    string getName()
    {
        return name;
    }

    void speak()
    {
        cout<<name<<" : "<<voice<<"\n";
        Beep(db,500);
    }
};

class Zoo
{
    vector<Animal*> animals;

public:
    void add(Animal* s)
    {
        if(s==nullptr)
        {
            printf("动物无效，无法添加\n");
            return;
        }

        for(auto x:animals)
        {
            if(x->getName()==s->getName())
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
        for(auto it=animals.begin();it!=animals.end();++it)
        {
            if((*it)->getName()==name)
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
        if(animals.empty())
        {
            printf("动物园里没有动物\n");
            return;
        }

        for(auto x:animals)
        {
            x->speak();
        }
    }

    ~Zoo()
    {
        for(auto x:animals)
        {
            delete x;
        }
    }
};

bool inputInt(int &x)
{
    if(scanf("%d",&x)!=1)
    {
        int c;
        while((c=getchar())!='\n'&&c!=EOF);
        return false;
    }

    return true;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Zoo zoo;

    zoo.add(new Animal("Cat","MiaoMiao",114));
    zoo.add(new Animal("Dog","WangWang",514));
    zoo.add(new Animal("Bird","JiJi",1000));
    zoo.add(new Animal("WolfDog","Aoooooo",400));

    while(1)
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

        if(!inputInt(choice))
        {
            printf("输入无效，请输入 1 ~ 4\n");
            continue;
        }

        switch(choice)
        {
        case 1:
            {
                string name,voice;
                int db;

                printf("请输入动物名称: ");
                cin>>name;

                if(name.empty())
                {
                    printf("动物名称不能为空\n");
                    break;
                }

                printf("请输入动物叫声: ");
                cin>>voice;

                if(voice.empty())
                {
                    printf("动物叫声不能为空\n");
                    break;
                }

                printf("请输入动物叫声频率: ");

                if(!inputInt(db))
                {
                    printf("频率输入无效\n");
                    break;
                }

                if(db<MIN_DB||db>MAX_DB)
                {
                    printf("频率范围应为 %d ~ %d\n",MIN_DB,MAX_DB);
                    break;
                }

                zoo.add(new Animal(name,voice,db));
                break;
            }

        case 2:
            {
                string name;

                printf("请输入动物名称: ");
                cin>>name;

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
}*/