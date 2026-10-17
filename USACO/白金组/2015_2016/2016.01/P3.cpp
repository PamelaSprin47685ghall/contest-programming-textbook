#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 210;               // 顶点数上限
const int NEG_INF = -1000000000;    // 表示"没有这种走法"

int n;
long long px[MAXN], py[MAXN];

// 把谷仓编码成一条"观测字符串"：偶数位放拐角（出口是 0），奇数位放边长。
// 左右转向用两个不同的负数表示，和边长（正数）、出口标记（0）都不会撞车。
const int TURN_CW = -1;
const int TURN_CCW = -2;

int seq[2 * MAXN + 2];
int seqLen;

int opt[MAXN + 1];    // opt[v]：灯亮时从顶点 v 走到出口的最短距离
int preSum[MAXN + 1]; // preSum[v]：前 v 条边的长度之和

// canon[i][len]：字符串 seq[i..i+len-1] 最早出现的位置（等价类的代表元）
int canon[2 * MAXN + 2][2 * MAXN + 2];

// nextLeft[start][size]：当前摸到的窗口代表元是 (start, size) 时，
// 再往左摸一条边之后，可能观察到的所有大小为 size+1 的窗口代表元。
// 注意"代表元"只由字符串决定，所以 Bessi 分不清的若干种真实位置会共用同一个状态。
vector<int> nextLeft[MAXN][MAXN];
vector<int> nextRight[MAXN][MAXN];

// dp[start][size][strt][side]：她已知的窗口是代表元 (start, size)，
// 出发点是窗口里第 strt 个顶点，人正站在 side 端（0 左端、1 右端）时，
// 从现在到摸清出口为止，"比灯亮时多走的距离"的最坏情况下的最小值。
int dp[MAXN][MAXN][MAXN][2];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%lld%lld", &px[i], &py[i]);

    // 顶点 0 是出口：先放它的标记 0，之后按顺序放"第 i 条边的长度、顶点 i+1 处的拐角"，
    // 最后那次 push 出来的拐角其实又落在顶点 0 上，改写成 0。
    seqLen = 0;
    seq[seqLen++] = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        int k = (i + 2) % n;
        long long dx = px[i] - px[j];
        long long dy = py[i] - py[j];
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        seq[seqLen++] = (int)(dx + dy);
        // 用叉积判断顶点 j 处往里凹还是往外凸
        long long cross = (px[i] - px[j]) * (py[k] - py[j]) - (px[k] - px[j]) * (py[i] - py[j]);
        seq[seqLen++] = (cross > 0) ? TURN_CW : TURN_CCW;
    }
    seq[seqLen - 1] = 0;

    // 灯亮时的最短距离：从顶点 v 出发只有顺时针、逆时针两种走法
    preSum[0] = 0;
    opt[0] = 0;
    for (int i = 0; i < n; ++i) {
        preSum[i + 1] = preSum[i] + seq[2 * i + 1];
        opt[i + 1] = preSum[i + 1];
    }
    opt[n] = 0; // 走到顶点 n（就是出口顶点 0）之后不用再走
    for (int i = n - 1; i >= 0; --i) {
        int cand = opt[i + 1] + seq[2 * i + 1];
        if (cand < opt[i]) opt[i] = cand;
    }

    // 求出每个子串等价类的代表元
    for (int len = 1; len <= seqLen; ++len) {
        for (int i = 0; i + len <= seqLen; ++i) {
            int j = 0;
            // 长度 len-1 的两段相同、且最后一个字符也相同时，j 就是等价位置
            for (; j < i; ++j) {
                if (canon[j][len - 1] == canon[i][len - 1] && seq[j + len - 1] == seq[i + len - 1]) break;
            }
            canon[i][len] = j;
        }
    }

    // 预先算好状态转移：谁能通过"往左/往右多摸一条边"变成谁
    for (int i = 0; i < seqLen; i += 2) {
        for (int len = 3; i + len <= seqLen; len += 2) {
            if (i != canon[i][len]) continue; // 只保留代表元
            // 去掉最左边一个字符（少摸一个顶点）后的代表元，就是这次转移的起点状态
            nextLeft[canon[i + 2][len - 2] / 2][len / 2].push_back(i / 2);
            nextRight[canon[i][len - 2] / 2][len / 2].push_back(i / 2);
        }
    }

    // 窗口越大信息越多，所以按窗口大小从大到小倒着做
    int result = 0;
    for (int ln = n; ln >= 1; --ln) {
        for (int i = 0; i + ln <= n + 1; ++i) {
            if (canon[2 * i][2 * ln - 1] != 2 * i) continue; // 只处理代表元
            // 窗口两端之间沿墙走的距离：站在一端要去另一端摸新边时要先走这么远
            int across = preSum[i + ln - 1] - preSum[i];
            for (int strt = 0; strt < ln; ++strt) {
                for (int side = 0; side < 2; ++side) {
                    if (i == 0 || i + ln == n + 1) {
                        // 窗口里已经露出出口：她能确定自己的位置，直接朝出口走最短路
                        dp[i][ln][strt][side] = -opt[i + strt];
                        continue;
                    }
                    // 往左摸：新摸到的边长 + 之后的最坏情况；换成"她其实在别的等价窗口"时取最坏
                    int leftCost = NEG_INF;
                    const vector<int> &leftList = nextLeft[i][ln];
                    for (size_t t = 0; t < leftList.size(); ++t) {
                        int j = leftList[t];
                        int cand = seq[2 * j + 1] + dp[j][ln + 1][strt + 1][0];
                        if (cand > leftCost) leftCost = cand;
                    }
                    // 往右摸：新摸到的边长 + 之后的最坏情况
                    int rightCost = NEG_INF;
                    const vector<int> &rightList = nextRight[i][ln];
                    for (size_t t = 0; t < rightList.size(); ++t) {
                        int j = rightList[t];
                        int cand = seq[2 * (j + ln) - 1] + dp[j][ln + 1][strt][1];
                        if (cand > rightCost) rightCost = cand;
                    }
                    // 站在右端还想往左摸（或站在左端还想往右摸），得先横穿整个窗口
                    if (side == 1) leftCost += across;
                    else rightCost += across;
                    dp[i][ln][strt][side] = (leftCost < rightCost ? leftCost : rightCost);
                    // 大小为 1 的窗口就是她的初始处境，所有出发点里取最坏
                    if (ln == 1 && dp[i][ln][strt][side] > result) result = dp[i][ln][strt][side];
                }
            }
        }
    }

    printf("%d\n", result);
    return 0;
}
