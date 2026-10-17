#include <cstdio>

using namespace std;

const int MAXN = 50005;

int entryPoint[MAXN];        // 每头牛第一次出现的位置, 还没出现记 -1
int tree[2 * MAXN + 5];      // 树状数组: 位置上有 1 表示有一头牛已进场但还没出场

int pointCount;

void add(int pos, int delta) {
    for (int i = pos + 1; i <= pointCount; i += i & -i) {
        tree[i] += delta;
    }
}

// 统计位置 [0, pos] 上仍留在场内的牛数
int query(int pos) {
    int res = 0;
    for (int i = pos + 1; i > 0; i -= i & -i) {
        res += tree[i];
    }
    return res;
}

int main() {
    int n;
    scanf("%d", &n);
    pointCount = 2 * n;
    for (int i = 1; i <= n; ++i) {
        entryPoint[i] = -1;
    }

    long long answer = 0;
    for (int pos = 0; pos < pointCount; ++pos) {
        int cow;
        scanf("%d", &cow);
        if (entryPoint[cow] == -1) {
            entryPoint[cow] = pos;
            add(pos, 1);  // 这头牛从 pos 进场, 开始计入"场内"
            continue;
        }
        int entry = entryPoint[cow];
        // 此刻还在场内的牛, 若入口落在 (entry, pos) 之间, 它必定比本牛晚出场,
        // 于是四个点的顺序是 entry < 对方入口 < pos(本牛出口) < 对方出口,
        // 正是 ABAB 的交错形状, 必与 (cow, 对方) 构成 crossing 对;
        // 入口在 entry 之前的是包含关系(AABB/BBAA), 不会交叉, 故不计入.
        // 每对牛只在先出场的那头牛处被数一次, 不重不漏.
        answer += query(pos - 1) - query(entry);
        add(entry, -1);  // 本牛离场
    }
    printf("%lld\n", answer);
    return 0;
}
