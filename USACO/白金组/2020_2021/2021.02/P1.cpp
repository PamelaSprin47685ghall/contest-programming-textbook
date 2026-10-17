#include <cstdio>

using namespace std;

const int MAXN = 200005;  // 栅栏段数上限
const int MAXQ = 200005;  // 询问数上限

int n, q;
int color[MAXN];  // 每段栅栏要涂的颜色

// 询问按右端点分桶挂成链表：离线从左到右扫右端点时，
// 扫到 b 就能顺手回答所有右端点为 b 的询问
int queryHead[MAXN];
int queryNext[MAXQ];
int queryLeft[MAXQ];
int answer[MAXQ];

// 树状数组统计"可以省下一笔的数对"的左端点个数
int bit[MAXN];

void bitAdd(int i) {
    for (; i <= n; i += i & -i) ++bit[i];
}

int bitSum(int i) {
    int total = 0;
    for (; i > 0; i -= i & -i) total += bit[i];
    return total;
}

int main() {
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; ++i) scanf("%d", &color[i]);
    for (int j = 1; j <= q; ++j) {
        int a, b;
        scanf("%d%d", &a, &b);
        queryLeft[j] = a;
        queryNext[j] = queryHead[b];
        queryHead[b] = j;
    }

    // 单调栈找出所有满足 color[l] == color[r] 且中间颜色都比它深的数对 (l, r)：
    // 这样的数对让 r 可以借着 l 那一笔刷过去，从而少用一笔
    int stackIndex[MAXN];
    int stackTop = 0;
    for (int r = 1; r <= n; ++r) {
        while (stackTop > 0 && color[stackIndex[stackTop]] > color[r]) --stackTop;
        if (stackTop > 0 && color[stackIndex[stackTop]] == color[r]) {
            bitAdd(stackIndex[stackTop]);  // 数对 (左端点, r) 已确定，左端点入树状数组
            stackIndex[stackTop] = r;      // 之后更靠右的同色段应该配对到 r，而不是被它挡住的旧位置
        } else {
            stackIndex[++stackTop] = r;
        }
        // 区间 [l, r] 的答案 = 段数 - 完全落在区间内的可省数对个数。
        // 此时树状数组里存的全是右端点不超过 r 的数对左端点，所以区间和就是所需个数
        for (int j = queryHead[r]; j != 0; j = queryNext[j]) {
            int l = queryLeft[j];
            answer[j] = (r - l + 1) - (bitSum(r) - bitSum(l - 1));
        }
    }

    for (int j = 1; j <= q; ++j) printf("%d\n", answer[j]);
    return 0;
}
