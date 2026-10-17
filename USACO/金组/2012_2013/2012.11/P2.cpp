#include <cstdio>
#include <map>
#include <vector>

using namespace std;

const int MAXK = 10;
const int MAXN = 50005;

int K, N;
char S[MAXK][MAXN + 1];
int pre[MAXK][MAXN + 1];  // 每个串的前缀高度: '(' 记 +1, ')' 记 -1
int low[MAXN + 1];        // low[j]: 所有串里 "最后一个让高度低于 pre[s][j] 的位置" 的最大值

// 同一组前缀高度 (即同一个 key) 出现过的位置, 按时间顺序存放;
// head 指向第一个还没被淘汰的位置: 因为 low[j] 随 j 单调不减, 位置一旦低于 low 就永远没用了.
struct Bucket {
    vector<int> pos;
    int head;
    Bucket() : head(0) {}
};

int main() {
    scanf("%d %d", &K, &N);
    for (int s = 0; s < K; s++) scanf("%s", S[s]);

    for (int j = 0; j <= N; j++) low[j] = -1;

    for (int s = 0; s < K; s++) {
        pre[s][0] = 0;
        for (int j = 1; j <= N; j++) {
            pre[s][j] = pre[s][j - 1] + (S[s][j - 1] == '(' ? 1 : -1);
        }

        // 单调栈: 求每个位置左边 "最后一个高度严格更小" 的位置 p (不存在则 -1).
        // 区间 (p, j] 内的高度都 >= pre[s][j], 因此以 p 为起点左端点的区间一旦越过 j 就不平衡了.
        int stack[MAXN + 1];
        int top = 0;
        for (int j = 0; j <= N; j++) {
            while (top > 0 && pre[s][stack[top - 1]] >= pre[s][j]) top--;
            int p = (top > 0) ? stack[top - 1] : -1;
            if (p > low[j]) low[j] = p;
            stack[top++] = j;
        }
    }

    map<vector<int>, Bucket> mp;
    long long ans = 0;  // 最多约 N^2/2 个区间, 需要 64 位
    vector<int> key(K);
    for (int j = 0; j <= N; j++) {
        for (int s = 0; s < K; s++) key[s] = pre[s][j];
        Bucket &bk = mp[key];
        while (bk.head < (int)bk.pos.size() && bk.pos[bk.head] < low[j]) bk.head++;
        // 之前出现过的、下标 >= low[j] 的同一组前缀高度都可以作为区间左端点 (p 代表左端点前一位)
        ans += (long long)(bk.pos.size() - bk.head);
        bk.pos.push_back(j);
    }

    printf("%lld\n", ans);
    return 0;
}
