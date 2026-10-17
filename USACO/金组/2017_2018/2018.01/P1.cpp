#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;  // 视频数与询问数的上限

// 一条连接, 以及 FJ 手工算出的相关度
struct Tunnel {
    int a, b, relevance;
};

// 一个询问: 阈值 K, 起点 v, 以及它在输出里的位置
struct Question {
    int k, v, id;
};

int n, q;
Tunnel tunnels[MAXN];
Question questions[MAXN];
int answer[MAXN];

// 并查集: 维护每个"相关度足够高"的连通块的大小
int parent[MAXN + 1];
int compSize[MAXN + 1];

bool MoreRelevant(const Tunnel &x, const Tunnel &y) {
    return x.relevance > y.relevance;
}

bool LargerThreshold(const Question &x, const Question &y) {
    return x.k > y.k;
}

int Find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];  // 路径折半, 顺便压平整条路
        x = parent[x];
    }
    return x;
}

int main() {
    scanf("%d %d", &n, &q);
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d %d", &tunnels[i].a, &tunnels[i].b, &tunnels[i].relevance);
    }
    for (int i = 0; i < q; i++) {
        scanf("%d %d", &questions[i].k, &questions[i].v);
        questions[i].id = i;
    }

    // 询问离线后按 K 从大到小处理: 阈值越小, 能用的连接越多,
    // 所以每条连接在整个过程中只会被加进并查集一次.
    sort(tunnels, tunnels + n - 1, MoreRelevant);
    sort(questions, questions + q, LargerThreshold);

    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        compSize[i] = 1;
    }

    int added = 0;  // 已经把前 added 条连接加进了图
    for (int i = 0; i < q; i++) {
        while (added < n - 1 && tunnels[added].relevance >= questions[i].k) {
            int rootA = Find(tunnels[added].a);
            int rootB = Find(tunnels[added].b);
            if (rootA != rootB) {
                if (compSize[rootA] < compSize[rootB]) {
                    int tmp = rootA;
                    rootA = rootB;
                    rootB = tmp;
                }
                parent[rootB] = rootA;
                compSize[rootA] += compSize[rootB];
            }
            added++;
        }
        // 与 v 相关度达标的就是 v 所在连通块里的其他所有视频
        answer[questions[i].id] = compSize[Find(questions[i].v)] - 1;
    }

    for (int i = 0; i < q; i++) {
        printf("%d\n", answer[i]);
    }
    return 0;
}
