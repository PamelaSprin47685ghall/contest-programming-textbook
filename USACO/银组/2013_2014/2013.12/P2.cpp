#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 205;
const long long INF = 1000000000000000000LL; // 1e18，大于任何真实路径长度

long long dist[MAXN][MAXN];

int main() {
    int n, m, k, q;
    scanf("%d %d %d %d", &n, &m, &k, &q);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = (i == j ? 0 : INF);
        }
    }

    for (int i = 0; i < m; i++) {
        int u, v, d;
        scanf("%d %d %d", &u, &v, &d);
        // 两个农场之间可能有多条航班，只保留最便宜的
        if (d < dist[u - 1][v - 1]) {
            dist[u - 1][v - 1] = d;
        }
    }

    // n <= 200，直接用 Floyd-Warshall 求全源最短路
    for (int x = 0; x < n; x++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][x] + dist[x][j] < dist[i][j]) {
                    dist[i][j] = dist[i][x] + dist[x][j];
                }
            }
        }
    }

    int cnt = 0;
    long long sum = 0;
    for (int i = 0; i < q; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        long long best = INF;
        // 中转站必须至少包含一个枢纽（编号 1..k），枚举是哪一个
        for (int h = 0; h < k; h++) {
            long long cost = dist[a - 1][h] + dist[h][b - 1];
            if (cost < best) {
                best = cost;
            }
        }
        if (best < INF) {
            cnt++;
            sum += best;
        }
    }

    printf("%d\n%lld\n", cnt, sum);
    return 0;
}
