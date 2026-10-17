#include <cstdio>

using namespace std;

const int MAXN = 505;
const int MAXCELL = MAXN * MAXN;
const int DR[4] = {-1, 0, 1, 0};
const int DC[4] = {0, -1, 0, 1};
const int MAXELEV = 1000000000;

int m, n;
int elevation[MAXCELL];  // 把网格展成一维, 编号是 r*n+c
char isWaypoint[MAXCELL];
char visited[MAXCELL];
int queueCell[MAXCELL];
int waypointCount = 0;  // 路标总数
int startCell = 0;      // 随意取一个路标作为搜索起点

int Diff(int a, int b) { return a > b ? a - b : b - a; }

// 高差上限为 limit 时, 从某个路标出发能不能走到所有路标
bool Reachable(int limit) {
    for (int i = 0; i < m * n; ++i) visited[i] = 0;
    int head = 0, tail = 0;
    queueCell[tail++] = startCell;
    visited[startCell] = 1;
    int found = 1;  // 起点本身就是一个路标
    // 只有孤零零一个路标时, 一步都不用走就"互相可达"了
    if (found == waypointCount) return true;

    while (head < tail) {
        int cur = queueCell[head++];
        int r = cur / n, c = cur % n;
        for (int d = 0; d < 4; ++d) {
            int nr = r + DR[d], nc = c + DC[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
            int next = nr * n + nc;
            // 只能走相邻格子高差不超过 limit 的边, 否则这一步过不去
            if (visited[next] || Diff(elevation[cur], elevation[next]) > limit) continue;
            visited[next] = 1;
            if (isWaypoint[next]) {
                ++found;
                // 所有路标都已连通, 不必再搜完整个网格
                if (found == waypointCount) return true;
            }
            queueCell[tail++] = next;
        }
    }
    return false;
}

int main() {
    scanf("%d%d", &m, &n);
    for (int i = 0; i < m * n; ++i) scanf("%d", &elevation[i]);
    for (int i = 0; i < m * n; ++i) {
        int flag;
        scanf("%d", &flag);
        isWaypoint[i] = (char)flag;
        if (flag) {
            ++waypointCount;
            startCell = i;
        }
    }

    // "存在走法"这个性质关于 D 单调: D 变大只会让能走的边更多,
    // 所以可以二分答案, 再用一次 BFS 判断可行性.
    int low = 0, high = MAXELEV;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (Reachable(mid)) high = mid;
        else low = mid + 1;
    }
    printf("%d\n", low);
    return 0;
}
