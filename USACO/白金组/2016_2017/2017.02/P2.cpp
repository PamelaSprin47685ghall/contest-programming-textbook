#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000; // 字段数上限

int n;
int sideA[MAXN + 1]; // 一侧字段的品种顺序
int posB[MAXN + 1];  // 每个品种在另一侧的位置
int bestAt[MAXN + 1]; // 前缀最大值树状数组：以某个另一侧位置为终点的最长可行链

void raiseMax(int x, int v) {
    for (; x <= n; x += x & -x) {
        if (bestAt[x] < v) bestAt[x] = v;
    }
}

int prefixMax(int x) {
    int res = 0;
    for (; x > 0; x -= x & -x) {
        if (bestAt[x] > res) res = bestAt[x];
    }
    return res;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", &sideA[i]);
    for (int i = 1; i <= n; ++i) {
        int breed;
        scanf("%d", &breed);
        posB[breed] = i;
    }

    int cand[9]; // |a-b| <= 4 最多给出 9 个可选品种
    for (int i = 1; i <= n; ++i) {
        int a = sideA[i];
        // 把一头牛和另一侧牛配对看作平面上的点（A 侧位置, B 侧位置），
        // 一组互不相交的连线恰好是一条横纵坐标都严格递增的链，取最长链即可。
        int lo = a - 4 < 1 ? 1 : a - 4;
        int hi = a + 4 > n ? n : a + 4;
        int cnt = 0;
        for (int x = lo; x <= hi; ++x) cand[cnt++] = posB[x];
        sort(cand, cand + cnt);
        // 同一头牛不能连出两条线，而查询只用到严格更小的位置，
        // 所以按位置从大到小更新：本轮已更新的更大位置不会被本轮查询看到。
        for (int idx = cnt - 1; idx >= 0; --idx) {
            int v = prefixMax(cand[idx] - 1) + 1;
            raiseMax(cand[idx], v);
        }
    }
    printf("%d\n", prefixMax(n));
    return 0;
}
