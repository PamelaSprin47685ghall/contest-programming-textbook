#include <cstdio>
#include <algorithm>

using namespace std;

// 点 i 记录"从 i 一路滑到 1 的路径上前 11 大的 run 难度"（降序存放，不足用 -1 补位）
// 以及总享受值 E[i]。朋友最多允许 c_j <= 10 条难度大于 s_j 的 run，等价于要求路径上
// 第 c_j + 1 大的难度 <= s_j，所以只需要前 11 大：父节点的前 11 大里没有的难度，在
// 子节点（路径更长）里也不可能挤进前 11 大。
// 对每个 c = 0..10，把点按"第 c+1 大难度"升序排序并做前缀最大享受值，这样一次二分
// 就能回答所有 c_j = c 的询问。

const int MAXN = 100005;
const int MAXC = 11;   // c_j <= 10
const int NO_RUN = -1; // 补位的"不存在"难度，s_j >= 0 所以不会被误判

int N, M;
int parent[MAXN];
int edgeD[MAXN];
int edgeE[MAXN];
long long enjoySum[MAXN];  // E[i]：从 i 滑到 1 的总享受值
int topD[MAXN][MAXC];      // 降序的前 11 大难度

struct Item {
    int difficulty;
    long long enjoyment;
};

Item items[MAXN];
int sortedD[MAXN];
long long answer[MAXN];
int qs[MAXN], qc[MAXN];

bool cmpItem(const Item &x, const Item &y) {
    return x.difficulty < y.difficulty;
}

int main() {
    scanf("%d", &N);
    for (int i = 2; i <= N; ++i) {
        scanf("%d %d %d", &parent[i], &edgeD[i], &edgeE[i]);
    }

    // 父节点编号一定小于子节点，所以按编号从小到大递推即可
    for (int c = 0; c < MAXC; ++c) {
        topD[1][c] = NO_RUN;
    }
    enjoySum[1] = 0;
    for (int i = 2; i <= N; ++i) {
        int p = parent[i];
        enjoySum[i] = enjoySum[p] + edgeE[i];
        for (int c = 0; c < MAXC; ++c) {
            topD[i][c] = topD[p][c];
        }
        // 把本边的难度插进降序序列，只保留前 11 个：若它比保留的最后一个还小就直接丢掉，
        // 否则从后往前把小于它的元素整体右移，再放到空出的位置
        int d = edgeD[i];
        if (d > topD[i][MAXC - 1]) {
            int j = MAXC - 2;
            while (j >= 0 && topD[i][j] < d) {
                topD[i][j + 1] = topD[i][j];
                --j;
            }
            topD[i][j + 1] = d;
        }
    }

    scanf("%d", &M);
    for (int q = 0; q < M; ++q) {
        scanf("%d %d", &qs[q], &qc[q]);
    }

    for (int c = 0; c < MAXC; ++c) {
        for (int i = 1; i <= N; ++i) {
            items[i - 1].difficulty = topD[i][c];
            items[i - 1].enjoyment = enjoySum[i];
        }
        sort(items, items + N, cmpItem);
        // 前缀最大：排序后前缀里的点都满足"第 c+1 大难度 <= 当前阈值"
        for (int i = 1; i < N; ++i) {
            if (items[i - 1].enjoyment > items[i].enjoyment) {
                items[i].enjoyment = items[i - 1].enjoyment;
            }
        }
        for (int i = 0; i < N; ++i) {
            sortedD[i] = items[i].difficulty;
        }
        for (int q = 0; q < M; ++q) {
            if (qc[q] != c) {
                continue;
            }
            // 最后一个第 c+1 大难度 <= s 的位置，它的前缀最大值就是答案
            int pos = (int)(upper_bound(sortedD, sortedD + N, qs[q]) - sortedD) - 1;
            answer[q] = (pos >= 0) ? items[pos].enjoyment : 0;
        }
    }

    for (int q = 0; q < M; ++q) {
        printf("%lld\n", answer[q]);
    }
    return 0;
}
