#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

int n;
long long A[MAXN];

// 等差数列求和 l + (l+1) + ... + r
long long arith(long long l, long long r) { return (l + r) * (r - l + 1) / 2; }

// Bessie 自己堆里有 B 个数时，能否保证最终数组有序（时间取 B 的最小可行值）。
// 关键结论：只要 B 可行，B+1 也一定可行，所以可以二分最小可行 B。
bool possible(long long B) {
    if (B == n) return true;            // 全部自己排序，必定可行
    long long numBefore = 0;            // 会被排在 a[i]（助手加入）之前的 Bessie 元素个数
    for (int i = 0; i + 1 < n; ++i) {
        // Bessie 第 k 次取出元素的时间是 f(k) = B + (B-1) + ... + (B-k+1)；
        // arith(B - numBefore, B) 恰好等于 f(numBefore + 1)，同刻时 Bessie 先加入，所以取等号。
        while (numBefore < B && arith(B - numBefore, B) <= A[i]) ++numBefore;
        // a[i] 若交给助手，它在结果序列中的名次必须正好等于它在助手堆里的名次，
        // 也就是 (i + 1) - numBefore；只要存在某个下标的名次等于 N - B，就能选出完整助手堆。
        if ((long long)(i + 1) - numBefore == (long long)n - B) return true;
    }
    return false;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) scanf("%lld", &A[i]);
        sort(A, A + n);

        long long lo = 1, hi = n;
        while (lo < hi) {
            long long mid = (lo + hi) / 2;
            if (possible(mid)) hi = mid;
            else lo = mid + 1;
        }
        // 自己排完最后（最大）一个元素的时间是 1+2+...+lo；
        // 另一种极端是全部交给助手，耗时就是最大值 A[n-1]。
        long long ans = arith(1, lo);
        if (A[n - 1] < ans) ans = A[n - 1];
        printf("%lld\n", ans);
    }
    return 0;
}
