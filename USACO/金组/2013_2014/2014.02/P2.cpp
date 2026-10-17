#include <cstdio>

using namespace std;

const int MAXN = 20;
const int MAXB = 20;

int N, B;
int skill[MAXN][MAXN];        // skill[cow][event]: 第 cow 头牛参加第 event 个项目的得分
int bonusCnt[MAXN];           // 前 k+1 个项目上共有多少条奖励
int bonusNeed[MAXN][MAXB];    // 触发奖励所需的分数 P
int bonusAdd[MAXN][MAXB];     // 触发后额外获得的分数 A
int dp[1 << MAXN];            // dp[mask]: 用 mask 中的牛填满前 popcount(mask) 个项目时的最高总分

// 结算"前 k+1 个项目"这一组奖励. 奖励按所需分数从小到大排序后一次扫描即可:
// 若某条奖励没触发, 说明当前总分低于它的 P, 而后面奖励的 P 只会更大,
// 且分数只有触发奖励才会上升, 于是后面的也一定触发不了, 可以直接结束.
int award(int score, int k) {
    for (int i = 0; i < bonusCnt[k]; i++) {
        if (score < bonusNeed[k][i]) break;
        score += bonusAdd[k][i];
    }
    return score;
}

int main() {
    scanf("%d %d", &N, &B);
    for (int i = 0; i < B; i++) {
        int k, p, a;
        scanf("%d %d %d", &k, &p, &a);
        k--; // 第 k+1 个项目, 换成 0 下标
        // 插入排序, 保证同一组的奖励按 P 递增存放
        int pos = bonusCnt[k];
        while (pos > 0 && bonusNeed[k][pos - 1] > p) {
            bonusNeed[k][pos] = bonusNeed[k][pos - 1];
            bonusAdd[k][pos] = bonusAdd[k][pos - 1];
            pos--;
        }
        bonusNeed[k][pos] = p;
        bonusAdd[k][pos] = a;
        bonusCnt[k]++;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) scanf("%d", &skill[i][j]);
    }

    int total = 1 << N;
    for (int mask = 1; mask < total; mask++) {
        // 已安排好的牛数 = 已经填满的项目数, 新填的项目编号就是 k-1
        int k = __builtin_popcount(mask);
        int best = 0;
        for (int j = 0; j < N; j++) {
            if (!(mask & (1 << j))) continue;
            int cand = dp[mask ^ (1 << j)] + skill[j][k - 1];
            if (cand > best) best = cand;
        }
        // award 关于 score 单调不减, 所以先取最优转移再统一结算奖励不会漏掉更优解
        dp[mask] = award(best, k - 1);
    }

    printf("%d\n", dp[total - 1]);
    return 0;
}
