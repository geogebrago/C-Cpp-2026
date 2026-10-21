#include <bits/stdc++.h>
using namespace std;

int MAX_LEVEL;
uint32_t seed = 114514;

uint32_t nextRandom()
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

struct Node
{
    int key, level;
    Node **forward;
    int *span;
    Node *inlineForward[4]{};
    int inlineSpan[4]{};

    Node(int k, int lvl)
    {
        key = k, level = lvl;
        if (lvl <= 4)
        {
            forward = inlineForward;
            span = inlineSpan;
            fill(forward, forward + lvl, nullptr);
            fill(span, span + lvl, 0);
        }
        else
        {
            forward = new Node *[lvl];
            span = new int[lvl];
            fill_n(forward, lvl, nullptr);
            fill_n(span, lvl, 0);
        }
    }

    ~Node()
    {
        if (level > 4)
        {
            delete[] forward;
            delete[] span;
        }
    }
};

class SkipList
{
private:
    Node *head;
    int level;

    static int randomLevel()
    {
        uint32_t r = nextRandom();
        if (r == UINT32_MAX)
            return MAX_LEVEL;
        int zeros = __builtin_ctz(~r);
        return zeros >= MAX_LEVEL - 1 ? MAX_LEVEL : zeros + 1;
    }

public:
    SkipList()
    {
        head = new Node(0, MAX_LEVEL);
        level = 1;
    }

    ~SkipList()
    {
        Node *cur = head->forward[0];
        while (cur)
        {
            Node *nt = cur->forward[0];
            delete cur;
            cur = nt;
        }
        delete head;
    }

    void build(int *values, int n)
    {
        if (n == 0)
        {
            level = 1;
            fill_n(head->forward, MAX_LEVEL, nullptr);
            fill_n(head->span, MAX_LEVEL, 0);
            return;
        }

        sort(values, values + n);
        level = min(MAX_LEVEL, __lg(n) + 1);

        int *nodeLevels = new int[n];
        for (int i = 0; i < n; ++i)
        {
            if (i == 0)
                nodeLevels[i] = level;
            else
            {
                int k = __builtin_ctz(static_cast<unsigned int>(i)) + 1;
                nodeLevels[i] = k < level ? k : level;
            }
        }

        Node **nodes = new Node *[n];
        for (int i = 0; i < n; ++i)
            nodes[i] = new Node(values[i], nodeLevels[i]);

        fill_n(head->forward, MAX_LEVEL, nullptr);
        fill_n(head->span, MAX_LEVEL, 0);

        head->forward[0] = nodes[0];
        for (int i = 0; i < n; ++i)
        {
            nodes[i]->forward[0] = (i + 1 < n) ? nodes[i + 1] : nullptr;
            nodes[i]->span[0] = 1;
        }
        head->span[0] = 1;

        for (int i = 1; i < level; ++i)
        {
            Node *pre = head;
            Node *cur = head->forward[0];
            pre->forward[i] = cur;
            pre->span[i] = 1;
            pre = cur;
            cur = cur->forward[i - 1];
            if (cur)
                cur = cur->forward[i - 1];

            int stp = 1 << i;
            while (cur)
            {
                pre->forward[i] = cur;
                pre->span[i] = stp;
                pre = cur;
                cur = cur->forward[i - 1];
                if (cur)
                    cur = cur->forward[i - 1];
            }
        }

        delete[] nodeLevels;
        delete[] nodes;
    }

    void insert(int x)
    {
        Node *update[MAX_LEVEL];
        int ranks[MAX_LEVEL];
        Node *p = head;

        for (int i = level - 1; i >= 0; --i)
        {
            ranks[i] = (i == level - 1) ? 0 : ranks[i + 1];
            while (p->forward[i] && p->forward[i]->key < x)
            {
                ranks[i] += p->span[i];
                p = p->forward[i];
            }
            update[i] = p;
        }

        int newLevel = randomLevel();
        if (newLevel > level)
        {
            for (int i = level; i < newLevel; ++i)
            {
                update[i] = head;
                ranks[i] = 0;
                head->span[i] = 0;
                head->forward[i] = nullptr;
            }
            level = newLevel;
        }

        Node *newNode = new Node(x, newLevel);
        for (int i = 0; i < newLevel; ++i)
        {
            newNode->forward[i] = update[i]->forward[i];
            if (update[i]->forward[i])
                newNode->span[i] =
                    update[i]->span[i] - (ranks[0] - ranks[i]);
            else
                newNode->span[i] = 0;

            update[i]->forward[i] = newNode;
            update[i]->span[i] = ranks[0] - ranks[i] + 1;
        }

        for (int i = newLevel; i < level; ++i)
            ++update[i]->span[i];
    }

    void erase(int x)
    {
        Node *update[MAX_LEVEL];
        Node *p = head;

        for (int i = level - 1; i >= 0; --i)
        {
            while (p->forward[i] && p->forward[i]->key < x)
                p = p->forward[i];
            update[i] = p;
        }

        p = p->forward[0];
        if (p && p->key == x)
        {
            for (int i = 0; i < level; ++i)
            {
                if (update[i]->forward[i] == p)
                {
                    update[i]->span[i] += p->span[i] - 1;
                    update[i]->forward[i] = p->forward[i];
                }
                else
                {
                    --update[i]->span[i];
                }
            }
            delete p;

            while (level > 1 && head->forward[level - 1] == nullptr)
                --level;
        }
    }

    int rank(int x)
    {
        Node *p = head;
        int r = 0;
        for (int i = level - 1; i >= 0; --i)
        {
            while (p->forward[i] && p->forward[i]->key < x)
            {
                r += p->span[i];
                p = p->forward[i];
            }
        }
        return r + 1;
    }

    int kth(int x)
    {
        Node *p = head;
        int r = 0;
        for (int i = level - 1; i >= 0; --i)
        {
            while (p->forward[i] && r + p->span[i] <= x)
            {
                r += p->span[i];
                p = p->forward[i];
            }
        }
        return p->key;
    }

    int predecessor(int x)
    {
        Node *p = head;
        for (int i = level - 1; i >= 0; --i)
        {
            while (p->forward[i] && p->forward[i]->key < x)
                p = p->forward[i];
        }
        if (p == head)
            return -1;
        return p->key;
    }

    int successor(int x)
    {
        Node *p = head;
        for (int i = level - 1; i >= 0; --i)
        {
            while (p->forward[i] && p->forward[i]->key <= x)
                p = p->forward[i];
        }
        if (p->forward[0] == nullptr)
            return -1;
        return p->forward[0]->key;
    }
};

namespace
{
bool readInt(int &x)
{
    int c = getchar();

    while (c != EOF && isspace(c))
        c = getchar();

    if (c == EOF)
        return false;

    int sign = 1;
    if (c == '-' || c == '+')
    {
        if (c == '-')
            sign = -1;
        c = getchar();
    }

    if (c < '0' || c > '9')
    {
        while (c != EOF && !isspace(c))
            c = getchar();
        return false;
    }

    long long value = 0;
    bool invalid = false;

    while (c >= '0' && c <= '9')
    {
        value = value * 10 + c - '0';

        if (value > (long long)INT_MAX + 1)
            invalid = true;

        c = getchar();
    }

    if (c != EOF && !isspace(c))
    {
        invalid = true;
        while (c != EOF && !isspace(c))
            c = getchar();
    }

    if (invalid)
        return false;

    value *= sign;

    if (value < INT_MIN || value > INT_MAX)
        return false;

    x = (int)value;
    return true;
}
}

int main()
{
    int n, m;

    if (!readInt(n) || !readInt(m) || n < 0 || m < 0)
        return 0;

    MAX_LEVEL = __lg(max(1, n + m)) + 2;

    SkipList sl;

    int *values = new int[n];
    for (int i = 0; i < n; ++i)
    {
        if (!readInt(values[i]))
        {
            delete[] values;
            return 0;
        }
    }

    sl.build(values, n);
    delete[] values;

    while (m--)
    {
        int opt, x;

        if (!readInt(opt) || !readInt(x))
            return 0;

        switch (opt)
        {
        case 1:
            sl.insert(x);
            break;
        case 2:
            sl.erase(x);
            break;
        case 3:
            printf("%d\n", sl.rank(x));
            break;
        case 4:
            printf("%d\n", sl.kth(x));
            break;
        case 5:
            printf("%d\n", sl.predecessor(x));
            break;
        case 6:
            printf("%d\n", sl.successor(x));
            break;
        default:
            break;
        }
    }

    return 0;
}