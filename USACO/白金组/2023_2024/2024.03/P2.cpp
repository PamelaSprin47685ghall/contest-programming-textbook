#include <cstdio>
#include <map>
#include <algorithm>

using namespace std;

// 记 d 为 Bessie 比 Elsie 多的干草数。处理第 i 个干草堆时：
//   d <= 0 就把 a_i 给 Bessie，d += a_i；d > 0 就给 Elsie，d -= a_i。
// 询问给出 d 的初值 x，问处理完 a_l..a_r 后 d 是多少。
//
// 做法：把询问按左端点挂链离线，从左往右扫描 a，同时维护所有"已经插入还没回答"的
// 询问当前的值。为了能用懒标记整体更新，把数值做变换 t = 2d - 1、a_i *= 2：
// 变换后所有值都是奇数（永远不等于 0），更新规则变成"t > 0 就减 a_i，t < 0 就加 a_i"，
// 最后把 t 还原回 d = (t + 1) / 2 即可。
//
// 处理完 a_i 之后，所有值按 |t| 与 a_i 的大小关系分成 4 类：
//   posSmall: 0 < t < a_i    posBig: t > a_i
//   negSmall: -a_i < t < 0   negBig: t < -a_i
// 每一类用一个"懒标记有序集合"维护：mp 里存相对值（实际值 - lazy），整体加减只需改
// lazy，因此 O(1)。注意 a 单调不增，类别之间的搬动只发生在相邻两类之间：
//   a 由 a_{i-1} 变成 a_i 时，small 里绝对值超过 a_i 的极端值要升格成 big；
//   而执行完这次更新后，big 里绝对值小于 a_i 的极端值又要降格回 small。
// 两类 small 的更新方向恰好相反（一个 +a_i 一个 -a_i），所以只要各自改好 lazy 再交换
// 两个集合的指针，就一次性完成了所有 small 的更新。
//
// 复杂度：设 M 为元素在 small 与 big 之间搬动的总次数。第 i 轮里从 small 搬进 big 的
// 互不相同的值都落在长度只有 a_{i-1} - a_i 的开区间内（插入的询问总共只有 Q 个），
// 所以 M = O(a_1 + Q)，总时间 O((N + Q + a_1) log Q)。
// 为了达到这个界，取值相同的询问必须合并：它们此后每一刻的值都相同，用并查集把它们
// 并成一个结点，集合里每个不同的值只保留一个代表。

const int MAXN = 200005;
const int MAXQ = 200005;

int N, Q;
long long A[MAXN];      // A[i] = 2 * a_i
long long initX[MAXQ];  // 询问初值变换后 2 * x - 1
long long ans[MAXQ];

// 询问按左端点 / 右端点挂链
int headL[MAXN], nxtL[MAXQ];
int headR[MAXN], nxtR[MAXQ];

// 并查集：值相同的询问合并成一个代表，dsuPar 为负表示根（绝对值为集合大小）
int dsuPar[MAXQ];
long long dsuVal[MAXQ];     // 代表在所属集合里的相对值（实际值 = 相对值 + lazy）

int Find(int v) {
    int r = v;
    while (dsuPar[r] >= 0) r = dsuPar[r];
    while (v != r) {        // 路径压缩
        int t = dsuPar[v];
        dsuPar[v] = r;
        v = t;
    }
    return r;
}

// 支持整体加法的有序集合（上面的 4 个类别各一个）
struct LazySet {
    map<long long, int> mp; // 相对值 -> 代表结点
    long long lazy;
    LazySet() : lazy(0) {}
};

LazySet *posSmall, *negSmall, *posBig, *negBig;
LazySet *belong[MAXQ];      // 每个代表当前所在的集合

// 把当前实际值为 val 的代表 rep 插入集合 s；若集合里已有同值的代表就并起来
void Add(LazySet *s, long long val, int rep) {
    long long key = val - s->lazy;
    map<long long, int>::iterator it = s->mp.find(key);
    if (it == s->mp.end()) {
        s->mp[key] = rep;
        dsuVal[rep] = key;
        belong[rep] = s;
        return;
    }
    int other = Find(it->second), self = Find(rep);
    if (other == self) return;
    if (dsuPar[other] > dsuPar[self]) { // 保证 other 是较大的集合
        int t = other;
        other = self;
        self = t;
    }
    dsuPar[other] += dsuPar[self];
    dsuPar[self] = other;
    dsuVal[other] = key;
    belong[other] = s;
    s->mp[key] = other;
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        long long v;
        scanf("%lld", &v);
        A[i] = 2 * v;
    }
    scanf("%d", &Q);
    for (int q = 1; q <= Q; q++) {
        int l, r;
        long long x;
        scanf("%d%d%lld", &l, &r, &x);
        initX[q] = 2 * x - 1;
        dsuPar[q] = -1;
        dsuVal[q] = initX[q];
        nxtL[q] = headL[l];
        headL[l] = q;
        nxtR[q] = headR[r];
        headR[r] = q;
    }

    posSmall = new LazySet();
    negSmall = new LazySet();
    posBig = new LazySet();
    negBig = new LazySet();

    for (int i = 1; i <= N; i++) {
        // 1. 左端点为 i 的询问现在才进入：它的初值就是此时的值，先归入 small
        for (int q = headL[i]; q; q = nxtL[q]) {
            int rep = Find(q);
            if (initX[q] > 0) Add(posSmall, initX[q], rep);
            else Add(negSmall, initX[q], rep);
        }

        // 2. a 变小到 A[i]，small 里绝对值超过 A[i] 的极端值升格为 big
        while (!posSmall->mp.empty()) {
            map<long long, int>::iterator it = posSmall->mp.end();
            --it;
            long long v = it->first + posSmall->lazy;
            if (v <= A[i]) break;
            int rep = it->second;
            posSmall->mp.erase(it);
            Add(posBig, v, rep);
        }
        while (!negSmall->mp.empty()) {
            map<long long, int>::iterator it = negSmall->mp.begin();
            long long v = it->first + negSmall->lazy;
            if (v >= -A[i]) break;
            int rep = it->second;
            negSmall->mp.erase(it);
            Add(negBig, v, rep);
        }

        // 3. 执行第 i 个干草堆：small 必定换号，两类方向相反，交换集合并改懒标记即可
        posSmall->lazy -= A[i];
        negSmall->lazy += A[i];
        swap(posSmall, negSmall);
        // big 里的值保持原来的符号，只是整体靠近 / 远离 0
        posBig->lazy -= A[i];
        negBig->lazy += A[i];

        // 4. 绝对值小于 A[i] 的 big 降格回 small
        while (!negBig->mp.empty()) {
            map<long long, int>::iterator it = negBig->mp.end();
            --it;
            long long v = it->first + negBig->lazy;
            if (v <= -A[i]) break;
            int rep = it->second;
            negBig->mp.erase(it);
            Add(negSmall, v, rep);
        }
        while (!posBig->mp.empty()) {
            map<long long, int>::iterator it = posBig->mp.begin();
            long long v = it->first + posBig->lazy;
            if (v >= A[i]) break;
            int rep = it->second;
            posBig->mp.erase(it);
            Add(posSmall, v, rep);
        }

        // 5. 右端点为 i 的询问到此处理完
        for (int q = headR[i]; q; q = nxtR[q]) {
            int rep = Find(q);
            ans[q] = belong[rep]->lazy + dsuVal[rep];
        }
    }

    for (int q = 1; q <= Q; q++) printf("%lld\n", (ans[q] + 1) / 2);
    return 0;
}
