#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

using namespace std;

const long long MOD = 1000000007;
const int MAXN = 2005;

int testCount, n;
int perm[MAXN];                // perm[i] = 约束 p_i = perm[i]；-1 表示该位置没有约束
long long factorial[MAXN];     // 阶乘
long long invFactorial[MAXN];  // 阶乘的逆元

struct Item {
    int value;  // 把它归到"大元素"一侧后的编号
    int pos;    // 它在排列中的下标
};
Item item[MAXN];
int itemCount;

// 按（编号, 下标）排序，与官方 sort(pair) 的顺序完全一致
bool CmpItem(const Item &a, const Item &b) {
    if (a.value != b.value) return a.value < b.value;
    return a.pos < b.pos;
}

// 快速幂；递归形式与官方 po 相同，指数最大只有 10^9+5 量级，深度约 30
long long Power(long long base, long long exp) {
    if (exp == 0) return 1;
    long long half = Power(base, exp / 2);
    half = half * half % MOD;
    if (exp % 2 == 1) half = half * base % MOD;
    return half;
}

// 组合数；越界一律视为 0，这样调用处不用再判边界
long long Comb(int total, int choose) {
    if (choose > total || choose < 0) return 0;
    return factorial[total] * invFactorial[choose] % MOD * invFactorial[total - choose] % MOD;
}

// 把约束整体镜像：值 x 变成 n-1-x。
// 排列取镜像后"最小元素在左、最大元素在右"与"最大在左、最小在右"互换，
// 于是两种对称情形可以共用同一段计数代码。
void MirrorConstraints() {
    for (int i = 0; i < n; ++i) {
        if (perm[i] != -1) perm[i] = n - 1 - perm[i];
    }
}

// 情形一：中间元素 (n-1)/2 固定在某个端点上。
// 剩下的区间 [l, r] 里，偶数下标放"大元素"（> n/2），奇数下标放"小元素"，
// 大元素 value 和它的小搭档 value-n/2 必须一起放下。
// smallestBig 是还没放置的最小大元素：它只能紧挨着已放好的部分出现，
// 所以每个"未放置的大元素"要么整批放在区间左端、要么放在右端。
// 约束只是这条填充路径上的必经点，路径是格点游走，方案数就是若干组合数之积。
long long CountFromEndpoint(int l, int r, int smallestBig) {
    itemCount = 0;
    for (int i = l; i <= r; ++i) {
        if (perm[i] == -1) continue;
        if (perm[i] == n / 2) return 0;  // 中间元素不在这个区间里
        if (i & 1) {
            // 奇数下标是小元素，它只能由搭档 value+n/2 带进来
            if (perm[i] > n / 2) return 0;
            if (perm[i] >= smallestBig - n / 2) return 0;
            item[itemCount].value = perm[i] + n / 2;
            item[itemCount].pos = i;
            ++itemCount;
        } else {
            // 偶数下标是大元素
            if (perm[i] < n / 2) return 0;
            if (perm[i] > smallestBig) return 0;
            item[itemCount].value = perm[i];
            item[itemCount].pos = i;
            ++itemCount;
        }
    }
    sort(item, item + itemCount, CmpItem);
    if (itemCount == 0) return Power(2, (r - l) / 2);  // 每个数对都可自由选左右
    long long ways = 1;
    int curr = n / 2;  // 已经放置到的最小大元素
    for (int i = 0; i + 1 < itemCount; ++i) {
        if (item[i].value == item[i + 1].value) {
            // 同一个 value 的两个端点都被约束钉住：它们必须相邻且构成一个数对
            if (abs(item[i + 1].pos - item[i].pos) != 1) return 0;
            if (item[i].pos % 2 == l % 2) {
                int num = item[i].value - curr, s = (item[i].pos - l) / 2 + 1;
                ways = ways * Comb(num - 1, s - 1) % MOD;
                curr = item[i].value, l += 2 * s, r -= 2 * (num - s);
            } else {
                int num = item[i].value - curr, s = (r - item[i].pos) / 2 + 1;
                ways = ways * Comb(num - 1, s - 1) % MOD;
                curr = item[i].value, l += 2 * (num - s), r -= 2 * s;
            }
            ++i;
            if (i == itemCount - 1) return ways * Power(2, (r - l) / 2) % MOD;
            continue;
        }
        if (item[i].pos < l || item[i].pos > r) return 0;
        if (item[i + 1].pos > item[i].pos) {
            int num = item[i].value - curr, s = (item[i].pos - l) / 2 + 1;
            ways = ways * Comb(num - 1, s - 1) % MOD;
            curr = item[i].value, l += 2 * s, r -= 2 * (num - s);
        } else {
            int num = item[i].value - curr, s = (r - item[i].pos) / 2 + 1;
            ways = ways * Comb(num - 1, s - 1) % MOD;
            curr = item[i].value, l += 2 * (num - s), r -= 2 * s;
        }
    }
    if (item[itemCount - 1].pos > r || item[itemCount - 1].pos < l) return 0;
    if (item[itemCount - 1].value == smallestBig)
        return ways * Comb((r - l) / 2, (item[itemCount - 1].pos - l) / 2) % MOD;
    ways = ways * (Comb(item[itemCount - 1].value - curr - 1, (item[itemCount - 1].pos - l) / 2)
                 + Comb(item[itemCount - 1].value - curr - 1, (r - item[itemCount - 1].pos) / 2)) % MOD;
    return ways * Power(2, smallestBig - item[itemCount - 1].value - 1) % MOD;
}

// 情形二：中间元素在内部位置 centerPos 上，且 centerPos 是奇数。
// 左边 [l, centerPos-2] 是"大元素区间"，右边自 r = centerPos+2 起向右长成"小元素区间"，
// 它们各自从中间向外、按 大元素与其小搭档成对 的方式填充。
// 同一条填充路径上的约束同样只是必经点，所以也只需连乘组合数。
long long CountFromCenter(int l, int r, int centerPos) {
    itemCount = 0;
    for (int i = l; i < centerPos - 1; ++i) {
        if (perm[i] == -1) continue;
        if (perm[i] == n / 2) return 0;
        if (i % 2 == l % 2) {
            if (perm[i] < n / 2) return 0;
            item[itemCount].value = perm[i];
            item[itemCount].pos = i;
            ++itemCount;
        } else {
            if (perm[i] > n / 2) return 0;
            item[itemCount].value = perm[i] + n / 2;
            item[itemCount].pos = i;
            ++itemCount;
        }
    }
    for (int i = r; i < n; ++i) {
        if (perm[i] == -1) continue;
        if (perm[i] == n / 2) return 0;
        if (i % 2 == r % 2) {
            if (perm[i] < n / 2) return 0;
            item[itemCount].value = perm[i];
            item[itemCount].pos = i;
            ++itemCount;
        } else {
            if (perm[i] > n / 2) return 0;
            item[itemCount].value = perm[i] + n / 2;
            item[itemCount].pos = i;
            ++itemCount;
        }
    }
    sort(item, item + itemCount, CmpItem);
    long long ways = 1;
    int curr = n / 2;
    for (int i = 0; i < itemCount; ++i) {
        if (i > 0 && item[i].value == item[i - 1].value) {
            // 同一个 value 的两个约束必须是一对相邻的 (大, 小)，否则无解
            if (item[i].pos - item[i - 1].pos != 1) return 0;
            if (item[i].pos < centerPos) {
                if (item[i].pos % 2 == l % 2) return 0;
            } else {
                if (item[i].pos % 2 == r % 2) return 0;
            }
            continue;
        }
        if (item[i].pos < l || (item[i].pos >= centerPos && item[i].pos < r)) return 0;
        if (item[i].pos < centerPos) {
            int num = item[i].value - curr, s = (item[i].pos - l) / 2 + 1;
            ways = ways * Comb(num - 1, s - 1) % MOD;
            curr = item[i].value, l += 2 * s, r += 2 * (num - s);
        } else {
            int num = item[i].value - curr, s = (item[i].pos - r) / 2 + 1;
            ways = ways * Comb(num - 1, s - 1) % MOD;
            curr = item[i].value, l += 2 * (num - s), r += 2 * s;
        }
    }
    // 两个区间各剩若干空位，空位上的数对可以任意选左右：只剩一个组合数
    ways = ways * Comb((centerPos - 2 - l + 1) / 2 + (n - 1 - r + 1) / 2,
                       (centerPos - 2 - l + 1) / 2) % MOD;
    return ways;
}

void Solve() {
    int constraintCount;
    scanf("%d", &constraintCount);
    memset(perm, -1, sizeof(perm));
    for (int i = 0; i < constraintCount; ++i) {
        int index, value;
        scanf("%d %d", &index, &value);
        perm[index] = value;  // 题目保证同一个 i 不会重复出现
    }

    if (n % 2 == 0) {
        // 偶数 N 时最大最小差 N/2 的排列只有两个：交替排列本身和它的倒序。
        // 逐位检查这两条候选排列是否满足全部约束即可。
        int forward = 1, backward = 1;
        for (int i = 0; i < n; ++i) {
            if (perm[i] == -1) continue;
            int candidate = (i % 2 == 0) ? (n / 2 + i / 2) : (i / 2);
            int mirrored = ((n - 1 - i) % 2 == 0) ? (n / 2 + (n - 1 - i) / 2) : ((n - 1 - i) / 2);
            if (perm[i] != candidate) forward = 0;
            if (perm[i] != mirrored) backward = 0;
        }
        printf("%d\n", forward + backward);
        return;
    }

    long long answer = 0;
    int flipped = 0;
    do {
        // 中间元素放在端点 0 上，旁边是 0 或 n-1
        if (perm[0] == n / 2 || perm[0] == -1) {
            if (perm[1] == 0 || perm[1] == -1) answer = (answer + CountFromEndpoint(2, n - 1, n - 1)) % MOD;
            if (perm[1] == n - 1 || perm[1] == -1) {
                MirrorConstraints();
                answer = (answer + CountFromEndpoint(2, n - 1, n - 1)) % MOD;
                MirrorConstraints();
            }
        }
        for (int i = 1; i < n - 1; ++i) {
            if (perm[i] != n / 2 && perm[i] != -1) continue;
            if (perm[i - 1] != n - 1 && perm[i - 1] != -1) continue;
            if (perm[i + 1] != 0 && perm[i + 1] != -1) continue;
            // 固定 i 放中间元素、i-1 放 n-1、i+1 放 0：这是"n-1 在左"的那一半情形
            int hadLeft = perm[i - 1] == n - 1, hadCenter = perm[i] == n / 2, hadRight = perm[i + 1] == 0;
            perm[i - 1] = n - 1, perm[i] = n / 2, perm[i + 1] = 0;
            if (i % 2 == 1) {
                answer = (answer + CountFromCenter(0, i + 2, i)) % MOD;
            } else {
                // i 为偶数时左右两段被拆成"小元素段"和"大元素段"，分别计数再相乘
                long long rightWays = CountFromEndpoint(i + 2, n - 1, n - i / 2 - 1);
                MirrorConstraints();
                long long leftWays = CountFromEndpoint(0, i - 2, n / 2 + i / 2);
                MirrorConstraints();
                answer = (answer + rightWays * leftWays) % MOD;
            }
            if (!hadLeft) perm[i - 1] = -1;
            if (!hadCenter) perm[i] = -1;
            if (!hadRight) perm[i + 1] = -1;
        }
        // 把整个排列倒过来再算一遍，覆盖"0 在左、n-1 在右"的另一半情形
        reverse(perm, perm + n);
        flipped ^= 1;
    } while (flipped);
    printf("%lld\n", answer);
}

int main() {
    scanf("%d %d", &testCount, &n);
    factorial[0] = 1;
    for (int i = 1; i <= n; ++i) factorial[i] = factorial[i - 1] * i % MOD;
    invFactorial[n] = Power(factorial[n], MOD - 2);
    for (int i = n - 1; i >= 0; --i) invFactorial[i] = invFactorial[i + 1] * (i + 1) % MOD;
    while (testCount--) Solve();
    return 0;
}
