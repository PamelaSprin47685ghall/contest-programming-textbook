#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 5000; // 奶牛数量上限

int T, N, K;
int guernseyLocation[MAXN]; // 按位置升序排列的 Guernsey（G）牛
int guernseyWeight[MAXN];
int holsteinLocation[MAXN]; // 按位置升序排列的 Holstein（H）牛
int holsteinWeight[MAXN];
int guernseyCount, holsteinCount;
int totalWeight; // 所有奶牛体重之和（最大 5e8，int 放得下）

int locationGap(int a, int b) {
    return a > b ? a - b : b - a;
}

// T = 1：让未配对的体重和最小，等价于让配对的体重和最大；
// 反过来，体重和最大的配对方案一定是极大的（否则还能再添一对让总和更大）。
int solveMin() {
    // dp[j]：处理完前 i 只 G 和前 j 只 H 时，配成对的奶牛最大体重和。
    // 只保留上一行和本行，省下 O(N^2) 的空间。
    vector<int> dp(holsteinCount + 1, 0);
    for (int i = 1; i <= guernseyCount; ++i) {
        int diagonal = 0; // dp[i-1][j-1]，j = 1 时是 dp[i-1][0]
        for (int j = 1; j <= holsteinCount; ++j) {
            int previousRow = dp[j]; // dp[i-1][j]
            int best = previousRow > dp[j - 1] ? previousRow : dp[j - 1];
            // 把第 i 只 G 和第 j 只 H 配成一对：换一种配对方式时，
            // 总是按位置顺序依次配最有利，所以这里只考虑"下一只配下一只"。
            if (locationGap(guernseyLocation[i - 1], holsteinLocation[j - 1]) <= K) {
                int paired = diagonal + guernseyWeight[i - 1] + holsteinWeight[j - 1];
                if (paired > best) best = paired;
            }
            diagonal = previousRow;
            dp[j] = best;
        }
    }
    return totalWeight - dp[holsteinCount];
}

// T = 2：让未配对的体重和最大。
// dp 的含义：mxLo[x][y] / mxHi[x][y] = 前 x 只 G、前 y 只 H 里未配对的体重
// 之和的最大值，其中"下一只允许不配对的牛"必须是 G（mxLo）或 H（mxHi）。
int solveMax() {
    int g = guernseyCount;
    int h = holsteinCount;
    int width = h + 1;

    // blockedHolstein[x]：位置在"第 x 只 G 的位置 + K"以内的 H 的只数。
    // 如果上一只未配对的牛是第 x 只 G，这些 H 必须全部配对，否则它们能和这只 G 配对。
    vector<int> blockedHolstein(g + 1, 0);
    int counted = 0;
    for (int x = 1; x <= g; ++x) {
        while (counted < h && holsteinLocation[counted] <= (long long)guernseyLocation[x - 1] + K) ++counted;
        blockedHolstein[x] = counted;
    }
    // blockedGuernsey[y] 同理：位置在"第 y 只 H 的位置 + K"以内的 G 的只数。
    vector<int> blockedGuernsey(h + 1, 0);
    counted = 0;
    for (int y = 1; y <= h; ++y) {
        while (counted < g && guernseyLocation[counted] <= (long long)holsteinLocation[y - 1] + K) ++counted;
        blockedGuernsey[y] = counted;
    }

    // runEnd[x][y]：从"第 x+1 只 G 配第 y+1 只 H"开始沿对角线一路配下去，
    // 最远能配到第几只 G；下一对就不合法（或根本不存在）时就是 x。
    vector<int> runEnd((g + 1) * width, 0);
    for (int x = g; x >= 0; --x) {
        for (int y = h; y >= 0; --y) {
            if (x < g && y < h && locationGap(guernseyLocation[x], holsteinLocation[y]) <= K)
                runEnd[x * width + y] = runEnd[(x + 1) * width + (y + 1)];
            else
                runEnd[x * width + y] = x;
        }
    }

    vector<int> mxLo((g + 1) * width, -1);
    vector<int> mxHi((g + 1) * width, -1);
    mxLo[0] = 0;
    mxHi[0] = 0;
    for (int x = 0; x <= g; ++x) {
        for (int y = 0; y <= h; ++y) {
            int index = x * width + y;
            int valueLo = mxLo[index];
            int valueHi = mxHi[index];

            // 把许可从 G 换成 H：下一只 H 必须落在上一只未配对的 G 右边 K 以外。
            // 需要先把中间那些 H 与 G 按对角线配掉，所以要求这一段对角线全合法；
            // 配对只是在消耗牛，不改变未配对的体重和。
            int nextY = y > blockedHolstein[x] ? y : blockedHolstein[x];
            if (nextY == y) {
                if (valueLo > valueHi) valueHi = valueLo; // 原地换许可，不用配任何牛
            } else if (runEnd[index] >= x + nextY - y) {
                int target = (x + nextY - y) * width + nextY;
                if (valueLo > mxHi[target]) mxHi[target] = valueLo;
            }
            // 反向换许可：下一只 G 落在上一只未配对的 H 右边 K 以外。
            int nextX = x > blockedGuernsey[y] ? x : blockedGuernsey[y];
            if (nextX == x) {
                if (valueHi > valueLo) valueLo = valueHi;
            } else if (runEnd[index] >= nextX) {
                int target = nextX * width + (nextX - x + y);
                if (valueHi > mxLo[target]) mxLo[target] = valueHi;
            }
            // 换许可后原地留下的更优值也要写回，供后面的转移使用
            mxLo[index] = valueLo;
            mxHi[index] = valueHi;

            // 把第 x+1 只 G 和第 y+1 只 H 配成一对
            if (runEnd[index] > x) {
                int target = (x + 1) * width + (y + 1);
                if (valueLo > mxLo[target]) mxLo[target] = valueLo;
                if (valueHi > mxHi[target]) mxHi[target] = valueHi;
            }
            // 第 x+1 只 G 不配对（只有拿了 G 许可才行，它自动与已出现的 H 距离够远）
            if (x < g && valueLo >= 0) {
                int target = (x + 1) * width + y;
                int candidate = valueLo + guernseyWeight[x];
                if (candidate > mxLo[target]) mxLo[target] = candidate;
            }
            // 第 y+1 只 H 不配对（只有拿了 H 许可才行）
            if (y < h && valueHi >= 0) {
                int target = x * width + (y + 1);
                int candidate = valueHi + holsteinWeight[y];
                if (candidate > mxHi[target]) mxHi[target] = candidate;
            }
        }
    }
    int answer = mxLo[g * width + h];
    if (mxHi[g * width + h] > answer) answer = mxHi[g * width + h];
    return answer;
}

int main() {
    scanf("%d%d%d", &T, &N, &K);
    for (int i = 0; i < N; ++i) {
        char breed;
        int position, weight;
        scanf(" %c%d%d", &breed, &position, &weight);
        if (breed == 'G') {
            guernseyLocation[guernseyCount] = position;
            guernseyWeight[guernseyCount] = weight;
            ++guernseyCount;
        } else {
            holsteinLocation[holsteinCount] = position;
            holsteinWeight[holsteinCount] = weight;
            ++holsteinCount;
        }
        totalWeight += weight;
    }
    if (T == 1) printf("%d\n", solveMin());
    else printf("%d\n", solveMax());
    return 0;
}
