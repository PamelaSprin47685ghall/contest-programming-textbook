#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1000;  // 房间数上限
const int MAXK = 7;     // 最多解锁的外门数
const long long INF = 1000000000000000000LL; // 比任何可行答案都大，表示"做不到"

int roomCount, doorCount;             // 房间数与要解锁的门数
long long want[MAXN + 1];             // 每间房需要多少奶牛
long long ring[2 * MAXN + 1];         // 环形房间按顺时针复制成两圈
long long cowSum[2 * MAXN + 1];       // ring 上奶牛数的前缀和
long long weightedSum[2 * MAXN + 1];  // ring 上"房间位置 × 奶牛数"的前缀和
int firstPos;                         // 本次枚举中第一扇门在 ring 上的位置
long long dp[MAXK + 1][MAXN + 1];     // dp[j][i]：用 j 扇门管好本地第 1..i 间房的最小总距离

// 本地第 from..to 间房全都从 from 处的门进入时，这些奶牛走的总距离。
// 房间 t 到门的距离就是 t - from，用前缀和把它拆成"位置×奶牛数"的差
long long segmentCost(int from, int to) {
    int leftPos = firstPos + from - 1;
    int rightPos = firstPos + to - 1;
    return (weightedSum[rightPos] - weightedSum[leftPos - 1]) -
           (long long)leftPos * (cowSum[rightPos] - cowSum[leftPos - 1]);
}

// 分治地求 dp[doors][lo..hi]：dp[doors - 1][*] 已经算好。
// 这些位置的决策点（最后一段的起点）随 i 单调不减，所以每个决策点只需在
// 一段区间里找，区间在递归中不断收窄，均摊下来每层只要 O(n log n) 次尝试
void solve(int doors, int lo, int hi, int optLow, int optHigh) {
    if (lo > hi) return;
    int mid = (lo + hi) / 2;
    int bestStart = optLow;
    dp[doors][mid] = INF;
    int upper = min(optHigh, mid);
    for (int from = optLow; from <= upper; ++from) {
        long long candidate = dp[doors - 1][from - 1] + segmentCost(from, mid);
        if (candidate < dp[doors][mid]) {
            dp[doors][mid] = candidate;
            bestStart = from;
        }
    }
    solve(doors, lo, mid - 1, optLow, bestStart);
    solve(doors, mid + 1, hi, bestStart, optHigh);
}

int main() {
    scanf("%d%d", &roomCount, &doorCount);
    for (int i = 1; i <= roomCount; ++i) scanf("%lld", &want[i]);
    for (int i = 1; i <= 2 * roomCount; ++i) ring[i] = want[(i - 1) % roomCount + 1];
    for (int i = 1; i <= 2 * roomCount; ++i) {
        cowSum[i] = cowSum[i - 1] + ring[i];
        weightedSum[i] = weightedSum[i - 1] + (long long)i * ring[i];
    }

    long long answer = INF;
    // 奶牛只朝顺时针走，所以每间房都由它逆时针方向最近的那扇门服务：
    // 门把环切成 k 段，每段里的奶牛都从段首那扇门进来。
    // 枚举第一扇门的位置，环就拉直成一条链，剩下的是"把链切成 k 段"的 DP
    for (int first = 1; first <= roomCount; ++first) {
        firstPos = first;
        for (int doors = 0; doors <= doorCount; ++doors)
            for (int i = 0; i <= roomCount; ++i) dp[doors][i] = INF;
        // 本地第 1..i 间房用 j 扇门：段数不能超过房间数，INF 会把它们自动排除
        dp[0][0] = 0;
        for (int doors = 1; doors <= doorCount; ++doors)
            solve(doors, doors, roomCount, 1, roomCount);
        if (dp[doorCount][roomCount] < answer) answer = dp[doorCount][roomCount];
    }

    printf("%lld\n", answer);
    return 0;
}
