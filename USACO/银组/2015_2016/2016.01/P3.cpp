#include <cstdio>

using namespace std;

// 把坐标整体放大 2 倍：原来的每个格点变成奇数坐标上的格子，每段栅栏变成
// 中间那一格。这样栅栏成了有宽度的走廊，两片只在一个点上“斜着相碰”的区域
// 不会因为四连通被误判成同一片，洪水填充数出的连通块数恰好等于平面被栅栏
// 分成的区域数。区域之间补一扇门就能连起来，所以答案 = 区域数 - 1。
const int SIZE = 4005;    // 覆盖放大后的坐标范围 [-2000, 2000]，四周还留有边界
const int OFF = 2002;     // 把原点平移到下标 2002 处
const int MAXC = SIZE * SIZE;

const int DR[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, -1, 1};

bool fence[MAXC]; // 该格子是否被栅栏占据；洪水填充时兼作访问标记
int q[MAXC];      // 洪水填充用的队列，存格子的线性下标

int main() {
    int N;
    char path[1005];
    scanf("%d%s", &N, path);

    int cx = OFF, cy = OFF; // 当前位置（放大 2 倍后的格子坐标）
    fence[cx * SIZE + cy] = true; // 起点也是一个栅栏端点
    for (int i = 0; i < N; ++i) {
        int dx = 0, dy = 0;
        if (path[i] == 'N') {
            dy = -1;
        } else if (path[i] == 'S') {
            dy = 1;
        } else if (path[i] == 'W') {
            dx = -1;
        } else { // 'E'
            dx = 1;
        }
        // 一步原本走 1 个单位，放大后要走 2 格，中间那格也要记成栅栏
        for (int t = 0; t < 2; ++t) {
            cx += dx;
            cy += dy;
            fence[cx * SIZE + cy] = true;
        }
    }

    int regions = 0; // 互相走不通的区域个数（包含栅栏外面那一大片）
    for (int s = 0; s < MAXC; ++s) {
        if (fence[s]) continue;
        ++regions;
        int head = 0, tail = 0;
        q[tail++] = s;
        fence[s] = true; // 标记已访问，避免重复统计
        while (head < tail) {
            int cur = q[head++];
            int r = cur / SIZE, c = cur % SIZE;
            for (int k = 0; k < 4; ++k) {
                int nr = r + DR[k], nc = c + DC[k];
                if (nr < 0 || nr >= SIZE || nc < 0 || nc >= SIZE) continue;
                int nxt = nr * SIZE + nc;
                if (fence[nxt]) continue;
                fence[nxt] = true;
                q[tail++] = nxt;
            }
        }
    }

    printf("%d\n", regions - 1); // 每扇门最多把两块区域连起来
    return 0;
}
