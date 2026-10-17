#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n, m;
int parent[MAXN + 1];   // 并查集: 同一个 moo 网络的奶牛属于同一个集合
long long cowX[MAXN + 1], cowY[MAXN + 1];
long long minX[MAXN + 1], maxX[MAXN + 1];   // 每个集合的包围盒 (挂在根上)
long long minY[MAXN + 1], maxY[MAXN + 1];

int find(int v) {
    while (parent[v] != v) {
        parent[v] = parent[parent[v]];   // 路径压缩: 直接挂到祖父, 摊还近似 O(1)
        v = parent[v];
    }
    return v;
}

void unite(int u, int v) {
    int ru = find(u), rv = find(v);
    if (ru == rv) {
        return;
    }
    // 小并到大没必要, 直接把 ru 接到 rv 上并合并包围盒, 复杂度依然够用.
    parent[ru] = rv;
    if (minX[ru] < minX[rv]) minX[rv] = minX[ru];
    if (maxX[ru] > maxX[rv]) maxX[rv] = maxX[ru];
    if (minY[ru] < minY[rv]) minY[rv] = minY[ru];
    if (maxY[ru] > maxY[rv]) maxY[rv] = maxY[ru];
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        scanf("%lld%lld", &cowX[i], &cowY[i]);
        parent[i] = i;
        // 单头牛的包围盒就是它自己
        minX[i] = maxX[i] = cowX[i];
        minY[i] = maxY[i] = cowY[i];
    }
    for (int k = 0; k < m; ++k) {
        int a, b;
        scanf("%d%d", &a, &b);
        unite(a, b);
    }

    // 围栏要正好框住某个网络, 最小周长就是该网络包围盒的周长:
    // 周长比包围盒更小的矩形装不下网络里 x/y 最极端的那些牛.
    long long best = -1;
    for (int i = 1; i <= n; ++i) {
        if (find(i) != i) {
            continue;
        }
        long long perimeter = 2 * ((maxX[i] - minX[i]) + (maxY[i] - minY[i]));
        if (best < 0 || perimeter < best) {
            best = perimeter;
        }
    }
    printf("%lld\n", best);
    return 0;
}
