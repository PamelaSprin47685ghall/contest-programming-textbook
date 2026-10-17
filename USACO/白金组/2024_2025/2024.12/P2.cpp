#include <cstdio>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 300005;
const long long INF = 1000000000000000000LL;

int L, N;
char str[MAXN];
int changeCost[MAXN];
long long mooCost[MAXN];  // mooCost[i]：让以 i 开头的 moo 成立所需的最小修改代价

void ComputeMooCosts() {
    for (int i = 0; i + L <= N; ++i) {
        long long cost = 0;
        for (int j = 0; j < L; ++j) {
            char want = (j == 0) ? 'M' : 'O';
            if (str[i + j] != want) {
                cost += changeCost[i + j];
            }
        }
        mooCost[i] = cost;
    }
}

// L = 1：每个 moo 只占一个字符，各个位置互相独立，
// 把已经是 M 的位置代价看成 0，从小到大取前 k 个即可。
void SolveSingle() {
    sort(mooCost, mooCost + N);
    long long total = 0;
    for (int i = 0; i < N; ++i) {
        total += mooCost[i];
        printf("%lld\n", total);
    }
}

// L = 2：每个 moo 占相邻两个位置，于是要选出 k 个互不相邻的起点。
// 经典做法：先取当前代价最小的位置，再把它和左右邻居合并成
// “左 + 右 - 自己”放回原处：以后若选中这个新值，等价于撤掉刚才的选择、
// 改成同时选左右两个。用小根堆 + 双向链表维护，堆里用惰性删除。
void SolveDouble() {
    static long long cur[MAXN];
    static bool gone[MAXN];
    static int prv[MAXN], nxt[MAXN];
    for (int i = 0; i <= N; ++i) {
        prv[i] = i - 1;
        nxt[i] = i + 1;
        gone[i] = false;
        cur[i] = INF;
    }
    for (int i = 0; i < N - 1; ++i) {
        cur[i + 1] = mooCost[i];
    }
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> heap;
    for (int i = 0; i <= N; ++i) {
        heap.push(make_pair(cur[i], i));
    }
    long long total = 0;
    for (int k = 0; k < N / 2; ++k) {
        // 惰性删除：被合并掉的、或代价已经变过的旧记录直接丢掉
        while (gone[heap.top().second] || heap.top().first != cur[heap.top().second]) {
            heap.pop();
        }
        int i = heap.top().second;
        long long value = heap.top().first;
        heap.pop();
        total += value;
        int a = prv[i], b = nxt[i];
        gone[a] = true;
        gone[b] = true;
        if (cur[a] < INF && cur[b] < INF) {
            cur[i] = cur[a] + cur[b] - value;
        } else {
            cur[i] = INF;  // 已经顶到边界，不能再“退货”扩展了
        }
        prv[i] = (a >= 0) ? prv[a] : -1;
        nxt[i] = (b <= N) ? nxt[b] : N + 1;
        if (prv[i] >= 0) {
            nxt[prv[i]] = i;
        }
        if (nxt[i] <= N) {
            prv[nxt[i]] = i;
        }
        heap.push(make_pair(cur[i], i));
        printf("%lld\n", total);
    }
}

// L = 3：记一个区间在左端删掉 i 个字符、右端删掉 j 个字符（i,j ≤ 2）后的
// 最优代价序列 seq[i][j]（seq[i][j][t] 表示再放 t 个 moo 的最小代价）。
// 容易证明这些序列都是凸的，两个凸序列的最小卷积可以用贪心归并在线性时间内完成。
struct AnswerSet {
    int len;
    vector<long long> seq[3][3];
};

// 把凸序列 a、b 的最小卷积写进 out 的下标 offset 开始处，
// 每个组合额外加上 base；偏移 offset 表示“已经用了 offset 个 moo”（跨中点的那个）。
void MergeInto(vector<long long> &out, const vector<long long> &a,
               const vector<long long> &b, int offset, long long base) {
    int need = (int)a.size() + (int)b.size() - 1 + offset;
    if ((int)out.size() < need) {
        out.resize(need, INF);
    }
    int ia = 0, ib = 0;
    long long value = base;
    for (int pos = offset;; ++pos) {
        if (value < out[pos]) {
            out[pos] = value;
        }
        // 凸序列的增量从小到大归并，谁的下一段增量更小就走谁
        if (ia + 1 < (int)a.size()) {
            if (ib + 1 < (int)b.size()) {
                if (a[ia + 1] - a[ia] <= b[ib + 1] - b[ib]) {
                    value += a[ia + 1] - a[ia];
                    ++ia;
                } else {
                    value += b[ib + 1] - b[ib];
                    ++ib;
                }
            } else {
                value += a[ia + 1] - a[ia];
                ++ia;
            }
        } else if (ib + 1 < (int)b.size()) {
            value += b[ib + 1] - b[ib];
            ++ib;
        } else {
            break;
        }
    }
}

AnswerSet SolveRange(int l, int r) {
    AnswerSet res;
    res.len = r - l;
    if (res.len < L) {
        // 太短，任何 moo 都放不下
        for (int i = 0; i < L; ++i) {
            for (int j = 0; j < L; ++j) {
                if (i + j <= res.len) {
                    res.seq[i][j].assign(1, 0);
                }
            }
        }
        return res;
    }
    int mid = (l + r) / 2;
    AnswerSet left = SolveRange(l, mid);
    AnswerSet right = SolveRange(mid, r);
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            if (i + j > res.len) {
                continue;
            }
            if (i >= left.len) {
                res.seq[i][j] = right.seq[i - left.len][j];
            } else if (j >= right.len) {
                res.seq[i][j] = left.seq[i][j - right.len];
            } else {
                // 不放跨中点的 moo：两边各自解决
                MergeInto(res.seq[i][j], left.seq[i][0], right.seq[0][j], 0, 0);
                // 放一个跨中点的 moo：它在左半边占 p 个字符、右半边占 L-p 个字符
                for (int p = 1; p < L; ++p) {
                    if (left.len >= i + p && right.len >= j + L - p) {
                        MergeInto(res.seq[i][j], left.seq[i][p],
                                  right.seq[L - p][j], 1, mooCost[mid - p]);
                    }
                }
            }
        }
    }
    return res;
}

void SolveTriple() {
    AnswerSet all = SolveRange(0, N);
    const vector<long long> &answer = all.seq[0][0];
    for (int k = 1; k <= N / L; ++k) {
        printf("%lld\n", answer[k]);
    }
}

int main() {
    scanf("%d %d", &L, &N);
    scanf("%s", str);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &changeCost[i]);
    }
    ComputeMooCosts();
    if (L == 1) {
        SolveSingle();
    } else if (L == 2) {
        SolveDouble();
    } else {
        SolveTriple();
    }
    return 0;
}
