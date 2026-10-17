#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int N;
    scanf("%d", &N);
    vector<int> a(N);
    for (int i = 0; i < N; ++i) scanf("%d", &a[i]);

    // 把数组复制一份接在后面，圆上的"走到下一个位置"就变成下标加一。
    // last[v] 是值 v 在展开数组里最近一次出现的位置，nxt[i] 是 a[i] 的下一次出现位置。
    vector<int> last(N + 1, -1);
    vector<int> nxt(N, -1);
    int r = 0;  // 覆盖全部出现过的值时，左端 i = 0 最靠左的右端
    for (int i = 0; i < 2 * N; ++i) {
        int v = a[i % N];
        if (last[v] == -1) {
            if (i > r) r = i;  // v 第一次出现：右端至少要到这里
        } else if (last[v] < N) {
            nxt[last[v]] = i;
        }
        last[v] = i;
    }

    // 路线至多转一次弯：先走到区间 [l, r] 的某一端，再沿区间走到另一端。
    // 起点恰好在端点时开销就是区间长度 r - l，先把这些值记下来。
    vector<int> ans(N, N);
    for (int l = 0; l < N; ++l) {
        // 当前 [l, r] 恰好覆盖所有值：起点为 l 就往右走 r - l 步，起点为 r（模 N）就往左走 r - l 步。
        int len = r - l;
        if (len < ans[l]) ans[l] = len;
        if (len < ans[r % N]) ans[r % N] = len;
        // 左端右移一位后，若值 a[l] 在 (l, r] 里不再出现，右端要扩到它下一次出现的位置。
        if (nxt[l] > r) r = nxt[l];
    }

    // 一般起点 i：先朝左或朝右走到某个端点 j，再走完那条区间，共 |i - j| + ans[j] 步。
    // 相邻两个起点的答案至多相差 1，所以把 ans 沿两个方向各扫一遍就能把端点答案传开；
    // 由于传播可能要跨过下标 0 回到开头，正、反两遍算一轮，共做两轮。
    for (int round = 0; round < 2; ++round) {
        for (int i = 0; i < N; ++i) {
            int j = (i + 1 == N) ? 0 : i + 1;
            if (ans[i] + 1 < ans[j]) ans[j] = ans[i] + 1;
        }
        for (int i = N - 1; i >= 0; --i) {
            int j = (i + 1 == N) ? 0 : i + 1;
            if (ans[j] + 1 < ans[i]) ans[i] = ans[j] + 1;
        }
    }

    for (int i = 0; i < N; ++i) {
        if (i) printf(" ");
        printf("%d", ans[i]);
    }
    printf("\n");
    return 0;
}
