#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

typedef long long ll;

const ll CAP = 2000000000000000000LL; // 2e18：远大于任何真正答案，同时防止加法溢出
const int MAXN = 5000;

int N;
ll T;
vector<ll> trainTime[2]; // 两侧台列车的最早发车时刻，已升序排序

int cnt[2];       // 两侧的列车数
int strideB;      // dp 第二维长度（1 号侧台列车数加一）
vector<ll> dpBuf; // dp[a][b][s]：a、b 为两侧已发出的列车数，最后一班"准点"发出的车在 s 侧
int sentSoFar[2]; // 每一侧已经当作锚点处理过的列车数
ll answer;

// 取 dp[a][b][s] 的引用
ll &dpState(int a, int b, int s) {
    return dpBuf[(size_t)((a * strideB) + b) * 2 + s];
}

// 加法并封顶：代价只会越攒越大，超过 CAP 的方案不可能最优，直接压成 CAP
ll capAdd(ll a, ll b) {
    if (a >= CAP || b >= CAP) return CAP;
    ll s = a + b;
    return s > CAP ? CAP : s;
}

// 以"side 侧第 i 班车准点发出"为锚点，处理所有由它推出的状态（对应官解的 process_all）
void processAll(int side) {
    int i = ++sentSoFar[side];
    // 这班车发出后，对面最早也只能在 t 时刻发车（此时它正好到站）
    ll t = trainTime[side][i - 1] + T;
    ll dp2 = CAP; // 从这里换向（并顺手发掉对面所有已就绪的列车）的最优总延误
    ll add = 0;   // 若此刻换向，对面所有 t_i <= t 的列车就地发出所需的延误
    int cur[2];
    cur[0] = i;
    cur[1] = 0; // 暂时借用：记录对面已经就绪（t_i <= t）的列车数
    for (int j = cnt[side ^ 1]; j >= 0; --j) {
        if (j < cnt[side ^ 1] && trainTime[side ^ 1][j] <= t) {
            add = capAdd(add, t - trainTime[side ^ 1][j]);
            if (cur[1] == 0) cur[1] = j + 1; // 对面 t_i <= t 的列车共有 j+1 班
        }
        ll v = side ? dpState(j, i, 1) : dpState(i, j, 0);
        if (v >= CAP) continue; // 这个状态不可达
        // 同一侧继续准点发下一班：同向列车可以同时在轨，不加延误
        if (i < cnt[side]) {
            ll &slot = side ? dpState(j, i + 1, 1) : dpState(i + 1, j, 0);
            if (v < slot) slot = v;
        }
        // 或者换向：对面已就绪的列车就地发出
        ll cand = capAdd(v, add);
        if (cand < dp2) dp2 = cand;
    }
    if (side) { // 让 cur[0]、cur[1] 分别对应 0 号、1 号侧台
        int tmp = cur[0];
        cur[0] = cur[1];
        cur[1] = tmp;
    }
    if (dp2 >= CAP) return; // 这条链上没有可行方案
    // 之后每隔 T 换一次向：每次把当前侧已经就绪的列车全部就地发出；
    // 也可以停在某一侧，让它的下一班列车准点出发，成为新的锚点状态
    for (int k = 1; k <= N; ++k, t += T) {
        int s = (side + k) & 1;
        while (cur[s] < cnt[s] && trainTime[s][cur[s]] <= t) {
            dp2 = capAdd(dp2, t - trainTime[s][cur[s]]);
            ++cur[s];
        }
        if (cur[s] == cnt[s]) {
            if (cur[s ^ 1] == cnt[s ^ 1] && dp2 < answer) answer = dp2; // 全部发完
        } else {
            ll &slot = dpState(cur[0] + (s == 0), cur[1] + (s == 1), s);
            if (dp2 < slot) slot = dp2;
        }
    }
}

int main() {
    scanf("%d%lld", &N, &T);
    for (int i = 0; i < N; ++i) {
        char s;
        ll t;
        scanf(" %c %lld", &s, &t);
        trainTime[s == 'B' ? 1 : 0].push_back(t);
    }
    for (int side = 0; side < 2; ++side) {
        sort(trainTime[side].begin(), trainTime[side].end());
        cnt[side] = (int)trainTime[side].size();
    }

    strideB = cnt[1] + 1;
    dpBuf.assign((size_t)(cnt[0] + 1) * strideB * 2, CAP);
    if (cnt[0]) dpState(1, 0, 0) = 0; // 第一班 A 侧车准点出发
    if (cnt[1]) dpState(0, 1, 1) = 0; // 第一班 B 侧车准点出发
    sentSoFar[0] = sentSoFar[1] = 0;
    answer = CAP;

    // 按锚点列车的原始时刻从小到大处理：先处理两侧下一班车较早的那一侧
    while (sentSoFar[0] < cnt[0] && sentSoFar[1] < cnt[1]) {
        if (trainTime[0][sentSoFar[0]] < trainTime[1][sentSoFar[1]]) processAll(0);
        else processAll(1);
    }
    for (int side = 0; side < 2; ++side) {
        while (sentSoFar[side] < cnt[side]) processAll(side);
    }

    printf("%lld\n", answer);
    return 0;
}
