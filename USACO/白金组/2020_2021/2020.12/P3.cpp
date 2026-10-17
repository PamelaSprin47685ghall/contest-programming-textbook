#include <cstdio>
#include <vector>

using namespace std;

const int MOD = 1000000007;        // 答案取模
const long long INV6 = 166666668;  // 6 在模 1e9+7 下的逆元，用来算 C(x,3)

// 一段闭区间 [low, high]
struct Interval {
    long long low;
    long long high;
};

// 一个盒子落在某个大小为 blockSize 的块里的那部分
struct Piece {
    long long block;  // 块号
    long long low;    // 块内区间的左端点
    long long high;   // 块内区间的右端点
};

long long answer = 0;
long long limit = 0;      // 阈值：混合物合法要求两两异或 < limit
long long blockSize = 1;  // 比 limit 大的最小 2 的幂

// C(x,2) 模 1e9+7（x 一定小于模数）
long long choose2(long long x) {
    return x % MOD * ((x - 1 + MOD) % MOD) % MOD * 500000004 % MOD;
}

// C(x,3) 模 1e9+7（x 一定小于模数）
long long choose3(long long x) {
    return x % MOD * ((x - 1 + MOD) % MOD) % MOD * ((x - 2 + MOD) % MOD) % MOD * INV6 % MOD;
}

// 一组互不相交区间的总长度，不超过 blockSize <= 2^30，取模后依然精确
long long totalLength(const vector<Interval> &v) {
    long long len = 0;
    for (size_t i = 0; i < v.size(); ++i) len += v[i].high - v[i].low + 1;
    return len % MOD;
}

// 把 [low, high] 与 [0, half) 求交后放进 v；交为空就不放
void addClamped(vector<Interval> &v, long long low, long long high, long long half) {
    if (low < 0) low = 0;
    if (high > half - 1) high = half - 1;
    if (low > high) return;
    Interval piece;
    piece.low = low;
    piece.high = high;
    v.push_back(piece);
}

// a、b 都是 [0, block) 内若干互不相交区间的并，且保证 limit < block。
// 对 a 中的每个 x，向答案累加 C(cur + #{y 属于 b 且 x^y < limit}, 2)，
// 其中 cur 是已经确定"对 a 中每个 x 都合法"的 y 的个数
void addToAnswer(const vector<Interval> &a, const vector<Interval> &b,
                 long long block, long long cur) {
    if (a.empty()) return;
    if (b.empty()) {
        // b 里没有 y 了：从之前攒下的 cur 个 y 里任取两个，都能和每个 x 配成混合物
        answer = (answer + totalLength(a) * choose2(cur)) % MOD;
        return;
    }
    if (b.size() == 1 && b[0].low == 0 && b[0].high == block - 1) {
        // b 正好是整个 block：固定 x 时映射 y -> x^y 是 [0,block) 上的一一对应，
        // 所以满足 x^y < limit 的 y 恰好有 limit mod block 个
        cur += (block - 1) & limit;
        answer = (answer + totalLength(a) * choose2(cur)) % MOD;
        return;
    }
    long long half = block / 2;
    vector<Interval> lowerA, upperA, lowerB, upperB;
    for (size_t i = 0; i < a.size(); ++i) {
        addClamped(lowerA, a[i].low, a[i].high, half);
        addClamped(upperA, a[i].low - half, a[i].high - half, half);
    }
    for (size_t i = 0; i < b.size(); ++i) {
        addClamped(lowerB, b[i].low, b[i].high, half);
        addClamped(upperB, b[i].low - half, b[i].high - half, half);
    }
    if (limit & half) {
        // limit 在这一位上为 1：x、y 同在半边时异或更小，一定合法，直接记进 cur；
        // 分处两边时异或的最高位固定是 half，只需继续比较更低的位
        addToAnswer(lowerA, upperB, half, cur + totalLength(lowerB));
        addToAnswer(upperA, lowerB, half, cur + totalLength(upperB));
    } else {
        // limit 在这一位上为 0：分处两边的异或已经 >= half > limit，一定不合法；
        // 同在半边才继续往低位看
        addToAnswer(lowerA, lowerB, half, cur);
        addToAnswer(upperA, upperB, half, cur);
    }
}

// 处理落在同一个大小为 blockSize 的块里的所有残片
void dealBlock(const vector<Interval> &pieces) {
    long long half = blockSize / 2;
    vector<Interval> halfPart[2];  // 按高位分成下半块、上半块，坐标各自平移到 [0, half)
    for (size_t i = 0; i < pieces.size(); ++i) {
        if (pieces[i].low / half < pieces[i].high / half) {
            addClamped(halfPart[0], pieces[i].low, half - 1, half);
            addClamped(halfPart[1], pieces[i].low - half, pieces[i].high - half, half);
        } else {
            int side = (int)(pieces[i].low / half);
            addClamped(halfPart[side], pieces[i].low % half, pieces[i].high % half, half);
        }
    }
    for (int i = 0; i < 2; ++i) {
        // 三个数都在同一半：两两异或 < half <= limit，全部合法
        answer = (answer + choose3(totalLength(halfPart[i]))) % MOD;
        // 一个数在半 i、另外两个在半 i^1：同一半的两个必然合法，只需检查另外两个异或
        addToAnswer(halfPart[i], halfPart[i ^ 1], half, 0);
    }
}

int main() {
    int n;
    scanf("%d %lld", &n, &limit);
    ++limit;                                    // 先把 a^b <= K 化成 a^b < limit
    while (blockSize <= limit) blockSize *= 2;  // 取比 limit 大的最小 2 的幂
    limit -= blockSize / 2;                     // 0 <= limit < blockSize/2

    // 一个被完整覆盖的块 [t*blockSize, (t+1)*blockSize) 的答案：
    // 同一半任取三个（两个半各一组），加上"一个在一半、两个在另一半"的 blockSize*C(limit,2) 个
    long long wholeBlock = (blockSize % MOD * choose2(limit) % MOD +
                            2 * choose3(blockSize / 2) % MOD) % MOD;

    vector<Piece> pieces;
    for (int i = 0; i < n; ++i) {
        long long l, r;
        scanf("%lld %lld", &l, &r);
        long long leftBlock = l / blockSize;
        long long rightBlock = r / blockSize;
        if (leftBlock != rightBlock) {
            // 中间那些块被整个盒子盖住，可以直接套公式
            long long middle = (rightBlock - leftBlock - 1) % MOD;
            answer = (answer + middle * wholeBlock) % MOD;
            Piece head, tail;
            head.block = leftBlock;
            head.low = l % blockSize;
            head.high = blockSize - 1;
            tail.block = rightBlock;
            tail.low = 0;
            tail.high = r % blockSize;
            pieces.push_back(head);
            pieces.push_back(tail);
        } else {
            Piece only;
            only.block = leftBlock;
            only.low = l % blockSize;
            only.high = r % blockSize;
            pieces.push_back(only);
        }
    }

    // 盒子按内容递增给出，残片因此天然按块号排好序，扫一遍分组即可
    size_t start = 0;
    while (start < pieces.size()) {
        size_t end = start;
        vector<Interval> group;
        while (end < pieces.size() && pieces[end].block == pieces[start].block) {
            Interval iv;
            iv.low = pieces[end].low;
            iv.high = pieces[end].high;
            group.push_back(iv);
            ++end;
        }
        dealBlock(group);
        start = end;
    }
    printf("%lld\n", answer);
    return 0;
}
