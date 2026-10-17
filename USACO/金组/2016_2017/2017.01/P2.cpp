#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXK = 20;

// 把手势编号: 0 = 蹄子(H), 1 = 纸(P), 2 = 剪刀(S)
// 编号变成 (编号 + 1) % 3 后正好能赢它: H 被 P 赢, P 被 S 赢, S 被 H 赢
int fjMove[MAXN];
// dp[j][s]: 处理完前 i 局, 最多切换 j 次, 且第 i 局出 s 时最多赢多少局
int prevLayer[MAXK + 1][3];
int curLayer[MAXK + 1][3];

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    char buf[4];
    for (int i = 0; i < n; ++i) {
        scanf("%s", buf);
        if (buf[0] == 'H') {
            fjMove[i] = 0;
        } else if (buf[0] == 'P') {
            fjMove[i] = 1;
        } else {
            fjMove[i] = 2;
        }
    }

    for (int i = 1; i <= n; ++i) {
        int win = (fjMove[i - 1] + 1) % 3;  // 本局能赢 FJ 的手势
        for (int j = 0; j <= k; ++j) {
            for (int s = 0; s < 3; ++s) {
                // 沿用上一局的手势, 不消耗切换次数
                int best = prevLayer[j][s];
                if (j > 0) {
                    // 本局换手势: 上一局必须是另外两种手势之一, 消耗一次切换
                    best = max(best, prevLayer[j - 1][(s + 1) % 3]);
                    best = max(best, prevLayer[j - 1][(s + 2) % 3]);
                }
                if (s == win) {
                    ++best;
                }
                curLayer[j][s] = best;
            }
        }
        // 滚动数组: 本层算完再覆盖上一层, 避免用到被污染的旧数据
        for (int j = 0; j <= k; ++j) {
            for (int s = 0; s < 3; ++s) {
                prevLayer[j][s] = curLayer[j][s];
            }
        }
    }

    int answer = 0;
    for (int s = 0; s < 3; ++s) {
        answer = max(answer, prevLayer[k][s]);
    }
    printf("%d\n", answer);
    return 0;
}
