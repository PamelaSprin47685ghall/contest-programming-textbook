#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 505;

int n;
int given[MAXN];        // FJ 写下的 N+1 个数
int deg[MAXN];          // 去掉一个数之后剩下的 N 个度数 (从大到小排序)
long long prefix[MAXN]; // deg 的前缀和

bool CmpDesc(int a, int b) {
    return a > b;
}

// 用 Erdos-Gallai 定理判断 deg[1..n] 能否成为一个简单图的度数序列:
// 总和为偶数, 且对每个 k 都有 sum_{i<=k} d_i <= k(k-1) + sum_{i>k} min(d_i, k).
bool Graphical() {
    if (prefix[n] % 2 != 0) return false;
    // 因为 deg 递减, 满足 deg[i] <= k 的位置构成一个后缀, 随着 k 增大只会变长
    int idx = n + 1;
    for (int k = 1; k <= n; k++) {
        while (idx - 1 >= 1 && deg[idx - 1] <= k) idx--;
        int la = max(idx, k + 1); // 从这里开始, min(d_i, k) = d_i
        // 位置 k+1..la-1 都大于 k, 每个贡献 k; 位置 la..n 直接贡献自己的度数
        long long tail = (long long)k * (la - k - 1) + (prefix[n] - prefix[la - 1]);
        if (prefix[k] > (long long)k * (k - 1) + tail) return false;
    }
    return true;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n + 1; i++) scanf("%d", &given[i]);

    int answer[MAXN];
    int cnt = 0;
    for (int skip = 1; skip <= n + 1; skip++) {
        int p = 0;
        for (int i = 1; i <= n + 1; i++) {
            if (i != skip) deg[++p] = given[i];
        }
        sort(deg + 1, deg + n + 1, CmpDesc);
        prefix[0] = 0;
        for (int i = 1; i <= n; i++) prefix[i] = prefix[i - 1] + deg[i];
        if (Graphical()) answer[++cnt] = skip;
    }
    printf("%d\n", cnt);
    for (int i = 1; i <= cnt; i++) printf("%d\n", answer[i]);
    return 0;
}
