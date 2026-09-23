#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

class Animal
{
public:
    virtual void speak() = 0;
    virtual ~Animal() {}
};

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

class Zoo
{
private:
    vector<Animal*> animals;

public:
    void add(Animal* animal)
    {
        animals.push_back(animal);
    }

    void speakAll()
    {
        for (Animal* animal : animals)
        {
            animal->speak();
        }
    }
};

int main()
{
    Zoo zoo;

    Dog dog;
    Cat cat;

    zoo.add(&dog);
    zoo.add(&cat);

    zoo.speakAll();

    return 0;
}