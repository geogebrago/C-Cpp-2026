/*#include <bits/stdc++.h>
#include <conio.h>
#include <Windows.h>
using namespace std;

const int MAX_FREE = 60;
const int BOX = 3;

struct StateKey
{
    uint16_t p;
    uint64_t mask;

    bool operator==(const StateKey &o) const noexcept
    {
        return p == o.p && mask == o.mask;
    }
};

struct StateHash
{
    size_t operator()(StateKey s) const noexcept
    {
        uint64_t x = s.mask ^ (uint64_t(s.p) * 0x9e3779b97f4a7c15ull);
        return hash<uint64_t>{}(x);
    }
};

struct Map
{
    vector<string> grid;
    int rows, cols;

    vector<pair<int, int>> idx2cell;
    vector<vector<int>> cell2idx;

    uint64_t goalMask = 0;

    bool inBounds(int r, int c) const
    {
        return r >= 0 && c >= 0 && r < rows && c < cols;
    }

    bool isWall(int r, int c) const
    {
        return !inBounds(r, c) || grid[r][c] == '#';
    }
};

struct Level
{
    Map mp;
    int player;
    uint64_t boxMask;
    int pushes;
};

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

bool isBox(uint64_t mask, int id)
{
    return (mask >> id) & 1ull;
}

bool isGoal(const Map &m, int id)
{
    return (m.goalMask >> id) & 1ull;
}

bool reachable(const Map &m, int s, int t, uint64_t mask)
{
    if (s == t)
        return true;

    vector<int> vis(m.idx2cell.size());

    queue<int> q;

    q.push(s);
    vis[s] = 1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        auto [r, c] = m.idx2cell[u];

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (m.isWall(nr, nc))
                continue;

            int v = m.cell2idx[nr][nc];

            if (v < 0)
                continue;

            if (isBox(mask, v))
                continue;

            if (vis[v])
                continue;

            if (v == t)
                return true;

            vis[v] = 1;
            q.push(v);
        }
    }

    return false;
}

bool isDeadlocked(const Map &m, uint64_t mask)
{
    int sz = m.idx2cell.size();

    vector<vector<bool>> occ(
        m.rows,
        vector<bool>(m.cols, false));

    for (int i = 0; i < sz; i++)
    {
        if (!isBox(mask, i))
            continue;

        auto [r, c] = m.idx2cell[i];

        occ[r][c] = true;
    }

    vector<vector<bool>> fz(
        m.rows,
        vector<bool>(m.cols, false));

    bool changed = true;

    while (changed)
    {
        changed = false;

        for (int r = 0; r < m.rows; r++)
        {
            for (int c = 0; c < m.cols; c++)
            {
                if (!occ[r][c] || fz[r][c])
                    continue;

                bool up = m.isWall(r - 1, c) || fz[r - 1][c];
                bool dn = m.isWall(r + 1, c) || fz[r + 1][c];
                bool lt = m.isWall(r, c - 1) || fz[r][c - 1];
                bool rt = m.isWall(r, c + 1) || fz[r][c + 1];

                if ((up && lt) || (up && rt) ||
                    (dn && lt) || (dn && rt))
                {
                    fz[r][c] = true;
                    changed = true;
                }
            }
        }

        for (int r = 0; r + 1 < m.rows; r++)
        {
            for (int c = 0; c + 1 < m.cols; c++)
            {
                int cnt = 0;

                cnt += (occ[r][c] || m.isWall(r, c));
                cnt += (occ[r + 1][c] || m.isWall(r + 1, c));
                cnt += (occ[r][c + 1] || m.isWall(r, c + 1));
                cnt += (occ[r + 1][c + 1] || m.isWall(r + 1, c + 1));

                if (cnt != 4)
                    continue;

                if (occ[r][c] && !fz[r][c])
                {
                    fz[r][c] = true;
                    changed = true;
                }

                if (occ[r + 1][c] && !fz[r + 1][c])
                {
                    fz[r + 1][c] = true;
                    changed = true;
                }

                if (occ[r][c + 1] && !fz[r][c + 1])
                {
                    fz[r][c + 1] = true;
                    changed = true;
                }

                if (occ[r + 1][c + 1] && !fz[r + 1][c + 1])
                {
                    fz[r + 1][c + 1] = true;
                    changed = true;
                }
            }
        }
    }

    for (int i = 0; i < sz; i++)
    {
        if (!isBox(mask, i))
            continue;

        auto [r, c] = m.idx2cell[i];

        if (fz[r][c] && !isGoal(m, i))
            return true;
    }

    return false;
}

bool finished(const Map &m, uint64_t mask)
{
    return mask == m.goalMask;
}

int minPushes(const Map &m, int player, uint64_t mask)
{
    if (mask == m.goalMask)
        return 0;

    unordered_map<StateKey, int, StateHash> dist;

    deque<StateKey> q;

    StateKey st{
        (uint16_t)player,
        mask};

    dist[st] = 0;
    q.push_back(st);

    while (!q.empty())
    {
        StateKey u = q.front();
        q.pop_front();

        int cd = dist[u];

        auto [r, c] = m.idx2cell[u.p];

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (m.isWall(nr, nc))
                continue;

            int ni = m.cell2idx[nr][nc];

            if (ni < 0)
                continue;

            /*
                不推箱子
            */

            if (!isBox(u.mask, ni))
            {
                StateKey v{
                    (uint16_t)ni,
                    u.mask};

                if (!dist.count(v))
                {
                    dist[v] = cd;
                    q.push_front(v);
                }

                continue;
            }

            /*
                推箱子
            */

            int br = nr + dr[d];
            int bc = nc + dc[d];

            if (m.isWall(br, bc))
                continue;

            int bi = m.cell2idx[br][bc];

            if (bi < 0 || isBox(u.mask, bi))
                continue;

            uint64_t nmask =
                u.mask ^ (1ull << ni);

            nmask |= (1ull << bi);

            StateKey v{
                (uint16_t)ni,
                nmask};

            if (!dist.count(v))
            {
                if (nmask == m.goalMask)
                    return cd + 1;

                dist[v] = cd + 1;
                q.push_back(v);
            }
        }
    }

    return -1;
}

Map buildMap(int rows, int cols, mt19937 &rng)
{
    Map m;

    m.rows = rows;
    m.cols = cols;

    m.grid.assign(
        rows,
        string(cols, '#'));

    m.cell2idx.assign(
        rows,
        vector<int>(cols, -1));

    /*
        Prim 风格迷宫
    */

    int sr = 1 + 2 * (rng() % ((rows - 2) / 2));
    int sc = 1 + 2 * (rng() % ((cols - 2) / 2));

    m.grid[sr][sc] = ' ';

    vector<pair<int, int>> cells;

    cells.push_back({sr, sc});

    while (!cells.empty())
    {
        int id = rng() % cells.size();

        auto [r, c] = cells[id];

        vector<int> ds = {0, 1, 2, 3};

        shuffle(ds.begin(), ds.end(), rng);

        bool ok = false;

        for (int d : ds)
        {
            int nr = r + dr[d] * 2;
            int nc = c + dc[d] * 2;

            if (nr <= 0 || nr >= rows - 1 ||
                nc <= 0 || nc >= cols - 1)
                continue;

            if (m.grid[nr][nc] != '#')
                continue;

            m.grid[r + dr[d]][c + dc[d]] = ' ';
            m.grid[nr][nc] = ' ';

            cells.push_back({nr, nc});

            ok = true;

            break;
        }

        if (!ok)
        {
            cells[id] = cells.back();
            cells.pop_back();
        }
    }

    /*
        开洞
    */

    for (int r = 1; r < rows - 1; r++)
    {
        for (int c = 1; c < cols - 1; c++)
        {
            if (m.grid[r][c] == '#' && rng() % 100 < 15)
                m.grid[r][c] = ' ';
        }
    }

    /*
        编号
    */

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (m.grid[r][c] == ' ')
            {
                int id = m.idx2cell.size();

                m.cell2idx[r][c] = id;
                m.idx2cell.push_back({r, c});
            }
        }
    }

    return m;
}

Level generateLevel(
    int rows,
    int cols,
    int boxCount,
    int reverseSteps,
    int minWanted,
    mt19937 &rng)
{
    while (true)
    {
        Map m = buildMap(rows, cols, rng);

        if (m.idx2cell.size() > MAX_FREE)
            continue;

        if (m.idx2cell.size() < boxCount + 1)
            continue;

        vector<int> ids(m.idx2cell.size());

        iota(ids.begin(), ids.end(), 0);

        shuffle(ids.begin(), ids.end(), rng);

        /*
            终局：

            $ $ $
            ↓ ↓ ↓
            . . .
        */

        m.goalMask = 0;

        for (int i = 0; i < boxCount; i++)
            m.goalMask |= 1ull << ids[i];

        /*
            反向开始
        */

        uint64_t mask = m.goalMask;

        int player = ids[boxCount];

        unordered_set<StateKey, StateHash> vis;

        vis.insert({(uint16_t)player,
                    mask});

        int realSteps = 0;

        for (int step = 0; step < reverseSteps; step++)
        {
            vector<int> boxes;

            for (int i = 0; i < (int)m.idx2cell.size(); i++)
            {
                if (isBox(mask, i))
                    boxes.push_back(i);
            }

            shuffle(boxes.begin(), boxes.end(), rng);

            bool moved = false;

            /*
                反向拉箱

                当前：

                    玩家
                     ↓
                    [箱]

                反向需要玩家先走到箱子的另一侧：

                    玩家
                     ↓
                    [箱]
                     ↓
                  拉箱方向
            */

            for (int bi : boxes)
            {
                auto [br, bc] = m.idx2cell[bi];

                vector<int> ds = {0, 1, 2, 3};

                shuffle(ds.begin(), ds.end(), rng);

                for (int d : ds)
                {
                    /*
                        箱子向 -d 方向移动

                        oldBox = br-dr[d]
                        玩家需要站在 br+dr[d]
                    */

                    int oldR = br - dr[d];
                    int oldC = bc - dc[d];

                    int standR = br + dr[d];
                    int standC = bc + dc[d];

                    if (!m.inBounds(oldR, oldC) ||
                        !m.inBounds(standR, standC))
                        continue;

                    int oldBox = m.cell2idx[oldR][oldC];
                    int stand = m.cell2idx[standR][standC];

                    if (oldBox < 0 || stand < 0)
                        continue;

                    /*
                        oldBox 不能有箱子
                        stand  不能有箱子
                    */

                    if (isBox(mask, oldBox) ||
                        isBox(mask, stand))
                        continue;

                    /*
                        当前玩家能否走到箱子另一边
                    */

                    if (!reachable(
                            m,
                            player,
                            stand,
                            mask))
                        continue;

                    uint64_t nmask = mask;

                    nmask ^= 1ull << bi;
                    nmask |= 1ull << oldBox;

                    /*
                        死锁剪枝
                    */

                    if (isDeadlocked(m, nmask))
                        continue;

                    StateKey nxt{
                        (uint16_t)bi,
                        nmask};

                    /*
                        反向完成一次拉箱：

                        箱子：
                        bi -> oldBox

                        玩家：
                        stand -> bi
                    */

                    if (vis.count(nxt))
                        continue;

                    vis.insert(nxt);

                    mask = nmask;
                    player = bi;

                    moved = true;
                    realSteps++;

                    break;
                }

                if (moved)
                    break;
            }

            if (!moved)
                break;
        }

        /*
            反向至少动了一些
        */

        if (realSteps < boxCount * 2)
            continue;

        /*
            正向验证

            这是最终保险。
        */

        int pushes = minPushes(
            m,
            player,
            mask);

        if (pushes < 0)
            continue;

        if (pushes < minWanted)
            continue;

        return {
            m,
            player,
            mask,
            pushes};
    }
}

void printLevel(
    const Level &lv,
    int nowPush)
{
    system("cls");

    const Map &m = lv.mp;

    cout << "============================\n";
    cout << "        SOKOBAN\n";
    cout << "============================\n\n";

    for (int r = 0; r < m.rows; r++)
    {
        for (int c = 0; c < m.cols; c++)
        {
            int id = m.cell2idx[r][c];

            if (id < 0)
            {
                cout << "#";
                continue;
            }

            bool g = isGoal(m, id);
            bool b = isBox(lv.boxMask, id);
            bool p = id == lv.player;

            if (p)
                cout << (g ? '+' : '@');
            else if (b)
                cout << (g ? '*' : '$');
            else if (g)
                cout << '.';
            else
                cout << ' ';
        }

        cout << "\n";
    }

    cout << "\n";
    cout << "@ 玩家\n";
    cout << "$ 箱子\n";
    cout << ". 目标\n";
    cout << "* 箱子 + 目标\n";
    cout << "+ 玩家 + 目标\n\n";

    cout << "当前推箱: " << nowPush << "\n";
    cout << "理论最少: " << lv.pushes << "\n\n";

    cout << "WASD / 方向键 移动\n";
    cout << "R 重新生成\n";
    cout << "ESC 退出\n";
}

void movePlayer(Level &lv, int d, int &pushes)
{
    const Map &m = lv.mp;

    auto [r, c] = m.idx2cell[lv.player];

    int nr = r + dr[d];
    int nc = c + dc[d];

    if (m.isWall(nr, nc))
        return;

    int ni = m.cell2idx[nr][nc];

    if (ni < 0)
        return;

    /*
        普通移动
    */

    if (!isBox(lv.boxMask, ni))
    {
        lv.player = ni;
        return;
    }

    /*
        推箱子
    */

    int br = nr + dr[d];
    int bc = nc + dc[d];

    if (m.isWall(br, bc))
        return;

    int bi = m.cell2idx[br][bc];

    if (bi < 0)
        return;

    if (isBox(lv.boxMask, bi))
        return;

    /*
        推箱
    */

    lv.boxMask ^= 1ull << ni;
    lv.boxMask |= 1ull << bi;

    lv.player = ni;

    pushes++;
}

bool win(const Level &lv)
{
    return lv.boxMask == lv.mp.goalMask;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    mt19937 rng(
        chrono::steady_clock::now()
            .time_since_epoch()
            .count());

    const int ROW = 11;
    const int COL = 15;

    while (true)
    {
        Level lv = generateLevel(
            ROW,
            COL,
            BOX,
            80,
            12,
            rng);

        int pushes = 0;

        while (true)
        {
            printLevel(lv, pushes);

            if (win(lv))
            {
                cout << "\n============================\n";
                cout << "          通关！\n";
                cout << "============================\n";
                cout << "你的推箱次数: " << pushes << "\n";
                cout << "理论最少次数: " << lv.pushes << "\n";
                cout << "\n按 R 重新生成\n";
                cout << "按 ESC 退出\n";

                int c = _getch();

                if (c == 27)
                    return 0;

                if (c == 'r' || c == 'R')
                    break;

                continue;
            }

            int c = _getch();

            if (c == 27)
                return 0;

            if (c == 'r' || c == 'R')
                break;

            if (c == 224)
            {
                c = _getch();

                if (c == 72)
                    movePlayer(lv, 0, pushes);

                if (c == 80)
                    movePlayer(lv, 1, pushes);

                if (c == 75)
                    movePlayer(lv, 2, pushes);

                if (c == 77)
                    movePlayer(lv, 3, pushes);

                continue;
            }

            if (c == 'w' || c == 'W')
                movePlayer(lv, 0, pushes);

            if (c == 's' || c == 'S')
                movePlayer(lv, 1, pushes);

            if (c == 'a' || c == 'A')
                movePlayer(lv, 2, pushes);

            if (c == 'd' || c == 'D')
                movePlayer(lv, 3, pushes);
        }
    }
}*/