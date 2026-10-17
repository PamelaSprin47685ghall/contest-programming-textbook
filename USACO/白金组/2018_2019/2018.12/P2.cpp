#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000; // 奶牛数上限

int n;
long long k;
int cow[MAXN];     // cow[i]：位置 i 上的奶牛编号
int lis[MAXN];     // lis[i]：以位置 i 的奶牛开头的最长上升子序列长度
int levelOf[MAXN]; // lis[i] - 1：把"开头长度"相同的牛分到同一层
long long cap;     // 计数的封顶值 K + 1：比 K 大的计数无需区分，一律压成它，防止溢出

// 计数的"封顶加法"：x、y 都不超过 cap，加起来不会超出 64 位；结果同样封顶在 cap
long long cappedAdd(long long x, long long y) {
    long long sum = x + y;
    return sum < cap ? sum : cap;
}

// ---------- 求 lis[]：值域线段树，支持单点改大、区间取最大值 ----------
int maxLeaf;                 // 值域线段树的叶子个数，取不小于 n 的 2 的幂
int maxTree[2 * 131072 + 5]; // n 最大 10^5，2 的幂最多 131072

void updateMax(int value, int length) {
    int node = value + maxLeaf;
    maxTree[node] = length;
    for (node >>= 1; node >= 1; node >>= 1) {
        maxTree[node] = max(maxTree[node << 1], maxTree[(node << 1) | 1]);
    }
}

// 查询值域闭区间 [l, r] 上的最大值，区间为空时返回 0
int queryMax(int l, int r) {
    int res = 0;
    int left = l + maxLeaf;
    int right = r + maxLeaf + 1;
    while (left < right) {
        if (left & 1) {
            res = max(res, maxTree[left]);
            ++left;
        }
        if (right & 1) {
            --right;
            res = max(res, maxTree[right]);
        }
        left >>= 1;
        right >>= 1;
    }
    return res;
}

// ---------- 按层摊平：同一层（lis 相同）内位置越靠后值越小，故按值从小到大排列 ----------
int levelSize[MAXN + 1];    // 每层的元素个数
int levelStart[MAXN + 2];   // 每层在扁平数组中的起始下标
int levelValue[MAXN];       // 层内按值从小到大排列的奶牛编号
int levelPos[MAXN];         // 与 levelValue 一一对应的位置
long long levelCount[MAXN]; // 每个位置开头、长度恰为 lis[i] 的上升子序列条数（封顶 cap）
int indexInLevel[MAXN];     // 位置 i 在其所在层内的下标
int queryStart[MAXN + 1];   // 每层查询时"值大于当前元素"的起始下标，随扫描单调右移

// ---------- 各层专属的计数线段树，扁平存储，按层划分区间 ----------
int segLeaf[MAXN + 1];  // 每层树占用的叶子个数
int segOffset[MAXN + 1]; // 每层树在扁平数组中的起始下标
long long countTree[4 * MAXN + 5];

// 第 level 层内下标区间 [l, r) 上的条数之和（同样封顶）
long long queryCount(int level, int l, int r) {
    long long res = 0;
    int base = segOffset[level];
    int left = l + segLeaf[level];
    int right = r + segLeaf[level];
    while (left < right) {
        if (left & 1) {
            res = cappedAdd(res, countTree[base + left]);
            ++left;
        }
        if (right & 1) {
            --right;
            res = cappedAdd(res, countTree[base + right]);
        }
        left >>= 1;
        right >>= 1;
    }
    return res;
}

void setCount(int level, int idx, long long value) {
    int base = segOffset[level];
    int node = idx + segLeaf[level];
    countTree[base + node] = value;
    for (node >>= 1; node >= 1; node >>= 1) {
        countTree[base + node] = cappedAdd(countTree[base + (node << 1)], countTree[base + ((node << 1) | 1)]);
    }
}

int main() {
    scanf("%d", &n);
    scanf("%lld", &k);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &cow[i]);
    }
    cap = k + 1;

    maxLeaf = 1;
    while (maxLeaf < n) {
        maxLeaf <<= 1;
    }
    // 从右往左扫描：树里只有右侧的奶牛，正好要求"位置更靠后且值更大"
    for (int i = n - 1; i >= 0; --i) {
        lis[i] = queryMax(cow[i] + 1, n) + 1;
        updateMax(cow[i], lis[i]);
    }
    int longest = 1;
    for (int i = 0; i < n; ++i) {
        if (lis[i] > longest) {
            longest = lis[i];
        }
    }

    for (int i = 0; i < n; ++i) {
        levelOf[i] = lis[i] - 1;
        ++levelSize[levelOf[i]];
    }
    int total = 0;
    for (int v = 0; v < longest; ++v) {
        levelStart[v] = total;
        total += levelSize[v];
    }
    levelStart[longest] = total;

    // 位置从大到小依次落位：同一层内下标越小位置越大、值越小，于是每层内部按值递增
    int fillAt[MAXN + 1];
    for (int v = 0; v < longest; ++v) {
        fillAt[v] = levelStart[v];
    }
    for (int i = n - 1; i >= 0; --i) {
        int v = levelOf[i];
        indexInLevel[i] = fillAt[v] - levelStart[v];
        levelValue[fillAt[v]] = cow[i];
        levelPos[fillAt[v]] = i;
        ++fillAt[v];
    }

    int used = 0;
    for (int v = 0; v < longest; ++v) {
        int leaf = 1;
        while (leaf < levelSize[v]) {
            leaf <<= 1;
        }
        segLeaf[v] = leaf;
        segOffset[v] = used;
        used += 2 * leaf;
    }

    // levelCount[i] = sum of levelCount[j]，其中 j 在 i 后面、cow[j] > cow[i]、lis[j] = lis[i] - 1
    for (int i = n - 1; i >= 0; --i) {
        int v = levelOf[i];
        long long ways;
        if (v == 0) {
            ways = 1; // 长度为 1 的上升子序列只有它自己这一条
        } else {
            int base = levelStart[v - 1];
            int ptr = queryStart[v];
            // cow[i] 在本层内随位置递减而单调变大，所以阈值指针只会向右走
            while (ptr < levelSize[v - 1] && levelValue[base + ptr] < cow[i]) {
                ++ptr;
            }
            queryStart[v] = ptr;
            // 还没插入的位置在树里记 0，恰好把"位置更靠前"的候选自动排除掉
            ways = queryCount(v - 1, ptr, levelSize[v - 1]);
        }
        levelCount[levelStart[v] + indexInLevel[i]] = ways;
        setCount(v, indexInLevel[i], ways);
    }

    // 第 K 小的答案集合 = 第 K 大上升子序列的补集，先还原这条子序列
    bool inSequence[MAXN + 1];
    for (int id = 1; id <= n; ++id) {
        inSequence[id] = false;
    }
    long long rem = k;
    int prevPos = -1;
    for (int v = longest - 1; v >= 0; --v) {
        int base = levelStart[v];
        int size = levelSize[v];
        // 下一个元素的位置必须大于上一个选中的位置；层内位置递减，所以可选的是下标的一段前缀
        int low = 0, high = size;
        while (low < high) {
            int mid = (low + high) >> 1;
            if (levelPos[base + mid] > prevPos) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        // 层内下标越大值越大，从下标 low-1 往前走就是按值从大到小枚举；
        // 值大于上一个元素（即真正接得上的）候选排在最前面，它们的条数之和恰好是
        // 上一个元素开头的子序列总数，一定覆盖当前的 K
        int use = low - 1;
        while (levelCount[base + use] < rem) {
            rem -= levelCount[base + use];
            --use;
        }
        inSequence[levelValue[base + use]] = true;
        prevPos = levelPos[base + use];
    }

    printf("%d\n", n - longest);
    for (int id = 1; id <= n; ++id) {
        if (!inSequence[id]) {
            printf("%d\n", id);
        }
    }
    return 0;
}
