#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

int n, b;
int snowDepth[MAXN]; // 每块地砖上的积雪深度
int tileOrder[MAXN]; // 地砖按雪深从大到小排序后的编号
int bootSnow[MAXN];  // 第 i 双靴子能承受的最大雪深
int bootStep[MAXN];  // 第 i 双靴子一步能迈出的最大距离
int bootOrder[MAXN]; // 靴子按承受雪深从大到小排序后的编号
int answer[MAXN];    // 每双靴子能否走完全程
int preTile[MAXN];   // 双向链表中某块砖的前一块
int nxtTile[MAXN];   // 双向链表中某块砖的后一块

// 排序比较: 雪深大的排在前面, 这样才能从深到浅依次删除地砖
bool CmpTile(int a, int c) { return snowDepth[a] > snowDepth[c]; }

// 排序比较: 承受雪深大的靴子先处理, 它的限制最松
bool CmpBoot(int a, int c) { return bootSnow[a] > bootSnow[c]; }

int main() {
    scanf("%d %d", &n, &b);
    for (int i = 0; i < n; i++) {
        scanf("%d", &snowDepth[i]);
        tileOrder[i] = i;
    }
    for (int i = 0; i < b; i++) {
        scanf("%d %d", &bootSnow[i], &bootStep[i]);
        bootOrder[i] = i;
    }
    sort(tileOrder, tileOrder + n, CmpTile);
    sort(bootOrder, bootOrder + b, CmpBoot);

    // 链表中留下的是"当前这双靴子踩得动"的地砖, 初始时所有砖都在链上
    for (int i = 0; i < n; i++) {
        preTile[i] = i - 1;
        nxtTile[i] = i + 1;
    }
    // 相邻地砖的间距都是 1, 所以初始最大间距为 1
    int maxGap = 1;
    int j = 0; // 从雪最深的砖开始, 依次把踩不动的砖摘掉

    // 按承受雪深从大到小处理靴子: s 变小, 需要删掉的深雪砖只会变多,
    // 所以 j 单调向前移动, 总删除次数不超过 N
    for (int i = 0; i < b; i++) {
        int boot = bootOrder[i];
        while (j < n && snowDepth[tileOrder[j]] > bootSnow[boot]) {
            int cur = tileOrder[j];
            // 从链表里摘掉这块踩不动的砖
            nxtTile[preTile[cur]] = nxtTile[cur];
            preTile[nxtTile[cur]] = preTile[cur];
            // 只有被删砖的前后两块砖之间可能出现新的最大间距
            int gap = nxtTile[cur] - preTile[cur];
            if (gap > maxGap) maxGap = gap;
            j++;
        }
        // 踩得动的砖之间最大间距不超过步长, 就能一路跳过去
        answer[boot] = (maxGap <= bootStep[boot]) ? 1 : 0;
    }

    for (int i = 0; i < b; i++) printf("%d\n", answer[i]);
    return 0;
}
