#include <cstdio>

using namespace std;

const int MAXN = 100005;

int step[MAXN]; // 一次“洗牌 + 左移”合并成的置换（new[i] = old[step[i]]）
int cur[MAXN];  // 快速幂过程中当前的置换
int nxt[MAXN];  // 复合运算的临时数组

// 置换复合：R[i] = A[B[i]]，表示“先作用 B，再作用 A”
void compose(int *R, const int *A, const int *B, int n) {
    for (int i = 0; i < n; i++) {
        R[i] = A[B[i]];
    }
}

int main() {
    int n, m, q;
    scanf("%d %d %d", &n, &m, &q);

    for (int i = 0; i < n; i++) {
        step[i] = i; // 洗牌不动的部分先填成恒等
    }
    for (int i = 0; i < m; i++) {
        int p;
        scanf("%d", &p);
        // 原题给的是“第 i 张牌洗完后到第 p 个位置”，
        // 这里按“位置 p 上的牌来自哪里”来记录，方便直接做置换复合。
        step[p - 1] = i;
    }
    // 拿走顶牌等价于整副牌左移一格，所以把洗牌置换整体左移一位，
    // 就把“洗牌 + 拿走顶牌”打包成了一个固定的置换。
    int first = step[0];
    for (int i = 0; i + 1 < n; i++) {
        step[i] = step[i + 1];
    }
    step[n - 1] = first;

    // 有 M 张以上牌时会洗牌，共 n-m+1 次；剩下的 m-1 次只有左移。
    int times = n - m + 1;

    for (int i = 0; i < n; i++) {
        cur[i] = i; // 从恒等置换（最初的牌序）出发
    }
    // 二进制快速幂：求 step 作用 times 次的结果
    for (int b = 16; b >= 0; b--) {
        compose(nxt, cur, cur, n);
        for (int i = 0; i < n; i++) {
            cur[i] = nxt[i];
        }
        if ((times >> b) & 1) {
            compose(nxt, cur, step, n);
            for (int i = 0; i < n; i++) {
                cur[i] = nxt[i];
            }
        }
    }

    // 打包置换做完后，每张被拿走的牌停在“它被拿起的那一轮”的下标上，
    // 最后堆成的牌堆从上往下正好与下标顺序相反；再把最后 m-1 次左移补回来。
    for (int i = 0; i < q; i++) {
        int x;
        scanf("%d", &x);
        printf("%d\n", cur[(n + m - 1 - x) % n] + 1);
    }
    return 0;
}
