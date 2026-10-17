#include <cstdio>
#include <algorithm>
#include <set>

using namespace std;

const int MAXN = 100005;

int N;
long long rawSize[MAXN], rawPos[MAXN]; // 读入时的原始数据
long long pos[MAXN], siz[MAXN];        // 按位置排序后的坐标与大小
int order[MAXN];                       // 干草堆下标, 按大小降序排列
bool covered[MAXN];                    // 相邻两个干草堆之间的路段是否被困
int nxt[MAXN];                         // 并查集: 下一个还没被判定的路段

bool cmpPos(int a, int b) { return rawPos[a] < rawPos[b]; }
bool cmpSize(int a, int b) { return siz[a] > siz[b]; }

// 并查集找下一个未覆盖的下标, 带路径压缩, 保证每个路段最多被真正覆盖一次
int findNext(int x) {
    int r = x;
    while (nxt[r] != r) r = nxt[r];
    while (nxt[x] != r) {
        int t = nxt[x];
        nxt[x] = r;
        x = t;
    }
    return r;
}

void cover(int l, int r) {
    for (int i = findNext(l); i <= r; i = findNext(i)) {
        covered[i] = true;
        nxt[i] = findNext(i + 1);
    }
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) scanf("%lld %lld", &rawSize[i], &rawPos[i]);

    for (int i = 0; i < N; i++) order[i] = i;
    sort(order, order + N, cmpPos);
    for (int i = 0; i < N; i++) {
        pos[i] = rawPos[order[i]];
        siz[i] = rawSize[order[i]];
    }
    for (int i = 0; i < N; i++) order[i] = i;
    sort(order, order + N, cmpSize);

    for (int i = 0; i <= N; i++) nxt[i] = i;

    // 按大小从大到小把干草堆放回路上。放第 i 个时, 左右两侧最近的已放置的干草堆
    // 就是夹住它的那对"更大的"干草堆, 而它们之间的干草堆全都还没放, 因此都比它小。
    // 若这段距离同时不超过两端干草堆的大小, 那么牛被这段距离限住, 撞不开任何一端,
    // 整段路都是困住它的区域。
    set<int> placed;
    for (int k = 0; k < N; k++) {
        int i = order[k];
        set<int>::iterator it = placed.lower_bound(i);
        if (it != placed.end()) {
            int j = *it;
            long long gap = pos[j] - pos[i];
            if (gap <= siz[i] && gap <= siz[j]) cover(i, j - 1);
        }
        if (it != placed.begin()) {
            int j = *(--it);
            long long gap = pos[i] - pos[j];
            if (gap <= siz[i] && gap <= siz[j]) cover(j, i - 1);
        }
        placed.insert(i);
    }

    long long ans = 0;
    for (int i = 0; i + 1 < N; i++)
        if (covered[i]) ans += pos[i + 1] - pos[i];
    printf("%lld\n", ans);
    return 0;
}
