#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1000; // 每边奶牛数上限
const int MAXK = 10;   // 队伍大小上限
const long long MOD = 1000000009;

struct Cow {
    long long score; // 评分
    bool fromJohn;   // 是否属于农夫约翰
};

Cow cow[2 * MAXN];
long long ways[MAXK + 1][MAXK + 1]; // ways[a][b]：已选出 a 头约翰的牛、b 头保罗的牛

// 评分升序；评分相同时约翰的牛排在前面，
// 这样评分并列的一对牛里约翰的牛会先被考虑，而它此时凑不出足够的保罗牛，并列对局自然被排除
bool scoreLess(const Cow &a, const Cow &b) {
    if (a.score != b.score) return a.score < b.score;
    return a.fromJohn && !b.fromJohn;
}

int main() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);

    for (int i = 0; i < n; ++i) {
        scanf("%lld", &cow[i].score);
        cow[i].fromJohn = true;
    }
    for (int i = 0; i < m; ++i) {
        scanf("%lld", &cow[n + i].score);
        cow[n + i].fromJohn = false;
    }

    // 两队按名次一一配对：约翰最高的对保罗最高的，第二高的对第二高的……
    // 从低分往高分扫，只要保证任何时候"已选约翰牛数 ≤ 已选保罗牛数"，
    // 每头约翰牛就一定能配到一头评分更低的保罗牛，获胜条件正好等价于此
    sort(cow, cow + n + m, scoreLess);

    ways[0][0] = 1;
    for (int t = 0; t < n + m; ++t) {
        if (cow[t].fromJohn) {
            // 选中这头约翰牛后必须仍有"保罗牛数 ≥ 约翰牛数"，
            // 所以只更新 a ≤ b 的状态；a 从小到大会把同一头牛用两次，必须倒着更新
            for (int b = 0; b <= k; ++b) {
                int upper = b < k ? b : k;
                for (int a = upper; a >= 1; --a) {
                    ways[a][b] = (ways[a][b] + ways[a - 1][b]) % MOD;
                }
            }
        } else {
            // 选一头保罗牛只是给后面留出配对名额，没有额外限制；b 同样要倒着更新
            for (int a = 0; a <= k; ++a) {
                for (int b = k; b >= 1; --b) {
                    ways[a][b] = (ways[a][b] + ways[a][b - 1]) % MOD;
                }
            }
        }
    }

    printf("%lld\n", ways[k][k]);
    return 0;
}
