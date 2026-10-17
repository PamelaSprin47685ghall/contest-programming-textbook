#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 50000;

struct Station {
    int pos;    // 距起点的距离
    int price;  // 每单位油价
};

Station st[MAXN];

bool cmpPos(const Station &a, const Station &b) {
    return a.pos < b.pos;
}

int nxt[MAXN];  // nxt[i]: 在 i 之后第一家油价严格更便宜的加油站, -1 表示没有
int stk[MAXN];  // 单调栈: 存下标, 栈内油价从上到下递增

int main() {
    int n;
    long long cap, fuel, road;
    scanf("%d %lld %lld %lld", &n, &cap, &fuel, &road);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &st[i].pos, &st[i].price);
    }

    // 按位置排序后, 只需一路向前贪心
    sort(st, st + n, cmpPos);

    // 从右往左扫描: 时刻保证栈里油价自底向上严格递增.
    // 栈顶比当前站贵(或同价)的站永远不会成为"更近的便宜站", 直接弹掉,
    // 于是栈顶(若存在)就是当前位置之后第一家更便宜的加油站, 均摊 O(N).
    int top = 0;
    for (int i = n - 1; i >= 0; --i) {
        while (top > 0 && st[stk[top - 1]].price >= st[i].price) {
            --top;
        }
        nxt[i] = (top == 0 ? -1 : stk[top - 1]);
        stk[top++] = i;
    }

    long long cost = 0;
    long long cur = fuel - st[0].pos;  // 开到第一家加油站后剩下的油
    for (int i = 0; i < n; ++i) {
        if (cur < 0) {
            printf("-1\n");  // 连当前加油站都到不了, 目的地更到不了
            return 0;
        }
        // 需要的油量: 刚好开到之后第一家便宜站(到那里再加油更划算), 一路都不便宜则尽量加满;
        // 两者都受油箱容量限制. 若车上已够, 就不买.
        long long target = (nxt[i] == -1 ? road : st[nxt[i]].pos);
        long long need = target - st[i].pos;
        if (need > cap) {
            need = cap;
        }
        if (need > cur) {
            cost += (need - cur) * st[i].price;
            cur = need;
        }
        // 开到下一站(或终点)
        long long nextPos = (i + 1 == n ? road : st[i + 1].pos);
        cur -= nextPos - st[i].pos;
    }

    if (cur < 0) {
        printf("-1\n");  // 最后一段油不够
    } else {
        printf("%lld\n", cost);
    }
    return 0;
}
