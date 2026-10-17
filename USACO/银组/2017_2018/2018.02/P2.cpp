#include <cstdio>

using namespace std;

const int MAXN = 255;

int N, B;
int snow[MAXN];
int limitSnow[MAXN];  // 第 b 双鞋能踩的雪深上限 s_b
int maxStep[MAXN];    // 第 b 双鞋一步最远能迈的距离 d_b
bool reach[MAXN][MAXN];  // reach[i][b]: 站在第 i 格、脚上是第 b 双鞋的状态是否可达

int main() {
    scanf("%d%d", &N, &B);
    for (int i = 0; i < N; i++) scanf("%d", &snow[i]);
    for (int b = 0; b < B; b++) scanf("%d%d", &limitSnow[b], &maxStep[b]);

    // 起点没有雪, FJ 可以先扔掉任意多双鞋再穿上第 b 双, 所以 (0, b) 全部可达.
    for (int b = 0; b < B; b++) reach[0][b] = true;

    // 状态转移只会让"格号"或"鞋号"变大, 故按 (鞋号, 格号) 递增顺序一遍扫描即可推完所有状态.
    for (int b = 0; b < B; b++) {
        for (int i = 0; i < N; i++) {
            if (!reach[i][b]) continue;

            // 穿着第 b 双鞋向前迈步: 只要求落脚的那一格雪不深于 s_b.
            for (int j = i + 1; j < N && j - i <= maxStep[b]; j++) {
                if (snow[j] <= limitSnow[b]) reach[j][b] = true;
            }

            // 在第 i 格换鞋: 把第 b+1 到第 b2-1 双直接扔掉, 再穿上第 b2 双.
            // 换鞋时脚上原有的第 b 双也必须能踩住当前雪深, 而该状态可达本身就保证了这点.
            for (int b2 = b + 1; b2 < B; b2++) {
                if (snow[i] <= limitSnow[b2]) reach[i][b2] = true;
            }
        }
    }

    // 终点也没有雪, 到达时脚上是第 b 双鞋就说明扔掉了前 b 双.
    for (int b = 0; b < B; b++) {
        if (reach[N - 1][b]) {
            printf("%d\n", b);
            break;
        }
    }
    return 0;
}
