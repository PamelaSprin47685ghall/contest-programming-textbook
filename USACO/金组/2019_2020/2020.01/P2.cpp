#include <cstdio>

using namespace std;

const int MAXN = 5005;
const int MAXQ = 100005;
const int OFF = 2000000;    // A 中元素在 [-1e6, 1e6]，故目标和在 [-2e6, 2e6]，加偏移后可直接作下标
const int SZ = 4000001;

int N, Q;
int A[MAXN];
int cnt[SZ];                // 当前窗口中各值（已加偏移）的出现次数

long long cntCur[MAXN];     // cntCur[k] = ans[i][k]：以 i 为左端点的整行答案
long long cntPrev[MAXN];    // cntPrev[k] = ans[i+1][k]，即上一行的答案

int qHead[MAXN], qNext[MAXQ], qRight[MAXQ];   // 把询问按左端点挂成链表
long long qAns[MAXQ];

int main() {
    scanf("%d %d", &N, &Q);
    for (int i = 1; i <= N; i++) scanf("%d", &A[i]);
    for (int i = 1; i <= N; i++) qHead[i] = -1;
    for (int q = 0; q < Q; q++) {
        int a, b;
        scanf("%d %d", &a, &b);
        qRight[q] = b;
        qNext[q] = qHead[a];
        qHead[a] = q;
    }

    // 行 i 只依赖行 i+1，因此从右往左逐行递推，并且只保留两行
    for (int i = N - 1; i >= 1; i--) {
        // 计算 num[i][k] 与 ans[i][k]（k 递增）；循环中 cnt 恰好保存 A[i+1..k-1] 的计数
        for (int k = i + 1; k <= N; k++) {
            int target = -(A[i] + A[k]) + OFF;
            long long num = cnt[target];
            // 按「最右元素是否为 k、最左元素是否为 i」分类容斥
            cntCur[k] = num + cntCur[k - 1] + cntPrev[k] - cntPrev[k - 1];
            cnt[A[k] + OFF]++;
        }
        for (int k = i + 1; k <= N; k++) cnt[A[k] + OFF]--;

        for (int q = qHead[i]; q != -1; q = qNext[q]) qAns[q] = cntCur[qRight[q]];
        for (int k = 1; k <= N; k++) cntPrev[k] = cntCur[k];
    }

    for (int q = 0; q < Q; q++) printf("%lld\n", qAns[q]);
    return 0;
}
