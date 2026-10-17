#include <cstdio>
#include <algorithm>

using namespace std;

// 把每个不同的 ID 看成一个点：若两个 ID 之和等于 A 或 B（2*id == A 时是自环），
// 就在它们之间连一条边 —— 只有这样的两头牛才能配对。
// 每个点至多连 2 条边（对应 A、B 各一条），所以图是若干条路径和环的并。
// 贪心摘叶子：若点 x 只连一条边到 y，那么在这条边上配尽可能多的对不会更差 ——
// 每多配一对最多只是让 y 少配一对，总对数不减。于是榨干这条边后删掉它，把 y 重新
// 入栈。重复到栈空即可处理完所有边：A != B 时图上不会出现"转不出去"的环，因为沿
// A 边、B 边交替走会让 ID 每次增加 B - A，点有限所以必然停在一个度数为 1 的点。
// 复杂度：排序 O(N log N)，摘叶子 O(N)。

const int MAXN = 200005;

struct Cow {
    long long id;
    long long count;
};

Cow cows[MAXN];

bool cmpCow(const Cow &x, const Cow &y) {
    return x.id < y.id;
}

// 供 lower_bound 使用：把元素按 id 直接和目标值比较
bool cmpId(const Cow &x, long long id) {
    return x.id < id;
}

int N;
long long A, B;
int distinctCnt;
long long cowCount[MAXN]; // 每个不同 ID 的奶牛数（配对过程中会被扣减）
int deg[MAXN];            // 度数，自环算 1
int nb[MAXN][2];          // 邻居下标，自环时为自身
int stackIds[3 * MAXN];   // 摘叶子用的手写栈

int main() {
    scanf("%d %lld %lld", &N, &A, &B);
    for (int i = 0; i < N; ++i) {
        scanf("%lld %lld", &cows[i].count, &cows[i].id);
    }

    // 按 ID 升序排好，之后用下标当点编号、用二分找"和为 A/B 的搭档"
    sort(cows, cows + N, cmpCow);
    distinctCnt = N; // 题目保证 ID 两两不同
    for (int i = 0; i < distinctCnt; ++i) {
        cowCount[i] = cows[i].count;
        deg[i] = 0;
    }

    long long sums[2] = {A, B};
    for (int i = 0; i < distinctCnt; ++i) {
        for (int k = 0; k < 2; ++k) {
            long long target = sums[k] - cows[i].id;
            int pos = (int)(lower_bound(cows, cows + distinctCnt, target, cmpId) - cows);
            if (pos >= distinctCnt || cows[pos].id != target) {
                continue;
            }
            if (deg[i] == 1 && nb[i][0] == pos) {
                continue; // A == B 时同一个搭档不重复连边
            }
            nb[i][deg[i]++] = pos;
        }
    }

    long long answer = 0;
    int top = 0;
    for (int i = 0; i < distinctCnt; ++i) {
        stackIds[top++] = i;
    }
    while (top > 0) {
        int x = stackIds[--top];
        if (deg[x] != 1) {
            continue; // 度数为 0 或 2 的点暂时没得摘
        }
        int y = nb[x][0];
        if (x == y) {
            answer += cowCount[x] / 2; // 自环只能同类两头两头地配
            deg[x] = 0;
            continue;
        }
        long long pairs = cowCount[x] < cowCount[y] ? cowCount[x] : cowCount[y];
        answer += pairs;
        cowCount[y] -= pairs;
        deg[x] = 0;
        // 从 y 的邻接表里删掉 x
        if (nb[y][0] == x) {
            nb[y][0] = nb[y][1];
        }
        nb[y][1] = -1;
        --deg[y];
        stackIds[top++] = y;
    }
    printf("%lld\n", answer);
    return 0;
}
