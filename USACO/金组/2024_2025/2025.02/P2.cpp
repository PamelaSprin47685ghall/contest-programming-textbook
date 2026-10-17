#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXM = 200005;            // 更新/查询条数上限
const int MAXP = 4 * MAXM + 10;     // 关键位置上限: 每个更新/查询各贡献 2 个
const ll MOD = 1000000007;

int n, m, q;
int pos[MAXP];              // 关键位置, 升序去重; 含 0, n+1 及每个 l 和 r+1
int cntP = 0;
int updL[MAXM], updR[MAXM];
int askL[MAXM], askR[MAXM], askK[MAXM];
ll ones[MAXP];              // ones[t]: 位置 1..pos[t]-1 中 1 的个数
ll pre[MAXP];               // pre[t]: s[1..pos[t]-1] 作为二进制数的值, 最低位是 pos[t]-1
int bit[MAXP];              // bit[t]: 位置 pos[t] 所在整段的字符值(段内全相同)
int delta[MAXP];            // 异或差分标记, 用来还原每段的字符

ll qpow(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) res = res * base % MOD;
        base = base * base % MOD;
        exp >>= 1;
    }
    return res;
}

// 二分找出 x 在 pos 中的下标, 保证 x 一定是关键位置
int findIdx(int x) {
    int lo = 0, hi = cntP - 1;
    while (lo < hi) {
        int mid = (lo + hi) >> 1;
        if (pos[mid] < x) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

int main() {
    scanf("%d %d %d", &n, &m, &q);

    // 两两翻转区间最多把字符串切成 O(m+q) 段, 段内字符相同,
    // 所以只需要在 l 与 r+1 这些"切点"上记录前缀信息
    pos[cntP++] = 0;
    pos[cntP++] = n + 1;
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &updL[i], &updR[i]);
        pos[cntP++] = updL[i];
        pos[cntP++] = updR[i] + 1;
    }
    for (int i = 0; i < q; i++) {
        scanf("%d %d %d", &askL[i], &askR[i], &askK[i]);
        pos[cntP++] = askL[i];
        pos[cntP++] = askR[i] + 1;
    }
    sort(pos, pos + cntP);
    cntP = unique(pos, pos + cntP) - pos;

    for (int i = 0; i < m; i++) {
        delta[findIdx(updL[i])] ^= 1;
        delta[findIdx(updR[i] + 1)] ^= 1;
    }

    int cur = 0;
    for (int t = 0; t < cntP; t++) {
        cur ^= delta[t];
        bit[t] = cur;
        if (t > 0) {
            // 段 [pos[t-1], pos[t]) 内字符全是 bit[t-1]
            ll len = pos[t] - pos[t - 1];
            ll p2 = qpow(2, len);
            ones[t] = ones[t - 1] + (ll)bit[t - 1] * len;
            // 接上一段全 1 的 len 位, 等价于左移 len 位再补上 2^len - 1
            pre[t] = (pre[t - 1] * p2 + bit[t - 1] * (p2 - 1)) % MOD;
        }
    }

    for (int i = 0; i < q; i++) {
        int l = askL[i], r = askR[i], k = askK[i];
        int gl = findIdx(l), gr = findIdx(r + 1);
        ll sz = r - l + 1;
        ll inOnes = ones[gr] - ones[gl];        // s[l..r] 中 1 的个数
        ll zeros = sz - inOnes;

        // 贪心: 先删 0 再删 1 最优; 要删 sz-k 个字符, 若 0 不够删就只能全 1
        if (zeros <= sz - k) {
            printf("%lld\n", (qpow(2, k) - 1 + MOD) % MOD);
            continue;
        }

        ll zi = sz - k + 1;     // 删掉前 sz-k 个 0, 保留的第 1 个 0 就是第 zi 个 0
        // 二分: 找最靠右的关键位置 t (pos[t] <= r) 使 [l, pos[t]-1] 中 0 的个数少于 zi
        int lo = gl, hi = gr - 1;
        while (lo < hi) {
            int mid = (lo + hi + 1) >> 1;
            ll zBefore = (ll)(pos[mid] - l) - (ones[mid] - ones[gl]);
            if (zBefore < zi) lo = mid;
            else hi = mid - 1;
        }
        int t = lo;
        ll zBefore = (ll)(pos[t] - l) - (ones[t] - ones[gl]);
        int p = pos[t] + (int)(zi - zBefore) - 1;   // 第 zi 个 0 的真实位置

        ll keepOnes = (ll)(p - l) - (zi - 1);       // [l, p-1] 中 1 的个数
        // 答案 = keepOnes 个 1 接上 s[p..r], 总长度 keepOnes + (r-p+1) = k
        ll front = (qpow(2, k) - qpow(2, k - keepOnes) + MOD) % MOD;
        // s[p..r] 的值 = pre[r] - pre[p-1] * 2^(r-p+1),
        // 而 [pos[t], p-1] 整段都是 0, 故 pre[p-1] = pre[t] * 2^(p-pos[t])
        ll back = (pre[gr] - pre[t] * qpow(2, p - pos[t]) % MOD * qpow(2, r - p + 1)) % MOD;
        back = (back + MOD) % MOD;

        printf("%lld\n", (front + back) % MOD);
    }
    return 0;
}
