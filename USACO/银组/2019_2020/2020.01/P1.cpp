// USACO 2020 January Contest, Silver P1: Berry Picking
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1000;

int N, K;
int B[MAXN];
int curB; // 当前枚举的 b；排序比较函数不能捕获局部变量，所以放在全局

// 按 B[i] % b 从大到小排序
bool cmpMod(int x, int y) {
    return (x % curB) > (y % curB);
}

int main() {
    scanf("%d %d", &N, &K);
    int maxB = 0;
    for (int i = 0; i < N; i++) {
        scanf("%d", &B[i]);
        if (B[i] > maxB) maxB = B[i];
    }
    int best = 0;
    // 枚举 b = Elsie 所有桶中的最小容量；可假设她的 K/2 个桶恰好都装 b 个
    for (int b = 1; b <= maxB; b++) {
        int full = 0; // 所有树一共能装满几个容量为 b 的桶
        for (int i = 0; i < N; i++) full += B[i] / b;
        if (full < K / 2) break; // floor(B_i/b) 随 b 单调不增，后面的 b 只会更少
        if (full >= K) {
            // 容量 b 的桶足够多，两边各拿 K/2 个，Bessie 不可能更好
            int cand = b * (K / 2);
            if (cand > best) best = cand;
            continue;
        }
        // Elsie 拿走 K/2 个满桶，多出的 full - K/2 个满桶归 Bessie
        curB = b;
        sort(B, B + N, cmpMod);
        int cand = b * (full - K / 2);
        // Bessie 剩下的 K - full 个桶用各树取模后的零头填，优先挑最大的零头
        for (int i = 0; i < N && i + full < K; i++) cand += B[i] % b;
        if (cand > best) best = cand;
    }
    printf("%d\n", best);
    return 0;
}
