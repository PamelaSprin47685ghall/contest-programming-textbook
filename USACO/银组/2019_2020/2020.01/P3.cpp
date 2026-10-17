// USACO 2020 January Contest, Silver P3: Wormhole Sort
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100000, MAXM = 100000;

int N, M;
int loc[MAXN + 1]; // loc[i]：位置 i 上的奶牛编号
struct Edge { int a, b, w; } E[MAXM];
int parent[MAXN + 1];

// 按宽度从大到小排序，判定时只需看宽度 >= minW 的前缀
bool cmpW(const Edge &x, const Edge &y) {
    return x.w > y.w;
}

int findRoot(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]]; // 路径减半
        x = parent[x];
    }
    return x;
}

// 只用宽度 >= minW 的虫洞时，每头牛能否走到自己的位置
bool canSort(int minW) {
    for (int i = 1; i <= N; i++) parent[i] = i;
    for (int i = 0; i < M && E[i].w >= minW; i++) {
        int ra = findRoot(E[i].a), rb = findRoot(E[i].b);
        if (ra != rb) parent[ra] = rb;
    }
    // 连通分量内部的奶牛可以通过交换任意重排，所以只需 p_i 与 i 同属一个分量
    for (int i = 1; i <= N; i++)
        if (findRoot(i) != findRoot(loc[i])) return false;
    return true;
}

int main() {
    scanf("%d %d", &N, &M);
    bool alreadySorted = true;
    for (int i = 1; i <= N; i++) {
        scanf("%d", &loc[i]);
        if (loc[i] != i) alreadySorted = false;
    }
    int maxW = 0;
    for (int i = 0; i < M; i++) {
        scanf("%d %d %d", &E[i].a, &E[i].b, &E[i].w);
        if (E[i].w > maxW) maxW = E[i].w;
    }
    if (alreadySorted) { // 本来就不需要任何虫洞
        printf("-1\n");
        return 0;
    }
    sort(E, E + M, cmpW);
    int lo = 1, hi = maxW;
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        if (canSort(mid)) lo = mid;
        else hi = mid - 1;
    }
    printf("%d\n", lo);
    return 0;
}
