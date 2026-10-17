// P2 Milk Buckets
// 记 b_i 为第 i 个桶相邻两次翻转（倒空）的间隔：b_1 = a_1 + 1（装满耗时 a_1，
// 翻转耗时 1）；上面那个桶每倒空一次给下面送去 a_1 加仑，且它每次倒空时正好装满，
// 所以第 i 个桶装满需要 ceil(a_i / a_{i-1}) 次上面的倒空，b_i = ceil(a_i / a_{i-1}) * b_{i-1}。
// 第 N 个桶第一次往池子倒在第 N-1+b_N 秒（下面每层都晚一次），之后每 b_N 秒倒 a_N 加仑。
// 所以答案 = a_N * floor(max(t-(N-1), 0) / b_N)。
// b_N 可能超过 1e18，不能直接算，改成把除法链从前往后逐层做：
// ans = floor(x / (a_1+1))，再对每个 i 除以 ceil(a_i / a_{i-1})，逐层取整与一次相除等价。
// ceil 为 1 的位置（a_i <= a_{i-1}）除不除一样，用 set 只记下 a_i > a_{i-1} 的下标；
// 每个因子至少是 2，而 ans <= 1e18 < 2^60，所以每查询至多除 60 次就成了 0，可以提前停。
#include <cstdio>
#include <set>
#include <vector>

using namespace std;

int main() {
    int N;
    scanf("%d", &N);
    vector<long long> a(N);
    for (int i = 0; i < N; ++i) {
        scanf("%lld", &a[i]);
    }

    // up 保存所有 a[i] > a[i-1] 的下标 i（i >= 1），这些位置的因子至少是 2。
    set<int> up;
    for (int i = 1; i < N; ++i) {
        if (a[i] > a[i - 1]) {
            up.insert(i);
        }
    }

    int Q;
    scanf("%d", &Q);
    while (Q--) {
        int pos;
        long long v, t;
        scanf("%d %lld %lld", &pos, &v, &t);
        --pos;
        a[pos] = v;
        // 改动 a[pos] 只可能影响 pos 和 pos+1 两处是否"上升"。
        if (pos >= 1 && a[pos] > a[pos - 1]) up.insert(pos); else up.erase(pos);
        if (pos + 1 < N && a[pos + 1] > a[pos]) up.insert(pos + 1); else up.erase(pos + 1);

        long long rounds = t - (N - 1); // 池子第一次进水在第 N-1+b_N 秒
        if (rounds < 0) rounds = 0;
        long long ans = rounds / (a[0] + 1);
        for (set<int>::iterator it = up.begin(); it != up.end() && ans > 0; ++it) {
            long long prev = a[*it - 1];
            ans /= (a[*it] + prev - 1) / prev;
        }
        printf("%lld\n", ans * a[N - 1]);
    }
    return 0;
}
