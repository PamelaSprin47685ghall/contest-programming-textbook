#include <cstdio>
#include <algorithm>
#include <unordered_map>

using namespace std;

const long long MOD = 1000000007LL;
const long long HALF = (MOD + 1) / 2;         // 模意义下的 1/2，用来避免每次做除法
const long long LAST_DAY = 200000000000000LL; // 安全上界：最大投喂日 1e14 加总捆数 1e14
const long long INF = 4000000000000000000LL;
const int MAXQ = 100005;
const int MAXNODE = 10000000; // 持久化线段树的节点池（约 400MB）

// 持久化线段树：叶子是若干连续天数组成的区间。因为"吃"总是从最左的空天开始，
// 所以一个叶子内被吃掉的天数一定是它的一个前缀，只需用 (sum, rem) 描述。
struct Node {
    long long from, to; // 覆盖的天数区间
    long long sum;      // 区间内被吃掉的天数之和（先乘 2 存着，最后乘 1/2）
    long long rem;      // 区间内还没被吃掉的天数
    int lc, rc;         // 左右儿子下标，-1 表示叶子
};

Node pool[MAXNODE];
int poolTop = 0;

int newNode(long long from, long long to, long long sum, long long rem, int lc, int rc) {
    Node &nd = pool[poolTop];
    nd.from = from;
    nd.to = to;
    nd.sum = sum;
    nd.rem = rem;
    nd.lc = lc;
    nd.rc = rc;
    return poolTop++;
}

// rem 个空天：被吃掉的是前缀 [from, usedUpTo]，等差数列求和
int leafNode(long long from, long long to, long long rem) {
    long long usedUpTo = to - rem;
    long long cnt = (usedUpTo - from + 1) % MOD;
    long long tot = (from + usedUpTo) % MOD;
    return newNode(from, to, cnt * tot % MOD, rem, -1, -1);
}

int emptyLeaf(long long from, long long to) { return leafNode(from, to, to - from + 1); }
int fullLeaf(long long from, long long to) { return leafNode(from, to, 0); }

int combineAt(int L, int R) {
    return newNode(pool[L].from, pool[R].to, (pool[L].sum + pool[R].sum) % MOD,
                   pool[L].rem + pool[R].rem, L, R);
}

int Q;
long long qday[MAXQ], qamt[MAXQ];
int uniqCnt = 0;
long long uniqDay[MAXQ];

// 天数区间 [uniqDay[i], uniqDay[i+1]-1]（最后一段直到 LAST_DAY）作为叶子建树
int build(int i, int j) {
    if (i == j) {
        long long from = uniqDay[i];
        long long to = (i + 1 < uniqCnt) ? uniqDay[i + 1] - 1 : LAST_DAY;
        return emptyLeaf(from, to);
    }
    int mid = (i + j) / 2;
    return combineAt(build(i, mid), build(mid + 1, j));
}

// 从第 start 天起增加 amt 捆草：吃掉从 start 开始最靠左的 amt 个空天。
// 分三种情况剪枝，保证一次更新只需要 O(log) 个新节点。
int update(int node, long long start, long long amt) {
    if (start > pool[node].to || amt == 0) return node;
    if (start <= pool[node].from && amt >= pool[node].rem) {
        return fullLeaf(pool[node].from, pool[node].to);
    }
    if (pool[node].lc == -1) {
        long long rem = pool[node].rem - amt;
        if (rem < 0) rem = 0;
        return leafNode(pool[node].from, pool[node].to, rem);
    }
    int nl = update(pool[node].lc, start, amt);
    // 左儿子装不下（已经吃满）时，剩下的草继续去右儿子找空天
    long long still = amt - (pool[pool[node].lc].rem - pool[nl].rem);
    int nr = update(pool[node].rc, start, still);
    return combineAt(nl, nr);
}

// cur[day] = 当前时刻该天的草量 + 1（0 表示这道天还没被任何更新提到，即 0 捆）
unordered_map<long long, long long> cur;

// 对询问区间 [from, to] 分治。
// 不变式：进入本层时，root 这棵树里每一天 d 的值恰好是
//   min{ b(d, t) : t 属于 [from-1, to] }，
// 其中 b(d, t) 是做完第 t 次更新后第 d 天的草量。到单点时这个最小值就是 b(d, from)，
// 也就是那一刻的真实状态，可以直接输出答案。
void solve(int from, int to, int root) {
    int savedTop = poolTop;
    if (from == to) {
        long long inCur = 0;
        unordered_map<long long, long long>::iterator it = cur.find(qday[from]);
        if (it != cur.end()) inCur = it->second - 1;
        if (qamt[from] > inCur) {
            // 单点状态里该天的值已被压到 min(旧值, 新值)，这里补足到新值
            root = update(root, qday[from], qamt[from] - inCur);
        }
        printf("%lld\n", HALF * pool[root].sum % MOD);
        cur[qday[from]] = qamt[from] + 1;
    } else {
        int mid = (from + to) / 2;
        // 左半段：每天出现过的草量最小值、最后一次出现的草量
        unordered_map<long long, long long> minLeft, finalLeft;
        // 右半段：每天出现过的草量最小值
        unordered_map<long long, long long> minRight;
        minLeft.reserve(mid - from + 2);
        finalLeft.reserve(mid - from + 2);
        minRight.reserve(to - mid + 1);
        for (int j = from; j <= mid; j++) {
            long long v = qamt[j] + 1;
            unordered_map<long long, long long>::iterator it = minLeft.find(qday[j]);
            if (it == minLeft.end()) {
                minLeft[qday[j]] = v;
            } else if (v < it->second) {
                it->second = v;
            }
            finalLeft[qday[j]] = v;
        }
        for (int j = mid + 1; j <= to; j++) {
            long long v = qamt[j] + 1;
            unordered_map<long long, long long>::iterator it = minRight.find(qday[j]);
            if (it == minRight.end()) {
                minRight[qday[j]] = v;
            } else if (v < it->second) {
                it->second = v;
            }
        }

        // 左子树需要的状态：把"只被右半段改动过"的那些天抬到 min(左半段之前的值, 左半段最小值)，
        // 也就是 [from-1, mid] 这一段的最小值。
        int leftRoot = root;
        for (unordered_map<long long, long long>::iterator it = minRight.begin();
             it != minRight.end(); ++it) {
            long long day = it->first;
            long long rv = it->second;
            long long base = 1;
            unordered_map<long long, long long>::iterator itc = cur.find(day);
            if (itc != cur.end()) base = itc->second;
            long long lv = INF;
            unordered_map<long long, long long>::iterator itl = minLeft.find(day);
            if (itl != minLeft.end()) lv = itl->second;
            long long next = base < lv ? base : lv;
            if (next > rv) leftRoot = update(leftRoot, day, next - rv);
        }

        // 右子树需要的状态：对"被左半段改动过"的天，把它从 [from-1, mid] 的最小值
        // 抬到 [mid, to] 的最小值（mid 时刻的值与右半段最小值取小）。
        int rightRoot = root;
        for (unordered_map<long long, long long>::iterator it = minLeft.begin();
             it != minLeft.end(); ++it) {
            long long day = it->first;
            long long lv = it->second;
            long long oldBase = 1;
            unordered_map<long long, long long>::iterator itc = cur.find(day);
            if (itc != cur.end()) oldBase = itc->second;
            long long prevHad = oldBase < lv ? oldBase : lv;
            long long base = finalLeft[day];
            long long rv = INF;
            unordered_map<long long, long long>::iterator itr = minRight.find(day);
            if (itr != minRight.end()) rv = itr->second;
            long long next = base < rv ? base : rv;
            if (next > prevHad) rightRoot = update(rightRoot, day, next - prevHad);
        }

        solve(from, mid, leftRoot);
        solve(mid + 1, to, rightRoot);
    }
    // 回滚本层申请的所有节点：本层的两个分支都已经走完，这些版本不再被需要
    poolTop = savedTop;
}

int main() {
    scanf("%d", &Q);
    for (int i = 0; i < Q; i++) {
        scanf("%lld %lld", &qday[i], &qamt[i]);
        uniqDay[i] = qday[i];
    }
    sort(uniqDay, uniqDay + Q);
    uniqCnt = (int)(unique(uniqDay, uniqDay + Q) - uniqDay);

    cur.reserve(2 * Q + 10);
    int root = build(0, uniqCnt - 1);
    solve(0, Q - 1, root);
    return 0;
}
