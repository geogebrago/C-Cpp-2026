#include <bits/stdc++.h>
using namespace std;

// 链表节点
struct Node
{
    int value;
    Node* next;

    Node(int value)
    {
        this->value = value;
        this->next = nullptr;
    }
};

// 遍历链表
void printList(Node* head)
{
    Node* p = head;

    while (p != nullptr)
    {
        cout << p->value << " ";
        p = p->next;
    }

    cout << '\n';
}

// 反转链表
Node* reverseList(Node* head)
{
    Node* prev = nullptr;
    Node* cur = head;

    while (cur != nullptr)
    {
        Node* next = cur->next;

        cur->next = prev;

        prev = cur;
        cur = next;
    }

    return prev;
}

// 查找第一个值为 5 的节点
// 返回节点序号，不存在返回 -1
int findFirst(Node* head, int value)
{
    int index = 0;
    Node* p = head;

    while (p != nullptr)
    {
        if (p->value == value)
            return index;

        p = p->next;
        index++;
    }

    return -1;
}

// 查找指定位置之后的下一个值为 5 的节点
int findNext(Node* head, int value, int start)
{
    int index = 0;
    Node* p = head;

    while (p != nullptr)
    {
        if (index > start && p->value == value)
            return index;

        p = p->next;
        index++;
    }

    return -1;
}

int main()
{
    // 1. 创建单向链表
    Node* head = new Node(1);

    head->next = new Node(3);
    head->next->next = new Node(5);
    head->next->next->next = new Node(2);
    head->next->next->next->next = new Node(5);
    head->next->next->next->next->next = new Node(7);

    // 2. 遍历链表
    cout << "原链表：";
    printList(head);

    // 3. 反转链表
    head = reverseList(head);

    cout << "反转后：";
    printList(head);

    // 4. 查找第一个 5
    int first = findFirst(head, 5);

    cout << "第一个 5 的序号：" << first << '\n';

    // 5. 查找下一个 5
    int second = findNext(head, 5, first);

    cout << "下一个 5 的序号：" << second << '\n';

    return 0;
}//长大后学习