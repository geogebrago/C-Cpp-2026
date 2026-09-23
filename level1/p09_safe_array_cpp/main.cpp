#include <iostream>
using namespace std;

class SafeArray
{
private:
    int *data;
    int size;

public:
    SafeArray(int n)
    {
        size = n;
        data = new int[size];
    }

    ~SafeArray()
    {
        delete[] data;
    }

    void set(int index, int value)
    {
        if (index < 0 || index >= size)
        {
            cout << "数组下标越界！" << endl;
            return;
        }

        data[index] = value;
    }

    int get(int index)
    {
        if (index < 0 || index >= size)
        {
            cout << "数组下标越界！" << endl;
            return 0;
        }

        return data[index];
    }
};

int main()
{
    SafeArray a(5);

    a.set(0, 10);
    a.set(1, 20);
    a.set(4, 50);

    cout << a.get(0) << endl;
    cout << a.get(4) << endl;

    a.set(5, 100);   // 越界
    cout << a.get(5) << endl;  // 越界

    return 0;
}