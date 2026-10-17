#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 1000005;

int N;
int a[MAXN];              // 原数组，1 下标
int leftActive[MAXN + 2]; // 左侧最近的"仍未处理"下标（其 (值,下标) 字典序更大）
int rightActive[MAXN + 2];
int order[MAXN];          // 按 (值, 下标) 升序排列的下标
int bucketStart[MAXN + 2];

long long answer;

// [1, x] 中奇偶性为 p 的整数个数
inline long long CountUpto(int x, int p) {
    if (x <= 0) return 0;
    if (p == 0) return x / 2;
    return (x + 1) / 2;
}

// [lo, hi] 中奇偶性为 p 的整数个数
inline long long CountRange(int lo, int hi, int p) {
    if (lo > hi) return 0;
    return CountUpto(hi, p) - CountUpto(lo - 1, p);
}

// 奇数长度、答案恰为 a[i] 的子数组个数。
// 此时 i 必须是（按 (值,下标) 打破平局后的）最大值：子数组包含 i，
// 且不含除 i 之外的任何"活跃"元素。左右端点必须同奇偶。
inline long long CountOddFirst(int i) {
    long long res = 0;
    for (int p = 0; p < 2; ++p) {
        res += CountRange(leftActive[i] + 1, i, p) * CountRange(i, rightActive[i] - 1, p);
    }
    return res;
}

// 偶数长度、i 是（打破平局后的）次大值，且另一个活跃元素是 x 或 y
//（x、y 必须是相邻的活跃下标）。左右端点必须异奇偶。
inline long long CountEvenPair(int x, int y) {
    if (x <= 0 || y >= N + 1) return 0;
    long long res = 0;
    for (int p = 0; p < 2; ++p) {
        res += CountRange(leftActive[x] + 1, x, p) * CountRange(y, rightActive[y] - 1, p ^ 1);
    }
    return res;
}

inline long long CountEvenSecond(int i) {
    return CountEvenPair(leftActive[i], i) + CountEvenPair(i, rightActive[i]);
}

// 给定 0/1 数组（arrayMask 的第 j 位表示第 j 个位置的值），
// 用"可达状态集合"DP 求出该游戏的最大最终值。
// 状态 mask 记录"还剩下的位置集合"；初始状态是全集，与值数组区分开。
// 因为每一步操作数固定，下一步是最小还是最大由已做操作数的奇偶决定。
int SolveZeroOne(int len, int arrayMask) {
    int full = (1 << len) - 1;
    bool dp[64];
    for (int s = 0; s < 64; ++s) dp[s] = false;
    dp[full] = true;
    int best = 0;
    for (int state = full; state >= 1; --state) {
        if (!dp[state]) continue;
        int cnt = __builtin_popcount(state);
        if (cnt == 1) {
            // 只留一个位置：它的值就是最终值
            for (int j = 0; j < len; ++j) {
                if (((state >> j) & 1) && ((arrayMask >> j) & 1)) best = 1;
            }
            continue;
        }
        int opParity = (len - cnt) & 1; // 0 表示下一步做 min，1 表示 max
        int last = -1;
        for (int j = 0; j < len; ++j) {
            if (!((state >> j) & 1)) continue;
            if (last != -1) {
                int vLast = (arrayMask >> last) & 1;
                int vCur = (arrayMask >> j) & 1;
                int next;
                // min 保留较小者，max 保留较大者
                if ((opParity == 0 && vLast > vCur) || (opParity == 1 && vLast < vCur)) {
                    next = state ^ (1 << last);
                } else {
                    next = state ^ (1 << j);
                }
                dp[next] = true;
            }
            last = j;
        }
    }
    return best;
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; ++i) scanf("%d", &a[i]);

    // 计数排序：按 (值, 下标) 升序处理；同一桶内下标天然升序
    static int cnt[MAXN + 1];
    for (int i = 1; i <= N; ++i) ++cnt[a[i]];
    bucketStart[1] = 1;
    for (int v = 1; v <= N; ++v) bucketStart[v + 1] = bucketStart[v] + cnt[v];
    static int fillPos[MAXN + 1];
    for (int v = 1; v <= N; ++v) fillPos[v] = bucketStart[v];
    for (int i = 1; i <= N; ++i) order[fillPos[a[i]]++] = i;

    for (int i = 0; i <= N + 1; ++i) {
        leftActive[i] = i - 1;
        rightActive[i] = i + 1;
    }

    // 统一按"奇数长度取最大值、偶数长度取次大值"计数，
    // 之后对长度为 3、4、6 的窗口做差量修正。
    for (int v = 1; v <= N; ++v) {
        for (int k = bucketStart[v]; k < bucketStart[v + 1]; ++k) {
            int i = order[k];
            answer += (long long)v * (CountOddFirst(i) + CountEvenSecond(i));
            // 处理完 i 后把它从"活跃"集合中删除（双向链表）
            int lo = leftActive[i], hi = rightActive[i];
            rightActive[lo] = hi;
            leftActive[hi] = lo;
        }
    }

    // 长度 1、2 以及 >=5 的奇数长度、>=8 的偶数长度上公式都成立，
    // 只有 3、4、6 需要修正（结论来自官方题解的小规模枚举 + 归纳）。
    static bool preTrue[7][64];
    const int smallLens[3] = {3, 4, 6};
    for (int t = 0; t < 3; ++t) {
        int len = smallLens[t];
        for (int mask = 0; mask < (1 << len); ++mask) {
            preTrue[len][mask] = (SolveZeroOne(len, mask) == 1);
        }
    }
    for (int t = 0; t < 3; ++t) {
        int len = smallLens[t];
        for (int i = 1; i + len - 1 <= N; ++i) {
            // 公式给出的值：奇数长度取最大值，偶数长度取次大值
            long long formula;
            if (len & 1) {
                int mx = 0;
                for (int j = 0; j < len; ++j) mx = max(mx, a[i + j]);
                formula = mx;
            } else {
                int mx = 0, smx = 0;
                for (int j = 0; j < len; ++j) {
                    if (a[i + j] > mx) {
                        smx = mx;
                        mx = a[i + j];
                    } else if (a[i + j] > smx) {
                        smx = a[i + j];
                    }
                }
                formula = smx;
            }
            // 真实答案：按值从大到小逐个标记为 1，第一个使 0-1 游戏为 1 的值
            //（0-1 原理：全部值整体抬高/压低不影响游戏的单调结构）。
            int pos[6];
            for (int j = 0; j < len; ++j) pos[j] = j;
            for (int x = 1; x < len; ++x) {
                int key = pos[x], y = x - 1;
                while (y >= 0 && a[i + pos[y]] < a[i + key]) {
                    pos[y + 1] = pos[y];
                    --y;
                }
                pos[y + 1] = key;
            }
            int mask = 0;
            long long realAnswer = 0;
            for (int j = 0; j < len; ++j) {
                mask |= 1 << pos[j];
                if (preTrue[len][mask]) {
                    realAnswer = a[i + pos[j]];
                    break;
                }
            }
            answer += realAnswer - formula;
        }
    }

    printf("%lld\n", answer);
    return 0;
}
