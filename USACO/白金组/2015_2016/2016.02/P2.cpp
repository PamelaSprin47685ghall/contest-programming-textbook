#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXFENCE = 25000; // 竖栅栏、横栅栏各自的数量上限

int gapX[MAXFENCE + 2]; // 先存竖栅栏位置，随后改成相邻竖栅栏之间的距离（列宽）
int gapY[MAXFENCE + 2]; // 先存横栅栏位置，随后改成相邻横栅栏之间的距离（行高）

int main() {
    int fieldA, fieldB, fenceN, fenceM;
    scanf("%d%d%d%d", &fieldA, &fieldB, &fenceN, &fenceM);
    for (int i = 1; i <= fenceN; ++i) scanf("%d", &gapX[i]);
    for (int j = 1; j <= fenceM; ++j) scanf("%d", &gapY[j]);
    // 田地边界当成两条不用拆的"栅栏"，它们只用来算最外侧的行高、列宽
    gapX[0] = 0;
    gapX[fenceN + 1] = fieldA;
    gapY[0] = 0;
    gapY[fenceM + 1] = fieldB;
    sort(gapX, gapX + fenceN + 2);
    sort(gapY, gapY + fenceM + 2);
    // 换成列宽、行高：拆掉一段竖栅栏，就是给同一行里相邻的两间牛圈开口，
    // 那段栅栏的长度恰好等于这一行的行高；横栅栏同理，长度等于这一列的列宽
    for (int i = 0; i <= fenceN; ++i) gapX[i] = gapX[i + 1] - gapX[i];
    for (int j = 0; j <= fenceM; ++j) gapY[j] = gapY[j + 1] - gapY[j];
    sort(gapX, gapX + fenceN + 1);
    sort(gapY, gapY + fenceM + 1);

    // 连通所有牛圈相当于在"牛圈为点、要拆的栅栏段为边"的图上求最小生成树。
    // 最矮的一行（行高 gapY[0]）要拆掉全部 fenceN 段竖栅栏，才能把这行串成一串；
    // 最窄的一列（列宽 gapX[0]）同理要拆掉全部 fenceM 段横栅栏。
    long long removed = (long long)gapX[0] * fenceM + (long long)gapY[0] * fenceN;
    int usedColumn = 1, usedRow = 1; // 已经处理过的列宽、行高个数
    while (usedColumn <= fenceN && usedRow <= fenceM) {
        if (gapX[usedColumn] < gapY[usedRow]) {
            // 这一列上下已经有 usedRow 行连成一片，只需再跟没处理过的行接上
            removed += (long long)gapX[usedColumn] * (fenceM + 1 - usedRow);
            ++usedColumn;
        } else {
            removed += (long long)gapY[usedRow] * (fenceN + 1 - usedColumn);
            ++usedRow;
        }
    }
    printf("%lld\n", removed);
    return 0;
}
