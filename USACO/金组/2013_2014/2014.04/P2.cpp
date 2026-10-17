// P2 Cow Optics
// 激光从 (0,0) 向北射出，场上已有 N 面 45 度镜子，Bessie 还要再放一面镜子，
// 问有多少个位置能让激光正好打到谷仓。
//
// 关键结论：新镜子必须同时落在"激光原本的光路"和"从谷仓某方向反向射出的光的光路"上。
// 因为若镜子不在激光原光路上，激光的走向完全不变（题目保证原本打不到谷仓）；
// 反过来看，从谷仓射出的光束若碰不到这面镜子，它也不可能把激光接到谷仓。
// 又因为两条光路在交点处必须互相垂直（否则激光拐弯后仍到不了谷仓），
// 所以答案就是对两族折线段做"竖直段 × 水平段"的交点计数。
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

const int MAXC = 1000000000;    // 坐标绝对值上限；射出场地用 ±(MAXC+1) 表示
const int LIMIT = MAXC + 1;

int N, Bx, By;

int dx[4] = {0, 1, 0, -1};      // 方向编号 0,1,2,3 依次为 北、东、南、西
int dy[4] = {1, 0, -1, 0};

// 一个物体：kind 为 '/'、'\\' 表示镜子，'S' 为激光起点，'B' 为谷仓。
// 后两者不反射光束而是把它"吸收"掉，因此光束撞上它们就停止。
// pos 是它在所属直线上的坐标：同一列上按 y 排，同一行上按 x 排。
struct Thing {
    int pos;
    char kind;
};

// 先比坐标再比类型，让同一坐标上的先后次序完全确定
bool cmpThing(const Thing &a, const Thing &b) {
    if (a.pos != b.pos) return a.pos < b.pos;
    return a.kind < b.kind;
}

map<int, vector<Thing> > byX;   // 每一列上的所有物体，已按 y 排序
map<int, vector<Thing> > byY;   // 每一行上的所有物体，已按 x 排序

struct Point { int x, y; };

// 求光束从 (x,y) 沿 dir 前进后最先撞到的物体（会跳过它自己所在的那个物体）
// 撞点写入 nx,ny，物体类型写入 kind；该方向上没有物体时表示射出场地，kind = '?'
void nextHit(int x, int y, int dir, int &nx, int &ny, char &kind) {
    bool vertical = (dir % 2 == 0);            // 偶数方向是竖直移动
    int key = vertical ? x : y;                // 所在直线（列或行）的坐标
    int from = vertical ? y : x;               // 出发点在该直线上的坐标
    int step = vertical ? dy[dir] : dx[dir];
    vector<Thing> &objs = vertical ? byX[key] : byY[key];

    // 二分出第一个坐标 >= from 的物体，再朝前进方向挪一格
    int lo = 0, hi = (int)objs.size();
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (objs[mid].pos < from) lo = mid + 1;
        else hi = mid;
    }
    int id = lo + step;

    kind = '?';
    if (id < 0) {                              // 该方向上没有物体，射出场地
        nx = vertical ? key : -LIMIT;
        ny = vertical ? -LIMIT : key;
    } else if (id >= (int)objs.size()) {
        nx = vertical ? key : LIMIT;
        ny = vertical ? LIMIT : key;
    } else {
        int p = objs[id].pos;
        kind = objs[id].kind;
        nx = vertical ? key : p;
        ny = vertical ? p : key;
    }
}

// 记录光束走过的所有节点（含起点和最终射出/被吸收的点）
vector<Point> getPath(int x, int y, int dir) {
    vector<Point> path;
    Point cur;
    cur.x = x;
    cur.y = y;
    path.push_back(cur);

    // 每个 (物体, 方向) 状态最多被经过一次，所以步数不会超过 4*(N+2)；
    // 加上这个上界只是为了防止数据中出现"打不到谷仓却成环"的光路导致死循环
    int limit = 4 * (N + 2) + 10;
    for (int s = 0; s < limit; s++) {
        int nx, ny;
        char kind;
        nextHit(cur.x, cur.y, dir, nx, ny, kind);
        cur.x = nx;
        cur.y = ny;
        path.push_back(cur);

        // 45 度镜面：'/' 把 北<->东、南<->西 互相转换，'\\' 把 北<->西、南<->东 互相转换
        if (kind == '/') dir = (dir + (dir % 2 != 0 ? 3 : 1)) % 4;
        else if (kind == '\\') dir = (dir + (dir % 2 == 0 ? 3 : 1)) % 4;
        else break;                            // '?'、'S'、'B' 都意味着光束到此为止
    }
    return path;
}

struct VSeg { int x, lo, hi; };  // 竖直段：x 固定，y 在 [lo, hi] 之间
struct HSeg { int y, lo, hi; };  // 水平段：y 固定，x 在 [lo, hi] 之间

bool cmpVSeg(const VSeg &a, const VSeg &b) {
    if (a.x != b.x) return a.x < b.x;
    if (a.lo != b.lo) return a.lo < b.lo;
    return a.hi < b.hi;
}

// 拆出光路里所有的竖直段
void getVerticals(const vector<Point> &path, vector<VSeg> &out) {
    for (int i = 0; i + 1 < (int)path.size(); i++) {
        if (path[i].x == path[i + 1].x) {
            VSeg v;
            v.x = path[i].x;
            v.lo = min(path[i].y, path[i + 1].y);
            v.hi = max(path[i].y, path[i + 1].y);
            out.push_back(v);
        }
    }
}

// 拆出光路里所有的水平段
void getHorizontals(const vector<Point> &path, vector<HSeg> &out) {
    for (int i = 0; i + 1 < (int)path.size(); i++) {
        if (path[i].y == path[i + 1].y) {
            HSeg h;
            h.y = path[i].y;
            h.lo = min(path[i].x, path[i + 1].x);
            h.hi = max(path[i].x, path[i + 1].x);
            out.push_back(h);
        }
    }
}

vector<int> bitTree;            // 树状数组，内部下标从 1 开始
int bitSize;

// 离散化后的纵坐标单点加（i 从 0 开始）
void bitAdd(int i, int v) {
    for (i++; i <= bitSize; i += i & -i) bitTree[i] += v;
}

// 离散化下标落在 [0, i) 的权值和
int bitSum(int i) {
    int s = 0;
    for (; i > 0; i -= i & -i) s += bitTree[i];
    return s;
}

struct Event {
    int x;      // 事件所在的 x 坐标
    int yIdx;   // 该水平段离散化后的 y
    int delta;  // +1 表示水平段开始，-1 表示结束
};

bool cmpEvent(const Event &a, const Event &b) {
    if (a.x != b.x) return a.x < b.x;
    if (a.yIdx != b.yIdx) return a.yIdx < b.yIdx;
    return a.delta < b.delta;
}

// 统计竖直段与水平段的交点个数。
// 把水平段的两端当作事件按 x 从左到右扫描：每条竖直段都在"它左侧第一个事件之后再看到"
// 的时刻被统计一次，那时树里恰好是满足 xlo <= x0 < xhi 的水平段。
// 纵坐标上使用半开区间 (lo, hi]：与下半端点重合的交点不算，
// 这样落在镜子（即光路拐点）上的"伪交点"会被排除掉。
long long countInts(vector<VSeg> vs, vector<HSeg> hs) {
    // 纵坐标离散化：竖直段的两端和所有水平段的 y 都可能成为查询边界
    vector<int> ys;
    for (int i = 0; i < (int)vs.size(); i++) {
        ys.push_back(vs[i].lo);
        ys.push_back(vs[i].hi);
    }
    for (int i = 0; i < (int)hs.size(); i++) ys.push_back(hs[i].y);
    sort(ys.begin(), ys.end());
    ys.resize(unique(ys.begin(), ys.end()) - ys.begin());

    for (int i = 0; i < (int)vs.size(); i++) {
        vs[i].lo = (int)(lower_bound(ys.begin(), ys.end(), vs[i].lo) - ys.begin());
        vs[i].hi = (int)(lower_bound(ys.begin(), ys.end(), vs[i].hi) - ys.begin());
    }
    for (int i = 0; i < (int)hs.size(); i++)
        hs[i].y = (int)(lower_bound(ys.begin(), ys.end(), hs[i].y) - ys.begin());

    sort(vs.begin(), vs.end(), cmpVSeg);

    vector<Event> ev;
    for (int i = 0; i < (int)hs.size(); i++) {
        Event e;
        e.x = hs[i].lo;
        e.yIdx = hs[i].y;
        e.delta = 1;
        ev.push_back(e);
        e.x = hs[i].hi;
        e.delta = -1;
        ev.push_back(e);
    }
    sort(ev.begin(), ev.end(), cmpEvent);

    bitTree.assign(ys.size() + 1, 0);
    bitSize = (int)ys.size();

    long long res = 0;
    int j = 0;                                  // 还没统计过的竖直段
    for (int i = 0; i < (int)ev.size(); i++) {
        while (j < (int)vs.size() && vs[j].x < ev[i].x) {
            // 此刻树里留下的水平段都跨过了这条竖线段的 x，再按 y 区间筛选
            res += bitSum(vs[j].hi + 1) - bitSum(vs[j].lo + 1);
            j++;
        }
        bitAdd(ev[i].yIdx, ev[i].delta);
    }
    return res;
}

int main() {
    scanf("%d %d %d", &N, &Bx, &By);

    // 把激光起点和谷仓也登记成物体：前者收住跑回起点的光，后者收住打到谷仓的光
    Thing t;
    t.pos = 0;
    t.kind = 'S';
    byX[0].push_back(t);
    byY[0].push_back(t);
    t.pos = By;
    t.kind = 'B';
    byX[Bx].push_back(t);
    t.pos = Bx;
    byY[By].push_back(t);

    for (int i = 0; i < N; i++) {
        int x, y;
        char buf[8];
        scanf("%d %d %7s", &x, &y, buf);
        Thing a;
        a.pos = y;
        a.kind = buf[0];
        byX[x].push_back(a);
        Thing b;
        b.pos = x;
        b.kind = buf[0];
        byY[y].push_back(b);
    }
    for (map<int, vector<Thing> >::iterator it = byX.begin(); it != byX.end(); ++it)
        sort(it->second.begin(), it->second.end(), cmpThing);
    for (map<int, vector<Thing> >::iterator it = byY.begin(); it != byY.end(); ++it)
        sort(it->second.begin(), it->second.end(), cmpThing);

    vector<Point> laser = getPath(0, 0, 0);
    vector<VSeg> laserV;
    vector<HSeg> laserH;
    getVerticals(laser, laserV);
    getHorizontals(laser, laserH);

    long long total = 0;
    for (int d = 0; d < 4; d++) {
        vector<Point> barn = getPath(Bx, By, d);
        vector<VSeg> barnV;
        vector<HSeg> barnH;
        getVerticals(barn, barnV);
        getHorizontals(barn, barnH);

        // 激光光路与谷仓光路的交点：竖直段配水平段，两个方向都要统计
        long long res = countInts(laserV, barnH) + countInts(barnV, laserH);

        // 若从谷仓射出的光绕了一圈又回到谷仓，则这条光路是个环：
        // 环上的点往环的两个方向走都能回到谷仓，却会分别从相反的两个出发方向被扫到，
        // 所以这里只记 1 份；其余情况同一个位置在四个方向里只会出现一次，记 2 份。
        bool isCycle = (barn.front().x == barn.back().x && barn.front().y == barn.back().y);
        if (isCycle) total += res;
        else total += 2 * res;
    }
    // 归一化：每个可行位置在上面一共被计了 2 份
    printf("%lld\n", total / 2);
    return 0;
}
