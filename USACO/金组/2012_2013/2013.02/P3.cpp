#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 40005;
const int MAXR = 100005;

struct Route {
    int leftSite;   // 河左岸的景点编号
    int rightSite;  // 河右岸的景点编号
};

int n, m, r;
long long leftValue[MAXN];   // 左岸每个景点的观赏价值
long long rightValue[MAXN];  // 右岸每个景点的观赏价值
Route routes[MAXR];
long long bestFromLeft[MAXN];   // 从左岸第 i 个景点出发能取得的最大总分
long long bestFromRight[MAXN];  // 从右岸第 j 个景点出发能取得的最大总分

// 两条路线不交叉 => 左岸编号和右岸编号必须同时严格递增,
// 所以先处理左端点大的路线, 它要用的后继状态才已经算好
bool CompareRoute(const Route &a, const Route &b) {
    if (a.leftSite != b.leftSite) return a.leftSite > b.leftSite;
    return a.rightSite > b.rightSite;
}

int main() {
    scanf("%d %d %d", &n, &m, &r);
    for (int i = 1; i <= n; i++) scanf("%lld", &leftValue[i]);
    for (int j = 1; j <= m; j++) scanf("%lld", &rightValue[j]);
    for (int i = 0; i < r; i++) scanf("%d %d", &routes[i].leftSite, &routes[i].rightSite);

    // 一条只包含一个景点的路线也是合法的行程, 先当作 "原地结束"
    for (int i = 1; i <= n; i++) bestFromLeft[i] = leftValue[i];
    for (int j = 1; j <= m; j++) bestFromRight[j] = rightValue[j];

    sort(routes, routes + r, CompareRoute);
    for (int i = 0; i < r; i++) {
        int u = routes[i].leftSite;
        int v = routes[i].rightSite;
        // 走在 (u, v) 这条路线上的行程: 从 u 过来, 再从 v 继续往编号更大的地方走.
        // 此时 bestFromRight[v] / bestFromLeft[u] 里只有编号更大的那些路线,
        // 正好满足 "两条路线不交叉" 的要求
        long long goRight = leftValue[u] + bestFromRight[v];
        long long goLeft = rightValue[v] + bestFromLeft[u];
        if (goRight > bestFromLeft[u]) bestFromLeft[u] = goRight;
        if (goLeft > bestFromRight[v]) bestFromRight[v] = goLeft;
    }

    long long answer = 0;
    for (int i = 1; i <= n; i++)
        if (bestFromLeft[i] > answer) answer = bestFromLeft[i];
    for (int j = 1; j <= m; j++)
        if (bestFromRight[j] > answer) answer = bestFromRight[j];
    printf("%lld\n", answer);
    return 0;
}
