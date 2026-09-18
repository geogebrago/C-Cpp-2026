#include <iostream>
#include <windows.h> // 确保引入了这个头文件以使用 Sleep
#include <string>
using namespace std;

int main() {
    string word = "HELLO";
    int pos = 0;
    int dir = 1;
    int width = 50;

    while (true) {
        // 注意：在 CLion 中直接运行可能清屏不彻底，建议在外部终端运行
        system("cls");
        cout << "\r"; // 1. 每次循环先把光标移回最前面（行首）

        for (int i = 0; i < pos; i++) {
            cout << " ";
        }
        cout << word; // 2. 输出单词

        cout.flush(); // 3. 强制立刻刷新屏幕，不要等待

        Sleep(100); // 4. 停顿 100 毫秒，让眼睛能看清

        pos += dir;

        if (pos <= 0) {
            dir = 1;
        }
        if (pos + 5 >= width) { // 5 是 word 的长度
            dir = -1;
        }
    }
    return 0;
}
