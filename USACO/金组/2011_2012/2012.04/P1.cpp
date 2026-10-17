#include <cstdio>

using namespace std;

const int MAXN = 10;     // 柱子数上限: 1 <= N <= 10
const int MAXP = 10005;  // 绳子折点数上限: M <= 10000, 折点共 M + 1 个

int n, m;
long long postX;               // 所有柱子共用的 x 坐标
long long postY[MAXN];         // 柱子的 y 坐标
long long rx[MAXP], ry[MAXP];  // 绳子的折点
int cut[MAXP];                 // 绳子每次穿过柱子所在直线的位置编码
int nCut;
int grp[MAXN + 1];             // grp[k]: 编号小于 k 的柱子中还剩下几根
int stk[MAXP];                 // 判定用的手写栈

// 向量 (b - a) 与 (c - a) 的叉积; 符号表示点 c 在直线 ab 的哪一侧
long long cross(long long ax, long long ay, long long bx, long long by, long long cx, long long cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

int main() {
    long long bx, by;
    scanf("%d %d %lld %lld", &n, &m, &bx, &by);
    for (int i = 0; i < n; i++) {
        long long x, y;
        scanf("%lld %lld", &x, &y);
        postX = x;  // 所有柱子 x 相同, 记最后一个就行
        postY[i] = y;
    }
    for (int i = 0; i <= m; i++) {
        scanf("%lld %lld", &rx[i], &ry[i]);
    }

    // 只保留穿过柱子所在直线的绳子段. 不穿过直线的部分可以随便拉直, 与答案无关.
    // 每段的"编码" = 交点下方(按某种统一朝向)有几根柱子, 也就是交点落在第几个空隙里.
    nCut = 0;
    for (int i = 0; i < m; i++) {
        if ((rx[i] > postX) ^ (rx[i + 1] > postX)) {
            int below = 0;
            for (int j = 0; j < n; j++) {
                if (cross(rx[i], ry[i], rx[i + 1], ry[i + 1], postX, postY[j]) > 0) below++;
            }
            // 从右往左穿时朝向翻了过来, 用 N - below 换回统一的"从下往上数"
            if (rx[i + 1] > postX) below = n - below;
            cut[nCut++] = below;
        }
    }

    int ans = n;  // 全部锯掉肯定能逃脱, 作为上界
    // 枚举保留哪些柱子; 2^N <= 1024, 足够直接搜
    for (int keep = 0; keep < (1 << n); keep++) {
        int kept = 0;
        grp[0] = 0;
        for (int i = 0; i < n; i++) {
            kept += (keep >> i) & 1;
            grp[i + 1] = kept;
        }
        if (n - kept >= ans) continue;  // 剪枝: 锯掉的根数已经不少于当前最优

        // 把编码换成"保留柱子的空隙编号"后, 相邻且相同的两个编码表示一个
        // 没有套住任何柱子的死圈, 可以拉直消掉, 等价于用栈配对消除.
        int top = 0;
        for (int i = 0; i < nCut; i++) {
            int g = grp[cut[i]];
            if (top > 0 && stk[top - 1] == g) top--;
            else stk[top++] = g;
        }
        if (top == 0) ans = n - kept;  // 全部消掉, 说明贝茜能跑掉
    }
    printf("%d\n", ans);
    return 0;
}
