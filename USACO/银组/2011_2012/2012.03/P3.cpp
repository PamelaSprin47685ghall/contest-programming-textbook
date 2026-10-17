#include <cstdio>

using namespace std;

// 每个花坛的土量不超过 10, 花坛不超过 100 个, 所以泥土总单位数不超过 1000.
const int MAXUNITS = 1000;

int sourcePos[MAXUNITS + 1];    // 原来第 i 单位泥土所在的(0 起的)花坛编号
int targetPos[MAXUNITS + 1];    // 目标里第 j 单位泥土所在的花坛编号
int sourceTotal, targetTotal;
// best[i][j]: 把前 i 单位原泥土变成前 j 单位目标泥土的最小花费
int best[MAXUNITS + 1][MAXUNITS + 1];

int main() {
    int n = 0, buyCost = 0, removeCost = 0, moveCost = 0;
    scanf("%d %d %d %d", &n, &buyCost, &removeCost, &moveCost);

    // 把"每个花坛有几单位土"摊平成"每单位土在哪个花坛", 得到两条非递减的序列.
    // 于是"搬土"就变成两个序列之间的配对, 可以套编辑距离的 DP.
    for (int i = 0; i < n; ++i) {
        int have = 0, want = 0;
        scanf("%d %d", &have, &want);
        for (int k = 0; k < have; ++k) sourcePos[++sourceTotal] = i;
        for (int k = 0; k < want; ++k) targetPos[++targetTotal] = i;
    }

    // 边界: 一边为空时只能全部买土或全部运走.
    for (int j = 0; j <= targetTotal; ++j) best[0][j] = j * buyCost;
    for (int i = 0; i <= sourceTotal; ++i) best[i][0] = i * removeCost;

    for (int i = 1; i <= sourceTotal; ++i)
        for (int j = 1; j <= targetTotal; ++j) {
            // 目标里多出的这一单位土只能买来放在第 j 个单位该在的花坛.
            int value = best[i][j - 1] + buyCost;
            // 原第 i 单位土用不上, 只能运走.
            if (best[i - 1][j] + removeCost < value) value = best[i - 1][j] + removeCost;
            // 让原第 i 单位土去填目标第 j 单位: 距离越远, 搬运费越高.
            // 两条序列都非递减, 交叉配对一定不如就近配对, 所以只需考虑这种一一对应.
            int diff = sourcePos[i] - targetPos[j];
            if (diff < 0) diff = -diff;
            if (best[i - 1][j - 1] + moveCost * diff < value)
                value = best[i - 1][j - 1] + moveCost * diff;
            best[i][j] = value;
        }

    printf("%d\n", best[sourceTotal][targetTotal]);
    return 0;
}
