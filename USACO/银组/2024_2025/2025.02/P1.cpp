#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 1000005;

int a[MAXN]; // 原始队列，a[0] 是队首

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int N;
        scanf("%d", &N);
        for (int i = 0; i < N; ++i) {
            scanf("%d", &a[i]);
        }

        // 不执行任何操作时，只有"从自己开始到队尾的最大值"（后缀最大值）这头奶牛会进 b：
        // 其余奶牛迟早被后面某头更大的奶牛盖过，放进去只会让 b 更小。
        // 于是先把这些奶牛按原顺序存成（标签值, 在 a 中的下标）放进 b。
        vector<pair<int, int> > b;
        int suffixMax = -1;
        for (int i = N - 1; i >= 0; --i) {
            if (a[i] >= suffixMax) {
                suffixMax = a[i];
                b.push_back(make_pair(a[i], i));
            }
        }
        reverse(b.begin(), b.end());

        // 两头各加一个哨兵：哨兵值不参与输出，只用来统一处理"搬到队首"这一情形，
        // 并给最后一只真正可能被搬动的奶牛一个右边界位置 N。
        b.insert(b.begin(), make_pair(N + 1, -1));
        b.push_back(make_pair(-1, N));

        // 关键推理：把 b[i] 这头奶牛搬到 b[i-1] 所在位置的后一格，是让它右边"新露出来"的
        // 后缀最大值最多的一种搬法（搬得再靠前只会让 b[i] 自己提前，而 b[i] <= b[i-1]，
        // 只会更差）。新露出来的后缀最大值会插到 b[i] 之后，令 b 字典序变大。
        // 越靠前插进新元素改进越大，所以从左往右试，第一个能插进新元素的 i 就是答案。
        for (int i = 1; i + 1 < (int)b.size(); ++i) {
            vector<pair<int, int> > add;
            // 候选只能来自 (p[i-1], p[i+1]) 这一段，并且必须不低于 b[i+1]（后面那段的右边界）
            int runningMax = b[i + 1].first;
            for (int j = b[i + 1].second - 1; j > b[i - 1].second; --j) {
                if (j == b[i].second) continue; // 被搬走的那头自己不在候选之列
                if (a[j] > runningMax) runningMax = a[j];
                if (a[j] == runningMax) add.push_back(make_pair(a[j], j));
            }
            if (!add.empty()) {
                reverse(add.begin(), add.end()); // 扫描从右往左，插回 b 时要恢复成从左往右
                b.insert(b.begin() + i + 1, add.begin(), add.end());
                break;
            }
        }

        for (int i = 1; i + 1 < (int)b.size(); ++i) {
            printf("%d", b[i].first);
            if (i + 2 == (int)b.size()) {
                printf("\n");
            } else {
                printf(" ");
            }
        }
    }
    return 0;
}
