#include <cstdio>

using namespace std;

const int MOD = 1000000007;
const int MAXS = 1 << 20;  // 2^K 的上界（K ≤ 20）
const int MAXN = 500005;
const int MAXK = 20;

int N, K;
int cowValue[MAXN];    // 每头奶牛的位串
int bitCount[MAXS];    // 每个下标的二进制 1 的个数
int cnt[MAXS];         // cnt[i]：位串 i 出现的奶牛头数
int sizeCnt[MAXS];     // cnt[i] * popcount(i)
int coef[MAXS];        // 容斥系数 g(U)
int firstSum[MAXS];    // sum_B 1/|A∪B|
int secondSum[MAXS];   // sum_B |B|/|A∪B|
int comb[MAXK + 1][MAXK + 1];
int inv[MAXK + 1];     // 1..K 的逆元
int gprime[MAXK + 1];  // g(U) 只和 |U| 有关，这是按大小存的表

void BuildBitCounts(int size) {
    bitCount[0] = 0;
    for (int i = 1; i < size; ++i) {
        bitCount[i] = bitCount[i >> 1] + (i & 1);
    }
}

void BuildComb() {
    for (int i = 0; i <= K; ++i) {
        comb[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            comb[i][j] = (comb[i - 1][j - 1] + comb[i - 1][j]) % MOD;
        }
    }
}

void BuildInverses() {
    // 逆元递推式 inv[i] = -(MOD/i) * inv[MOD%i]（取模后）
    inv[1] = 1;
    for (int i = 2; i <= K; ++i) {
        inv[i] = (long long)(MOD - MOD / i) * inv[MOD % i] % MOD;
    }
}

// 我们要给每个非空集合 U 定一个系数 g(U) = gprime[|U|]，
// 使得对任意非空集合 S 都有 sum_{U ⊇ S} g(U) = 1/|S|。
// 按 |S| 展开：sum_{k=|S|}^{K} C(K-|S|, k-|S|) * gprime[k] = 1/|S|，
// 于是从 k = K 往小递推即可。
void BuildCoef() {
    for (int m = K; m >= 1; --m) {
        long long value = inv[m];
        for (int k = m + 1; k <= K; ++k) {
            value -= (long long)comb[K - m][k - m] * gprime[k] % MOD;
        }
        value %= MOD;
        if (value < 0) value += MOD;
        gprime[m] = (int)value;
    }
    coef[0] = 0;  // 空串不会出现，取 0 即可
    for (int i = 1; i < (1 << K); ++i) {
        coef[i] = gprime[bitCount[i]];
    }
}

// 处理后 sum 满足 sum[i] = 原数组在 i 的所有子集上的和
void SubsetSum(int *sum, int size) {
    for (int bit = 0; bit < K; ++bit) {
        for (int i = 0; i < size; ++i) {
            if (i & (1 << bit)) {
                sum[i] += sum[i ^ (1 << bit)];
                if (sum[i] >= MOD) sum[i] -= MOD;
            }
        }
    }
}

// 处理后 sum 满足 sum[i] = 原数组在 i 的所有超集上的和
void SupersetSum(int *sum, int size) {
    for (int bit = 0; bit < K; ++bit) {
        for (int i = 0; i < size; ++i) {
            if (!(i & (1 << bit))) {
                sum[i] += sum[i ^ (1 << bit)];
                if (sum[i] >= MOD) sum[i] -= MOD;
            }
        }
    }
}

// 对权重数组 weight，计算 weightSum[A] = sum_B weight[B] / |A∪B|。
// 做法：先做子集和得到 f(U) = sum_{B ⊆ U} weight[B]；
// 再乘上系数 g(U) 后做超集和，展开后恰好等于上式。
void WeightedUnionSum(int *weight, int size) {
    SubsetSum(weight, size);
    for (int i = 0; i < size; ++i) {
        weight[i] = (long long)weight[i] * coef[i] % MOD;
    }
    SupersetSum(weight, size);
}

int main() {
    scanf("%d %d", &N, &K);
    int size = 1 << K;
    BuildBitCounts(size);
    for (int i = 0; i < N; ++i) {
        int value;
        scanf("%d", &value);
        cowValue[i] = value;
        cnt[value] += 1;
    }
    for (int i = 1; i < size; ++i) {
        sizeCnt[i] = cnt[i] * bitCount[i];
    }
    BuildComb();
    BuildInverses();
    BuildCoef();

    // |A∩B|/|A∪B| = |A|/|A∪B| + |B|/|A∪B| - 1，
    // 所以第一项用所有奶牛当权重，第二项用 |B| 当权重。
    for (int i = 0; i < size; ++i) {
        firstSum[i] = cnt[i];
    }
    WeightedUnionSum(firstSum, size);
    for (int i = 0; i < size; ++i) {
        secondSum[i] = sizeCnt[i];
    }
    WeightedUnionSum(secondSum, size);

    for (int i = 0; i < N; ++i) {
        int value = cowValue[i];
        long long answer = (long long)bitCount[value] * firstSum[value] % MOD;
        answer = (answer + secondSum[value] - N) % MOD;
        if (answer < 0) answer += MOD;
        printf("%d\n", (int)answer);
    }
    return 0;
}
