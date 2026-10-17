// P1 Clash!
// 每张牌记成 (是否为赢牌, 费用)：赢牌标记为 -1，其余为 0，比较大小就是
// "先看是不是赢牌，再挑便宜的"。FJ 每次必定打出手中最小的这张：手里有赢牌时
// 下一张只能是赢牌；同为赢牌或同为普通牌时先打便宜的，不会让后面的选择变差。
// 于是出牌顺序完全确定。moolixir 每秒 +1、牌费都是正数，所以第 j 张牌正好能
// 在第 j 张之前所有费用之和这一秒打出，t 秒内能打几张 = 前缀和不超过 t 的最大张数。
// 打满 N-H+1 张后进入循环：此后每步手里"全局最小的 N-H+1 张牌"只有一张，
// 正是 N-H+1 步之前打出的那张，因此出牌序列以 N-H+1 为周期重复。
// 把一轮里每张赢牌被打出的累计时刻记下来，每个查询整轮数乘每轮赢牌数，
// 零头再二分找出这一轮里还能多打几张赢牌。
#include <cstdio>
#include <algorithm>
#include <functional>
#include <queue>
#include <vector>

using namespace std;

int main() {
    int N, H;
    scanf("%d %d", &N, &H);

    vector<pair<int, int> > card(N); // (赢牌标记, 费用)
    for (int i = 0; i < N; ++i) {
        scanf("%d", &card[i].second);
    }

    int K;
    scanf("%d", &K);
    for (int i = 0; i < K; ++i) {
        int s;
        scanf("%d", &s);
        card[s - 1].first = -1;
    }

    // 优先队列存当前手牌：push 相当于摸牌，top 就是该打出的那张。
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > hand;
    for (int i = 0; i < H - 1; ++i) {
        hand.push(card[i]);
    }

    vector<long long> winTime; // 一轮（N-H+1 张）内赢牌被打出的累计时刻
    long long cycleLen = 0;    // 一轮的总费用，也是循环的时间长度
    for (int i = 0; i <= N - H; ++i) {
        hand.push(card[i + H - 1]);
        cycleLen += hand.top().second;
        if (hand.top().first == -1) {
            winTime.push_back(cycleLen);
        }
        hand.pop();
    }

    int Q;
    scanf("%d", &Q);
    while (Q--) {
        long long t;
        scanf("%lld", &t);
        long long ans = t / cycleLen * (long long)winTime.size(); // 完整循环轮数
        // 零头：剩下的费用额度在这一轮里还能覆盖到几张赢牌的分界点
        ans += upper_bound(winTime.begin(), winTime.end(), t % cycleLen) - winTime.begin();
        printf("%lld\n", ans);
    }
    return 0;
}
