#include <cstdio>

using namespace std;

const int MAXN = 100005;              // 牛数（也是位置数）上限
const int MAXK = 200005;              // 一次完整舞蹈的交换次数上限
const int MAXVIEW = MAXN + 2 * MAXK;  // "某头牛在某时刻位于某位置"的记录条数上限

int n, k;
long long m;

int viewPos[MAXVIEW];    // 记录的"位置"部分
int viewTime[MAXVIEW];   // 记录的"时刻"部分；同一头牛的记录按时刻递增存放
int viewBegin[MAXN];     // 每头牛的记录在扁平数组中的起点
int viewCount[MAXN];     // 每头牛的记录条数

int swapA[MAXK], swapB[MAXK];  // 输入的交换对（第一遍统计条数、第二遍真正填记录）
int atPos[MAXN];               // 模拟中：当前站在位置 p 的牛
int perm[MAXN];                // perm[x]：第一轮 K 次交换结束后，第 x 头牛所在的位置
int ptr[MAXN];                 // 第二遍填记录时的写指针

int cnt[MAXN];        // 滑动窗口中每个位置被覆盖的次数（按重数计）
int uniqueCnt;        // 窗口内出现过的不同位置个数
int cycle[MAXN];      // 当前环上的牛（按 perm 的顺序）
bool visited[MAXN];
int answer[MAXN];

// 把 cow 的记录中"时刻 <= bar"的位置全部计入窗口
void addViews(int cow, int bar) {
    int end = viewBegin[cow] + viewCount[cow];
    for (int i = viewBegin[cow]; i < end; ++i) {
        // 记录已按时刻升序，一旦超时就无须再看后面的
        if (viewTime[i] > bar) break;
        if (cnt[viewPos[i]] == 0) ++uniqueCnt;
        ++cnt[viewPos[i]];
    }
}

// addViews 的逆操作
void removeViews(int cow, int bar) {
    int end = viewBegin[cow] + viewCount[cow];
    for (int i = viewBegin[cow]; i < end; ++i) {
        if (viewTime[i] > bar) break;
        --cnt[viewPos[i]];
        if (cnt[viewPos[i]] == 0) --uniqueCnt;
    }
}

int main() {
    scanf("%d%d%lld", &n, &k, &m);
    for (int i = 0; i < k; ++i) scanf("%d%d", &swapA[i], &swapB[i]);

    // 第一遍模拟：数出每头牛到底需要多少条记录（自己初始的位置 + 每次参与交换后的新位置）
    for (int i = 1; i <= n; ++i) {
        atPos[i] = i;
        viewCount[i] = 1;
    }
    for (int i = 0; i < k; ++i) {
        int a = swapA[i], b = swapB[i];
        int ca = atPos[a], cb = atPos[b];
        ++viewCount[ca];
        ++viewCount[cb];
        atPos[a] = cb;
        atPos[b] = ca;
    }
    int total = 0;
    for (int x = 1; x <= n; ++x) {
        viewBegin[x] = total;
        total += viewCount[x];
    }

    // 第二遍模拟：填出每头牛在时刻 0, 1, ..., K 访问过的位置
    for (int x = 1; x <= n; ++x) {
        ptr[x] = viewBegin[x];
        viewPos[ptr[x]] = x;
        viewTime[ptr[x]] = 0;
        ++ptr[x];
    }
    for (int i = 1; i <= n; ++i) atPos[i] = i;
    for (int i = 0; i < k; ++i) {
        int a = swapA[i], b = swapB[i];
        int ca = atPos[a], cb = atPos[b];
        viewPos[ptr[ca]] = b; viewTime[ptr[ca]] = i + 1; ++ptr[ca];
        viewPos[ptr[cb]] = a; viewTime[ptr[cb]] = i + 1; ++ptr[cb];
        atPos[a] = cb;
        atPos[b] = ca;
    }
    for (int p = 1; p <= n; ++p) perm[atPos[p]] = p;

    // 一轮 K 次交换把位置整体置换 perm，所以牛 x 在第 t 轮走的就是
    // "从位置 perm^t(x) 出发的那头牛" 在第一轮走过的路线。
    // 于是 M = D*K + R 分钟里，牛 x 的足迹 = 环上连续 D 个集合的并，
    // 再加上 perm^D(x) 那条路线中时刻 <= R 的部分。
    long long full = m / k;   // 完整轮数 D
    long long extra = m % k;  // 多出来的 R 次交换
    for (int s = 1; s <= n; ++s) {
        if (visited[s]) continue;
        int len = 0;
        int x = s;
        while (!visited[x]) {
            visited[x] = true;
            cycle[len++] = x;
            x = perm[x];
        }

        // D >= len 时整个环都被走遍，多出的 R 次交换不会带来新位置
        int w = (full >= len) ? len : (int)full;
        int bar = (full >= len) ? 0 : (int)extra;

        // 上一个环已在末尾清理干净，每个环都从"所有位置计数为 0"的窗口出发
        uniqueCnt = 0;
        for (int i = 0; i < w; ++i) addViews(cycle[i], k);   // 初始窗口 [0, w)

        for (int i = 0; i < len; ++i) {
            int nxt = i + w;
            if (nxt >= len) nxt -= len;
            addViews(cycle[nxt], bar);          // 补上多出的 R 次交换能到的新位置
            answer[cycle[i]] = uniqueCnt;
            removeViews(cycle[nxt], bar);
            if (w > 0) {                        // w == 0 时窗口为空，无需滑动
                removeViews(cycle[i], k);       // 左端点出窗
                addViews(cycle[nxt], k);        // 新右端点入窗
            }
        }
        // 转完一圈后窗口恰好回到初始的 [0, w)，把它整体撤掉，
        // 保证下一个环是在"所有位置计数为 0"的干净状态下开始滑动
        for (int i = 0; i < w; ++i) removeViews(cycle[i], k);
    }

    for (int i = 1; i <= n; ++i) printf("%d\n", answer[i]);
    return 0;
}
