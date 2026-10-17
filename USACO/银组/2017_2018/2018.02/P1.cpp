#include <cstdio>

using namespace std;

const int MAXN = 100005;

int L, N, rF, rB;
int x[MAXN];
int c[MAXN];
bool worthStopping[MAXN];

int main() {
    scanf("%d%d%d%d", &L, &N, &rF, &rB);
    for (int i = 0; i < N; i++) {
        scanf("%d%d", &x[i], &c[i]);
    }

    // 从右往左标记"右侧没有更甜草"的休息点.
    // 若某个点右边存在更美味的点, 在该点吃草的那段时间搬到右边吃只会更好,
    // 因此这些点可以完全忽略.
    int bestRight = 0;
    for (int i = N - 1; i >= 0; i--) {
        if (c[i] > bestRight) {
            worthStopping[i] = true;
            bestRight = c[i];
        }
    }

    long long timeF = 0;  // FJ 走到当前关键点所花的时间
    long long timeB = 0;  // Bessie 按自己的速度走到该点所花的时间
    long long answer = 0;
    int lastX = 0;
    for (int i = 0; i < N; i++) {
        if (!worthStopping[i]) continue;

        timeF += (long long)(x[i] - lastX) * rF;
        timeB += (long long)(x[i] - lastX) * rB;

        // Bessie 早到 (timeF - timeB) 秒, 她可以一直吃到 FJ 追上为止;
        // 且在关键点上多呆的每一秒都不会被更甜的点取代, 故贪心最优.
        answer += (timeF - timeB) * (long long)c[i];

        // 吃饱后立即出发, 之后与 FJ 同时到达, 不会落后.
        timeB = timeF;
        lastX = x[i];
    }

    printf("%lld\n", answer);
    return 0;
}
