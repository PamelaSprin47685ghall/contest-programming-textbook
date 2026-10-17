#include <cstdio>

using namespace std;

const int MAXN = 100005;

// 建树: i 的父亲是 j_i + 1(于是 N+1 号点是根), 记 h_i 为"最深的点的高度减去 i 的高度".
// 数组取 x_i = h_i * K + t_i: 因为 h_{父亲} = h_i + 1, 所以 x_{j_i+1} > x_i + K 等价于
// t_{j_i+1} > t_i; 而所有高度为 h_i + 1 且下标不超过 j_i 的点都要落在 x_i + K 以内.
// 按"先访问编号大的儿子"的 DFS 顺序把 t 从 N 递减着分配, 恰好满足这两条要求, 取 K = N + 1.

int N;
int jv[MAXN];
int head[MAXN + 2];  // 儿子链表, 头插法保证遍历时编号递减
int nxtChild[MAXN];
int depth[MAXN + 2];
int offset[MAXN + 2];
int stk[MAXN + 2];

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; ++i) scanf("%d", &jv[i]);
    for (int v = 1; v <= N + 1; ++v) head[v] = 0;
    for (int i = 1; i <= N; ++i) {
        int p = jv[i] + 1;
        nxtChild[i] = head[p];
        head[p] = i;
    }
    // 显式栈模拟先序遍历, 避免链状树时递归过深
    int counter = N;
    offset[N + 1] = counter;
    --counter;
    depth[N + 1] = 0;
    int top = 0;
    stk[top++] = N + 1;
    int maxDepth = 0;
    while (top > 0) {
        int a = stk[top - 1];
        int b = head[a];
        if (b == 0) {
            --top;
            continue;
        }
        head[a] = nxtChild[b];  // 该儿子已访问, 从链表里摘掉
        depth[b] = depth[a] + 1;
        offset[b] = counter;
        --counter;
        if (depth[b] > maxDepth) maxDepth = depth[b];
        stk[top++] = b;
    }
    long long K = N + 1;
    printf("%lld\n", K);
    for (int i = 1; i <= N; ++i) {
        long long height = maxDepth - depth[i];
        printf("%lld\n", height * K + offset[i]);
    }
    return 0;
}
