#include <cstdio>

using namespace std;

// N 最大 1000, 整个网格只有 10^6 个格子, 直接一遍扫完就行.
const int MAXN = 1000 + 5;

int n;
char line[MAXN];
int firstRow[MAXN];   // 第一行: L 记 1, R 记 0, 当作"基准图案"
int rowBad[MAXN];     // 第 i 行 (i>=1) 在 1..n-1 列上与基准图案不符的格子数
int colBad[MAXN];     // 第 j 列 (j>=1) 在 1..n-1 行上与基准图案不符的格子数
int badTotal;         // 内部区域 (第 1..n-1 行, 第 1..n-1 列) 里不符的格子总数
int lastBadR;         // 内部不符格子的位置 (badTotal==1 时才是唯一的那一个)
int lastBadC;

int main() {
    scanf("%d", &n);
    scanf("%s", line);
    for (int j = 0; j < n; ++j) {
        firstRow[j] = (line[j] == 'L');
    }

    for (int i = 1; i < n; ++i) {
        scanf("%s", line);
        // base: 第 i 行整体相对第一行是否取反. 只看第 0 列:
        // 第 0 列与第一行相同, 这行其余格子就该和第一行相同; 不同则应该相反.
        // 于是"不符"的格子正好是让这一行无法与第一行对齐的那些格子,
        // 也是唯一值得被单独翻转的候选格子.
        int base = (line[0] == 'L') ^ firstRow[0];
        for (int j = 1; j < n; ++j) {
            int bad = ((line[j] == 'L') ^ firstRow[j]) ^ base;
            if (bad) {
                ++rowBad[i];
                ++colBad[j];
                ++badTotal;
                lastBadR = i;
                lastBadC = j;
            }
        }
    }

    int ansR = -1, ansC = -1;
    if (badTotal == (n - 1) * (n - 1)) {
        // 内部每个格子都不符: 翻转 (1,1) 之后第一行整体与每一行的差异量恰好统一,
        // 所有行都对齐, 所以答案是左上角.
        ansR = 0;
        ansC = 0;
    } else if (badTotal == n - 1) {
        // 翻转第一行上的某一格会同时影响所有行, 于是每一行都必须恰好"错在这一列",
        // 不符格子总数只能是 n-1 且集中在同一列. 行号的优先级高于列号, 先找最靠左的整列.
        for (int j = 1; j < n; ++j) {
            if (colBad[j] == n - 1) {
                ansR = 0;
                ansC = j;
                break;
            }
        }
        // 退而求其次: 翻转第 i 行第 1 列只影响第 i 行, 此时要求别的行完全对齐,
        // 而第 i 行在 1..n-1 列上全部不符 (即整行不符).
        if (ansR < 0) {
            for (int i = 1; i < n; ++i) {
                if (rowBad[i] == n - 1) {
                    ansR = i;
                    ansC = 0;
                    break;
                }
            }
        }
    } else if (badTotal == 1) {
        // 翻转内部唯一的那个不符格子, 其余行本来就已对齐.
        ansR = lastBadR;
        ansC = lastBadC;
    }

    if (ansR < 0) {
        printf("-1\n");
    } else {
        printf("%d %d\n", ansR + 1, ansC + 1);
    }
    return 0;
}
