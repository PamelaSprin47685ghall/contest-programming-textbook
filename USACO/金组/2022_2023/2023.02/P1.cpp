#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 505;
const int MAXM = MAXN * (MAXN + 1) / 2;
const long long INF = 4000000000000000000LL;

int N;
long long prefix[MAXN];

struct Subarray {
    long long sum;
    int from, to;
};

Subarray sub[MAXM];

// 题目保证所有子段和互不相同，直接按和升序排序即可
bool cmpSum(const Subarray &x, const Subarray &y) {
    return x.sum < y.sum;
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        long long x;
        scanf("%lld", &x);
        prefix[i] = prefix[i - 1] + x;
    }

    int M = 0;
    for (int l = 1; l <= N; l++) {
        for (int r = l; r <= N; r++) {
            sub[M].from = l;
            sub[M].to = r;
            sub[M].sum = prefix[r] - prefix[l - 1];
            M++;
        }
    }
    sort(sub, sub + M, cmpSum);

    // 对固定的下标 k，只有「一个区间含 k、另一个不含 k」的一对区间才有意义。
    // 若最优的一对在排序后并不相邻，那么它们之间必然存在相邻的一对大小关系发生变化，
    // 那一对的差只会更小，所以只需检查排序后相邻的两项。
    for (int k = 1; k <= N; k++) {
        long long ans = INF;
        for (int j = 1; j < M; j++) {
            bool prevHas = (sub[j - 1].from <= k && k <= sub[j - 1].to);
            bool curHas = (sub[j].from <= k && k <= sub[j].to);
            if (prevHas != curHas) {
                long long diff = sub[j].sum - sub[j - 1].sum;
                if (diff < ans) ans = diff;
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}
