#include <cstdio>

using namespace std;

const int MAXN = 200005;

int N;
int S[MAXN], T[MAXN];     // S[i]: 第 i 头牛的座位;  T[i]: 第 i 头牛放行李的秒数

// ---- 隐式 Treap ----
// 中序遍历的顺序就是"位置"从小到大的顺序. 每个结点代表一对 (位置 pos, 时间 tau):
// 含义是"一头想走到位置 pos 以右的牛, 经过 pos 的时刻不能早于 tau".
struct node {
    int l, r;
    int sz;            // 子树大小
    long long pos;     // 位置
    long long tau;     // 时间限制
    int lazyShift;     // 整棵子树的位置同时左移的懒标记
    int pri;           // 随机优先级, 用来维持平衡
};

node tr[MAXN];
int tot;
unsigned int rngState = 123456789u;

// xorshift 伪随机数, 给 Treap 产生优先级
unsigned int nextRand() {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 17;
    rngState ^= rngState << 5;
    return rngState;
}

int newNode(long long p, long long t) {
    tot++;
    tr[tot].l = 0;
    tr[tot].r = 0;
    tr[tot].sz = 1;
    tr[tot].pos = p;
    tr[tot].tau = t;
    tr[tot].lazyShift = 0;
    tr[tot].pri = (int)(nextRand() >> 1);
    return tot;
}

void pushdown(int t) {
    if (t == 0 || tr[t].lazyShift == 0) return;
    int d = tr[t].lazyShift;
    if (tr[t].l) {
        tr[tr[t].l].pos -= d;
        tr[tr[t].l].lazyShift += d;
    }
    if (tr[t].r) {
        tr[tr[t].r].pos -= d;
        tr[tr[t].r].lazyShift += d;
    }
    tr[t].lazyShift = 0;
}

void pull(int t) {
    tr[t].sz = tr[tr[t].l].sz + tr[tr[t].r].sz + 1;
}

// 把 a 与 b 拼接, 保证 a 中所有结点排在 b 前面
int merge(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    if (tr[a].pri < tr[b].pri) {
        pushdown(a);
        tr[a].r = merge(tr[a].r, b);
        pull(a);
        return a;
    } else {
        pushdown(b);
        tr[b].l = merge(a, tr[b].l);
        pull(b);
        return b;
    }
}

// 把 t 的前 k 个结点分给 a, 其余分给 b
void split(int t, int k, int &a, int &b) {
    if (t == 0) {
        a = 0;
        b = 0;
        return;
    }
    pushdown(t);
    if (tr[tr[t].l].sz >= k) {
        split(tr[t].l, k, a, tr[t].l);
        b = t;
        pull(b);
    } else {
        split(tr[t].r, k - tr[tr[t].l].sz - 1, tr[t].r, b);
        a = t;
        pull(a);
    }
}

// 统计 t 中位置严格小于 seat 的结点个数
int countLess(int t, long long seat) {
    if (t == 0) return 0;
    pushdown(t);
    if (tr[t].pos < seat) return tr[tr[t].l].sz + 1 + countLess(tr[t].r, seat);
    return countLess(tr[t].l, seat);
}

int leftmost(int t) {
    while (true) {
        pushdown(t);
        if (tr[t].l == 0) return t;
        t = tr[t].l;
    }
}

int rightmost(int t) {
    while (true) {
        pushdown(t);
        if (tr[t].r == 0) return t;
        t = tr[t].r;
    }
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) scanf("%d %d", &S[i], &T[i]);

    // 队首的限制: 最前面的牛 N 在时刻 0 必须位于位置 0, 即 (0, 0)
    tot = 0;
    int root = newNode(0, 0);

    long long ans = 0;
    // 从最前面(牛 N)依次往后处理, 每头牛到达座位的时刻只受前面这些对子影响
    for (int i = N; i >= 1; i--) {
        long long seat = S[i];
        // 只保留位置 < seat 的对子: 牛 i 会从这些位置右边经过, 受它们的限制
        int cnt = countLess(root, seat);
        int L, R;
        split(root, cnt, L, R);

        // 位置 < seat 的对子说明"牛 i 越过了该位置", 于是对下一位的牛来说
        // 该位置整体左移一格 (牛 i-1 永远比牛 i 靠左一格)
        long long delta = 0;
        if (L) {
            int last = rightmost(L);
            delta = tr[last].tau - tr[last].pos;   // 取最大的 tau - pos
            tr[L].pos -= 1;
            tr[L].lazyShift += 1;
        }

        // 牛 i 到达座位的时刻 = seat + max(tau - pos), 再花 T[i] 秒坐好
        long long finish = delta + seat + T[i];
        if (finish > ans) ans = finish;

        // 牛 i 会一直占着座位直到 finish, 之后的牛要到 finish 时刻才能越过 seat - 1
        int mid = newNode(seat - 1, finish);

        // 位置在 seat 及更右边、时间限制又更松的对子再也不会被用到, 直接删掉,
        // 这样序列里 tau - pos 始终随位置单调不减, 查询才会落在最右端
        long long dnew = finish - (seat - 1);
        while (R) {
            int first = leftmost(R);
            if (tr[first].tau - tr[first].pos < dnew) {
                int a, b;
                split(R, 1, a, b);
                R = b;
            } else {
                break;
            }
        }

        root = merge(merge(L, mid), R);
    }

    printf("%lld\n", ans);
    return 0;
}
