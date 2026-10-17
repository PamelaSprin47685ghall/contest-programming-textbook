#include <cstdio>

using namespace std;

const int MAXN = 2005;

int N;
int id[MAXN];        // 各队的编号
int best[MAXN];      // best[i]: 队 i 与已加入生成树的队伍之间最大的比赛得分
bool used[MAXN];     // 队 i 是否已加入生成树

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) scanf("%d", &id[i]);

    long long answer = 0;
    // 比赛过程对应一棵生成树: 每场比赛淘汰一队, 恰好选出 N-1 条边把所有队连起来.
    // 要使总分最大, 就是求最大生成树; 图是稠密的, 用 O(N^2) 的 Prim 最合适.
    for (int step = 0; step < N; step++) {
        int pick = -1;
        for (int i = 0; i < N; i++) {
            if (used[i]) continue;
            if (pick == -1 || best[i] > best[pick]) pick = i;
        }
        answer += best[pick];
        used[pick] = true;

        // 新加入一队后, 用它更新其余队伍到生成树的最大边权.
        for (int i = 0; i < N; i++) {
            int score = id[pick] ^ id[i];
            if (!used[i] && score > best[i]) best[i] = score;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
