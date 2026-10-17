#include <cstdio>
#include <utility>
#include <vector>

using namespace std;

const int MAXN = 105;

int N, M;
bool lit[MAXN][MAXN];     // 房间是否被点亮
bool visited[MAXN][MAXN]; // 房间是否已经到达过
vector<pair<int, int> > switches[MAXN][MAXN]; // 每个房间里能打开的房间

const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

// 待处理的房间队列：已经点亮、且可能可以走过去的房间。
// 同一个房间可能因为多个邻居而重复入队，出队时再判重即可。
vector<pair<int, int> > que;

bool inside(int x, int y) {
    return x >= 1 && x <= N && y >= 1 && y <= N;
}

// 判断 (x,y) 是否紧挨着某个已经到达过的房间。
// 只有满足这一点，新点亮的房间才有可能被走过去。
bool hasVisitedNeighbor(int x, int y) {
    for (int k = 0; k < 4; ++k) {
        int nx = x + dx[k], ny = y + dy[k];
        if (inside(nx, ny) && visited[nx][ny]) return true;
    }
    return false;
}

int main() {
    scanf("%d%d", &N, &M);
    for (int i = 0; i < M; ++i) {
        int x, y, a, b;
        scanf("%d%d%d%d", &x, &y, &a, &b);
        switches[x][y].push_back(make_pair(a, b));
    }

    // 只有 (1,1) 一开始是亮的，Bessie 从那里出发
    lit[1][1] = true;
    que.push_back(make_pair(1, 1));

    for (size_t head = 0; head < que.size(); ++head) {
        int x = que[head].first, y = que[head].second;
        if (visited[x][y]) continue; // 重复入队的房间只处理一次
        visited[x][y] = true;

        // 打开这个房间里所有的开关
        for (size_t i = 0; i < switches[x][y].size(); ++i) {
            int a = switches[x][y][i].first, b = switches[x][y][i].second;
            if (lit[a][b]) continue;
            lit[a][b] = true;
            // 新点亮的房间如果和已到达的区域相邻，就能从那里走进去
            if (hasVisitedNeighbor(a, b)) que.push_back(make_pair(a, b));
        }

        // 四周已经点亮、且还没到过的房间，都可以走过去
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k], ny = y + dy[k];
            if (inside(nx, ny) && lit[nx][ny] && !visited[nx][ny])
                que.push_back(make_pair(nx, ny));
        }
    }

    int answer = 0;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= N; ++j)
            if (lit[i][j]) ++answer;
    printf("%d\n", answer);
    return 0;
}
