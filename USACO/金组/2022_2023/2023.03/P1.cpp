#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 100005;

int N, M;
vector<int> corridor[MAXN];  // 走廊构成的邻接表
int stallColor[MAXN];        // C_i：栅栏 i 的颜色，也是进入它所需要的钥匙颜色
int initialKey[MAXN];        // S_i：一开始放在栅栏 i 里的钥匙颜色
int targetKey[MAXN];         // F_i：最后栅栏 i 里应该放着的钥匙颜色
int gainedKey[MAXN];         // 本趟搜索访问栅栏 v 时能拿到的钥匙颜色
char inFirstStage[MAXN];     // 第一阶段到过的栅栏
char inSecondStage[MAXN];    // 第二阶段到过的栅栏
char allowedStall[MAXN];     // 第二阶段只允许在第一阶段到过的栅栏之间活动
char discovered[MAXN];       // 已经进入队列或等待名单，避免重复发现
char heldKey[MAXN];          // 当前是否持有某种颜色的钥匙
vector<int> waiting[MAXN];   // 还差某种颜色的钥匙才能进入的栅栏

// 贪心搜索：每访问一个栅栏就把它的钥匙捡起来，因为多拿一把钥匙只会更有用。
// stage = 0 正向捡钥匙；stage = 1 是倒着推演放钥匙的过程。
void search(int stage) {
    queue<int> q;
    q.push(1);
    discovered[1] = 1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (stage == 0) {
            inFirstStage[v] = 1;
        } else {
            inSecondStage[v] = 1;
        }
        int key = gainedKey[v];
        // 手里多了这把钥匙以后，所有等着它的栅栏立刻都能进入
        if (!heldKey[key]) {
            heldKey[key] = 1;
            for (int u : waiting[key]) {
                q.push(u);
            }
            waiting[key].clear();
        }
        for (int u : corridor[v]) {
            if (discovered[u]) continue;
            if (stage == 1 && !allowedStall[u]) continue;
            discovered[u] = 1;
            if (heldKey[stallColor[u]]) {
                q.push(u);
            } else if (stage == 1 && stallColor[u] == targetKey[u]) {
                // 倒着推演时，栅栏 u 里本来放着的就是它需要的钥匙 F_u，
                // 所以一进去就能拿到颜色 C_u 的钥匙，也就允许进入它
                q.push(u);
            } else {
                waiting[stallColor[u]].push_back(u);
            }
        }
    }
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &N, &M);
        for (int i = 1; i <= N; i++) {
            scanf("%d", &stallColor[i]);
        }
        for (int i = 1; i <= N; i++) {
            scanf("%d", &initialKey[i]);
        }
        for (int i = 1; i <= N; i++) {
            scanf("%d", &targetKey[i]);
        }
        for (int i = 0; i < M; i++) {
            int u, v;
            scanf("%d%d", &u, &v);
            corridor[u].push_back(v);
            corridor[v].push_back(u);
        }

        // 第一阶段：FJ 一路捡钥匙，能捡多少就捡多少
        for (int i = 1; i <= N; i++) {
            gainedKey[i] = initialKey[i];
        }
        search(0);

        // 第二阶段：倒着推演「把钥匙放回栅栏」的过程。
        // 放钥匙的正向过程倒过来看，只是一次同样形式的捡钥匙过程，
        // 只不过进入栅栏的条件变成了离开栅栏的条件。
        for (int i = 1; i <= N; i++) {
            gainedKey[i] = targetKey[i];
            allowedStall[i] = inFirstStage[i];
            discovered[i] = 0;
            heldKey[i] = 0;
            waiting[i].clear();
        }
        search(1);

        // 第一阶段到过的栅栏必须也能在第二阶段到过；
        // 两阶段都没到过的栅栏从头到尾碰不到，所以它一开始装的钥匙
        // 就必须已经是它需要的钥匙
        bool possible = true;
        for (int i = 1; i <= N; i++) {
            if (inFirstStage[i]) {
                if (!inSecondStage[i]) possible = false;
            } else if (initialKey[i] != targetKey[i]) {
                possible = false;
            }
        }
        printf("%s\n", possible ? "YES" : "NO");

        // 为下一个测试用例清空全局数组
        for (int i = 1; i <= N; i++) {
            corridor[i].clear();
            inFirstStage[i] = 0;
            inSecondStage[i] = 0;
            allowedStall[i] = 0;
            discovered[i] = 0;
            heldKey[i] = 0;
            waiting[i].clear();
        }
    }
    return 0;
}
