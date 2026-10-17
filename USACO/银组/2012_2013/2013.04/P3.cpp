#include <cstdio>

using namespace std;

const int MAXN = 1000;
const int MAXM = 500;

int n, m;
long long k;
int leftPort[MAXN];   // 每个港口的左河道通向哪个港口(0 下标)
int rightPort[MAXN];
char dirs[MAXM];      // 一轮里的 M 个指示
int nxt[MAXN];        // 从港口 i 完整走完一轮后到达的港口
int seen[MAXN];       // seen[i]: 第几步第一次到达港口 i, -1 表示还没到过

int main() {
    scanf("%d %d %lld", &n, &m, &k);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &leftPort[i], &rightPort[i]);
        --leftPort[i];
        --rightPort[i];
    }
    char buf[4];
    for (int j = 0; j < m; ++j) {
        scanf("%s", buf);
        dirs[j] = buf[0];
    }

    // 把"一整轮"看成一次跳跃: 从每个港口出发走完 M 条指示, 落到哪个港口.
    // 这样最多需要对每个港口模拟 M 步, 共 O(N*M).
    for (int i = 0; i < n; ++i) {
        int cur = i;
        for (int j = 0; j < m; ++j) {
            cur = (dirs[j] == 'L' ? leftPort[cur] : rightPort[cur]);
        }
        nxt[i] = cur;
    }

    // nxt 是每个点恰好一条出边的函数图: 沿它走必然在 N 步内进入一个环.
    // 于是先一步步走过去找出环, 环长 L 之后剩下的步数取模 L 即可, 不用真的走 K 步.
    for (int i = 0; i < n; ++i) {
        seen[i] = -1;
    }
    int cur = 0;  // 起点是 1 号港口
    long long step = 0;
    while (step < k && seen[cur] == -1) {
        seen[cur] = step;
        cur = nxt[cur];
        ++step;
    }
    if (step < k) {  // 说明撞上了环 (若正好走满 k 步则无需调整)
        long long cycle = step - seen[cur];
        long long rest = (k - step) % cycle;
        for (long long t = 0; t < rest; ++t) {
            cur = nxt[cur];
        }
    }

    printf("%d\n", cur + 1);
    return 0;
}
