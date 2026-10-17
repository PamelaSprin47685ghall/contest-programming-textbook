#include <cstdio>

using namespace std;

const int MAXN = 5000; // 单组数据堆数上限
const int MOD = 1000000007;

int h[MAXN];
int rankInClass[MAXN]; // 每堆干草在自己奇偶类中的 1 基编号
int oddCount, evenCount;
// mustBefore[p][r]：类别 p 的第 r 堆之前，必须排好队的另一类干草的最大编号
// （另一类里编号超过它的那些都不用管，因为它们本来就在更后面）
int mustBefore[2][MAXN + 1];
int prevRow[MAXN + 1], curRow[MAXN + 1];

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) scanf("%d", &h[i]);

        oddCount = evenCount = 0;
        for (int i = 0; i < n; ++i) {
            if (h[i] % 2 == 1) rankInClass[i] = ++oddCount;
            else rankInClass[i] = ++evenCount;
        }
        for (int p = 0; p < 2; ++p)
            for (int r = 0; r <= MAXN; ++r) mustBefore[p][r] = 0;

        // 奇偶性相同（差为偶数）或高度差不为 1 的两堆永远换不了位置：
        // 靠前的那堆必须一直排在靠后的那堆前面。
        // 奇偶相同时相对顺序被完全锁死，只有异奇偶且高度差恰好为 1 的才能互换，
        // 所以只需记录异奇偶、高度差至少为 2 的先后约束。
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (h[i] % 2 == h[j] % 2) continue;
                int diff = h[i] - h[j];
                if (diff < 0) diff = -diff;
                if (diff >= 2 && rankInClass[i] > mustBefore[h[j] % 2][rankInClass[j]])
                    mustBefore[h[j] % 2][rankInClass[j]] = rankInClass[i];
            }
        }

        // dp[j][k]：最终序列的前 j+k 位，恰好用掉前 j 堆奇数高度、前 k 堆偶数高度
        // 的方案数。同一类内部顺序固定，所以状态只由 j、k 决定。
        // 逐行滚动：prevRow 是 dp[j-1][*]，curRow 是 dp[j][*]。
        prevRow[0] = 1;
        for (int k = 1; k <= evenCount; ++k)
            prevRow[k] = (mustBefore[0][k] == 0) ? prevRow[k - 1] : 0; // j = 0 时偶数堆必须按编号依次放

        for (int j = 1; j <= oddCount; ++j) {
            // 把第 j 堆奇数放到第 j+k 位，前提是它在偶数类中的"前驱"都已就位
            curRow[0] = (mustBefore[1][j] == 0) ? prevRow[0] : 0;
            for (int k = 1; k <= evenCount; ++k) {
                long long val = 0;
                if (mustBefore[1][j] <= k) val += prevRow[k];      // 这一位放第 j 堆奇数
                if (mustBefore[0][k] <= j) val += curRow[k - 1];   // 这一位放第 k 堆偶数
                curRow[k] = (int)(val % MOD);
            }
            for (int k = 0; k <= evenCount; ++k) prevRow[k] = curRow[k];
        }
        printf("%d\n", prevRow[evenCount]);
    }
    return 0;
}
