#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n, q;
char color[MAXN + 5];
int prefixSol[MAXN + 5];  // prefixSol[x]：只涂前 x 段所需的最少笔画数
int suffixSol[MAXN + 5];  // suffixSol[x]：只涂后 x 段所需的最少笔画数

// "未结束的笔画"栈：自底向上颜色严格递增（浅色在下、深色在上）
char stk[30];

// dir = 1 从左往右求所有前缀的答案，dir = -1 从右往左求所有后缀的答案
// （后缀的答案等于把字符串反过来求前缀，所以只需把扫描方向换一下）
void build(int sol[], int dir) {
    int top = 0;
    for (int t = 0; t < n; ++t) {
        int i = (dir == 1) ? t : n - 1 - t;
        sol[t + 1] = sol[t];
        // 比当前颜色深的笔画必须结束，否则它盖住了当前格子
        while (top > 0 && stk[top - 1] > color[i]) --top;
        // 栈顶正好是当前颜色时，沿用那笔画笔把它延伸过来，不新增笔画
        if (top == 0 || stk[top - 1] < color[i]) {
            stk[top++] = color[i];
            ++sol[t + 1];
        }
    }
}

int main() {
    scanf("%d%d", &n, &q);
    scanf("%s", color);
    build(prefixSol, 1);
    build(suffixSol, -1);
    for (int i = 0; i < q; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        // 留下一段不涂，等价于分别涂好左边的前缀 a-1 段和右边的后缀 n-b 段
        printf("%d\n", prefixSol[a - 1] + suffixSol[n - b]);
    }
    return 0;
}
