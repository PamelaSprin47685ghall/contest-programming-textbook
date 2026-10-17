#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 200005;
const int MAXNODE = 4500000;
const ll NEG_INF = -(1LL << 62);

int N, Q;

ll rawA[MAXN], rawB[MAXN]; // 原始输入
int idx[MAXN];             // 排序用下标
int rankOf[MAXN];          // 原问题 -> b 的排名

// 按 a_i + b_i 升序排序后的问题序列
ll a[MAXN], b[MAXN];
ll prefA[MAXN];   // 前缀 a 之和
int bRank[MAXN];  // 排序后第 i 个问题的 b 排名
ll sortedB[MAXN]; // 排名 -> b 的值

int rootVersion[MAXN]; // 可持久化线段树的各版本根

// 可持久化线段树（值域为 b 的排名，维护个数与和）
int leftChild[MAXNODE], rightChild[MAXNODE], nodeCount[MAXNODE];
ll nodeSum[MAXNODE];
int nodeUsed = 0;

ll ans[MAXN]; // ans[k]：恰好"Elsie 可以改 k 道题"时的最优分数

bool CmpB(int x, int y) { return rawB[x] < rawB[y]; }

bool CmpSumAB(int x, int y) { return rawA[x] + rawB[x] < rawA[y] + rawB[y]; }

int Build(int l, int r) {
    int id = ++nodeUsed;
    nodeCount[id] = 0;
    nodeSum[id] = 0;
    leftChild[id] = rightChild[id] = 0;
    if (l != r) {
        int mid = (l + r) >> 1;
        leftChild[id] = Build(l, mid);
        rightChild[id] = Build(mid + 1, r);
    }
    return id;
}

int Update(int prev, int l, int r, int pos, ll val) {
    int id = ++nodeUsed;
    leftChild[id] = leftChild[prev];
    rightChild[id] = rightChild[prev];
    nodeCount[id] = nodeCount[prev] + 1;
    nodeSum[id] = nodeSum[prev] + val;
    if (l != r) {
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            leftChild[id] = Update(leftChild[prev], l, mid, pos, val);
        } else {
            rightChild[id] = Update(rightChild[prev], mid + 1, r, pos, val);
        }
    }
    return id;
}

// 版本差 newId - oldId（即一段后缀）中，最小的 k 个 b 之和
ll KMinSum(int oldId, int newId, int k) {
    ll res = 0;
    int l = 1, r = N;
    while (k > 0) {
        if (l == r) {
            res += (ll)k * sortedB[l];
            break;
        }
        int mid = (l + r) >> 1;
        int oldLeft = leftChild[oldId], newLeft = leftChild[newId];
        int cntLeft = nodeCount[newLeft] - nodeCount[oldLeft];
        if (cntLeft >= k) {
            oldId = oldLeft;
            newId = newLeft;
            r = mid;
        } else {
            res += nodeSum[newLeft] - nodeSum[oldLeft];
            k -= cntLeft;
            oldId = rightChild[oldId];
            newId = rightChild[newId];
            l = mid + 1;
        }
    }
    return res;
}

// 分治：已知 k 在 [kLo, kHi] 时的最优前缀长度 p 落在 [cutLo, cutHi]。
// 由最优前缀长度关于 k 的单调性（平局取更大的 p），先算中间 k 的最优 p，
// 再据此收缩两侧范围。
void Divide(int kLo, int kHi, int cutLo, int cutHi) {
    if (kLo > kHi) return;
    int k = (kLo + kHi) >> 1;
    int hiLimit = min(cutHi, N - k);
    ll best = NEG_INF;
    int bestCut = cutLo;
    for (int p = cutLo; p <= hiLimit; ++p) {
        // 分数 = 前缀 a 之和 - 后缀里最小的 k 个 b 之和
        ll score = prefA[p] - KMinSum(rootVersion[p], rootVersion[N], k);
        if (score >= best) {
            best = score;
            bestCut = p;
        }
    }
    ans[k] = best;
    Divide(kLo, k - 1, bestCut, cutHi);
    Divide(k + 1, kHi, cutLo, bestCut);
}

int main() {
    scanf("%d %d", &N, &Q);
    for (int i = 1; i <= N; ++i) scanf("%lld %lld", &rawA[i], &rawB[i]);

    // 先按 b 排序得到每个问题的 b 排名
    for (int i = 1; i <= N; ++i) idx[i] = i;
    sort(idx + 1, idx + N + 1, CmpB);
    for (int r = 1; r <= N; ++r) {
        rankOf[idx[r]] = r;
        sortedB[r] = rawB[idx[r]];
    }

    // 再按 a + b 升序排序，得到"前缀纯赚、后缀被改"的划分顺序
    for (int i = 1; i <= N; ++i) idx[i] = i;
    sort(idx + 1, idx + N + 1, CmpSumAB);
    for (int i = 1; i <= N; ++i) {
        a[i] = rawA[idx[i]];
        b[i] = rawB[idx[i]];
        bRank[i] = rankOf[idx[i]];
        prefA[i] = prefA[i - 1] + a[i];
    }

    // 版本 i 包含排序后前 i 个问题的 b
    rootVersion[0] = Build(1, N);
    for (int i = 1; i <= N; ++i) {
        rootVersion[i] = Update(rootVersion[i - 1], 1, N, bRank[i], b[i]);
    }

    Divide(0, N, 0, N);

    while (Q--) {
        int k;
        scanf("%d", &k);
        printf("%lld\n", ans[k]);
    }
    return 0;
}
