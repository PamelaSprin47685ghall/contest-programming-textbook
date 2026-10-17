#include <cstdio>

using namespace std;

const int MAXN = 100005;

// 每条出边 i -> a_i 代表好友 i 的一次"出发", 收益是 v_i.
// 所有边构成一张函数图(每个点出度为 1): 每个连通分量恰好含一个简单环,
// 环上挂着若干棵指向环的有向树.
// 关键结论: 一个分量的最大收益 = 分量内所有 v 之和 - 环上最小的 v.
//   上界: 取环上在排列 p 中最晚出场的好友 c, 轮到 c 时 a_c 要么已经出发
//         (此时 c 不能出发), 要么还没出发(说明 a_c 在自己的轮次里被拦下了,
//         永远不会出发), 所以环上至少有一头牛拿不到收益, 最优也只能放弃
//         环上 v 最小的那一头.
//   下界: 取环上 v 最小的 c, 把 p 排成"除 c 外任何 i 都排在 a_i 前面".
//         删掉边 c -> a_c 后剩下的边是一棵以 c 为根的有向树, 按到 c 的距离
//         从大到小排列即可满足条件. 此时环上除 c 外的牛依次出发(每头牛的
//         目标都还没离开), 于是轮到 c 时 a_c 早已出发, 只有 c 拿不到收益.

int nxt[MAXN];    // nxt[i] = a_i
int val[MAXN];    // val[i] = v_i
int state[MAXN];  // 0 = 从未访问, 1 = 在当前这次行走的路径上, 2 = 已处理完
int path[MAXN];   // 当前这次行走依次经过的点
int pos[MAXN];    // pos[i] = 点 i 在 path 中的下标

int main() {
    int N;
    scanf("%d", &N);
    long long total = 0;  // v 之和最大 1e5 * 1e9 = 1e14, 必须用 64 位
    for (int i = 1; i <= N; ++i) {
        scanf("%d%d", &nxt[i], &val[i]);
        total += val[i];
    }
    long long answer = total;
    for (int start = 1; start <= N; ++start) {
        if (state[start] != 0) continue;
        // 迭代地顺着出边走, 边走边把路径记下来, 避免递归爆栈
        int top = 0;
        int x = start;
        while (state[x] == 0) {
            state[x] = 1;
            pos[x] = top;
            path[top++] = x;
            x = nxt[x];
        }
        // 撞上"本次路径上的点"才说明发现了一个还没处理过的环
        if (state[x] == 1) {
            int cheapest = val[path[pos[x]]];
            for (int k = pos[x]; k < top; ++k) {
                if (val[path[k]] < cheapest) cheapest = val[path[k]];
            }
            answer -= cheapest;
        }
        // 本次路径上的点全部处理完毕(含环上的点)
        for (int k = 0; k < top; ++k) state[path[k]] = 2;
    }
    printf("%lld\n", answer);
    return 0;
}
