#include <cstdio>

using namespace std;

const int MAXN = 2000;
const int WORDS = MAXN / 64 + 1;  // 每个点用 WORDS 个 64 位整数存"我能看见右边哪些点"

int n;
long long height[MAXN + 5];

// seen[i] 的第 j 位为 1 表示山峰 i 能看见 j（只记录 i 左边的点，保证每对只数一次）。
// 用位图而不用 set/vector：每次更新要对 O(N) 个 i 做"查前驱/查后继/连续删除"，
// 位图把这些操作压到 O(N/64)，2000 * 2000 的规模才不会超时。
unsigned long long seen[MAXN + 5][WORDS];

void turnOn(int i, int j) {
    seen[i][j >> 6] |= 1ULL << (j & 63);
}

void turnOff(int i, int j) {
    seen[i][j >> 6] &= ~(1ULL << (j & 63));
}

bool isOn(int i, int j) {
    return (seen[i][j >> 6] >> (j & 63)) & 1ULL;
}

// 山峰 i 到山峰 k 的斜率是否不低于 i 到 a 的斜率（a、k 都在 i 右侧，分母为正，
// 所以可以交叉相乘，避免浮点误差）
bool notLowerSlope(int i, int a, int k) {
    return (height[k] - height[i]) * (a - i) >= (height[a] - height[i]) * (k - i);
}

// i 能看见的点里 <= limit 的最大编号，没有则返回 -1
int previousSeen(int i, int limit) {
    if (limit > n) {
        limit = n;
    }
    if (limit < 1) {
        return -1;
    }
    int w = limit >> 6;
    // 把 limit 所在整数里高于 limit 的位抹掉，只保留 <= limit 的部分
    unsigned long long word = seen[i][w];
    if ((limit & 63) != 63) {
        word &= (1ULL << ((limit & 63) + 1)) - 1;
    }
    while (true) {
        if (word != 0) {
            return (w << 6) + 63 - __builtin_clzll(word);  // 最高位的那个可见点
        }
        if (w == 0) {
            return -1;
        }
        word = seen[i][--w];
    }
}

// i 能看见的点里 >= limit 的最小编号，没有则返回 -1
int nextSeen(int i, int limit) {
    if (limit > n) {
        return -1;
    }
    if (limit < 1) {
        limit = 1;
    }
    int w = limit >> 6;
    // 抹掉低于 limit 的位，只保留 >= limit 的部分
    unsigned long long word = seen[i][w] & (~0ULL << (limit & 63));
    while (true) {
        if (word != 0) {
            return (w << 6) + __builtin_ctzll(word);  // 最低位的那个可见点
        }
        if (w + 1 >= WORDS) {
            return -1;
        }
        word = seen[i][++w];
    }
}

int countSeen(int i) {
    int total = 0;
    for (int w = 0; w < WORDS; ++w) {
        total += __builtin_popcountll(seen[i][w]);
    }
    return total;
}

// 从 i 出发向右扫描：上一个可见点 last 到 i 的斜率是所有已知点里最大的，
// 因此 j 可见当且仅当 i->j 的斜率不低于 i->last 的斜率（相等说明三点共线，也不算被挡住）
void buildRow(int i) {
    for (int w = 0; w < WORDS; ++w) {
        seen[i][w] = 0;
    }
    int last = -1;
    for (int j = i + 1; j <= n; ++j) {
        if (last == -1 || notLowerSlope(i, last, j)) {
            turnOn(i, j);
            last = j;
        }
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%lld", &height[i]);
    }
    int answer = 0;
    for (int i = 1; i <= n; ++i) {
        buildRow(i);
        answer += countSeen(i);
    }

    int q;
    scanf("%d", &q);
    for (int t = 0; t < q; ++t) {
        int x;
        long long rise;
        scanf("%d %lld", &x, &rise);
        height[x] += rise;

        // 只有 x 左边的点向右看时才会受影响：它们到 x 的斜率变大，
        // 于是 x 可能新变可见，而 x 右边的点可能被挡掉
        for (int i = 1; i < x; ++i) {
            int prev = previousSeen(i, x - 1);
            // prev 是 x 左边最陡的可见点，所以只要 x 的斜率不低于它就说明 x 可见；
            // prev == -1 意味着 x 就是 i 右边的第一座山
            if (prev != -1 && !notLowerSlope(i, prev, x)) {
                continue;
            }
            if (!isOn(i, x)) {
                turnOn(i, x);
                ++answer;
            }
            // x 变高后，斜率比 x 小的可见点都被它挡住了。可见点的斜率是递增的，
            // 所以一旦遇到斜率不低于 x 的点就可以停手（它和更远的点都还看得见）
            int j = nextSeen(i, x + 1);
            while (j != -1 && !notLowerSlope(i, x, j)) {
                turnOff(i, j);
                --answer;
                j = nextSeen(i, j + 1);
            }
        }

        // x 自己向右的可见表直接重建：它变高后能看见的只会更多
        answer -= countSeen(x);
        buildRow(x);
        answer += countSeen(x);

        printf("%d\n", answer);
    }
    return 0;
}
