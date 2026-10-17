#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 10005;

int N, M;
int milkTime[MAXN];      // 每头牛挤奶所需时间
int indegree[MAXN];      // 还有多少头牛必须在它之前挤完
long long finish[MAXN];  // 每头牛最早挤完的时刻
vector<int> after[MAXN]; // after[a] 里是必须在 a 之后才能开始挤奶的牛

int main() {
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= N; ++i) scanf("%d", &milkTime[i]);
    for (int i = 0; i < M; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        after[a].push_back(b);  // a 必须完全挤完才能开始挤 b
        ++indegree[b];
    }

    // 牛可以无限并行, 所以拖时间没有好处: 一头牛的最早开始时间就是
    // 所有"必须在它之前完成"的牛的最晚完成时间, 按拓扑序递推即可.
    queue<int> q;
    for (int i = 1; i <= N; ++i) {
        if (indegree[i] == 0) {
            finish[i] = milkTime[i];
            q.push(i);
        }
    }

    long long answer = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        answer = max(answer, finish[u]);
        for (int k = 0; k < (int)after[u].size(); ++k) {
            int v = after[u][k];
            // u 挤完后 v 才可能开始, 用 u 的完成时刻更新 v 的完成时刻
            finish[v] = max(finish[v], finish[u] + milkTime[v]);
            // 所有前驱都处理完, v 的完成时刻才最终确定, 这时才入队
            if (--indegree[v] == 0) q.push(v);
        }
    }

    printf("%lld\n", answer);
    return 0;
}
