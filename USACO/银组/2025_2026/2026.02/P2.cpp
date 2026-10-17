#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int N, C;
    scanf("%d %d", &N, &C);
    vector<int> quota(C);
    for (int i = 0; i < C; ++i) scanf("%d", &quota[i]);
    vector<int> decline(N);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &decline[i]);
        --decline[i];
    }

    // satisfy[c] 里按名次递增地存着"满足第 c 条标准"的选手；head[c] 是队首位置。
    vector<vector<int> > satisfy(C);
    for (int i = 0; i < N; ++i) {
        int k;
        scanf("%d", &k);
        for (int j = 0; j < k; ++j) {
            int c;
            scanf("%d", &c);
            satisfy[c - 1].push_back(i);
        }
    }

    // invitedUnder[i] = 选手 i 被哪条标准邀请（C 表示还没被邀请，-1 表示已弃赛）。
    vector<int> invitedUnder(N, C);
    vector<int> head(C, 0);
    long long answer = 0;

    // 初始邀请：按标准编号从小到大，每条约 cap 个名额，队首里已被更早标准邀请的就跳过。
    for (int c = 0; c < C; ++c) {
        int got = 0;
        while (head[c] < (int)satisfy[c].size() && got < quota[c]) {
            int x = satisfy[c][head[c]];
            ++head[c];
            if (invitedUnder[x] < c) continue;
            invitedUnder[x] = c;
            answer += x + 1;
            ++got;
        }
    }

    for (int i = 0; i < N; ++i) {
        printf("%lld\n", answer);
        int d = decline[i];
        int c = invitedUnder[d];
        invitedUnder[d] = -1;
        if (c == C) continue;  // 本来就没被邀请，名额数不变
        answer -= d + 1;
        // d 空出标准 c 的一个名额：把队首还可用的人填进来。
        // 若填进来的 x 原本挂在更晚的标准 c' 上，就换出标准 c' 的空名额继续传递；
        // 否则 x 本来没被邀请，名额传递结束。
        while (head[c] < (int)satisfy[c].size()) {
            int x = satisfy[c][head[c]];
            ++head[c];
            if (invitedUnder[x] < c) continue;
            int wasUnder = invitedUnder[x];
            invitedUnder[x] = c;
            c = wasUnder;
            if (c == C) {
                answer += x + 1;
                break;
            }
        }
    }
    return 0;
}
