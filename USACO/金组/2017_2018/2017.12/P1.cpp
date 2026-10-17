#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

int n;                            // 每头牛各有 n 块派
long long d;                      // 允许的美味度差上限
long long tasteBessie[2 * MAXN];  // 每块派在 Bessie 眼里的美味度
long long tasteElsie[2 * MAXN];   // 每块派在 Elsie 眼里的美味度
int fewestGifts[2 * MAXN];        // 从这块派送出开始最少要送几块派才能幸福收场, -1 表示做不到

// 反向搜索用的有序表. 每张表都按"收到这块派的牛"的口味排序:
// Bessie 的派会被 Elsie 收到, 所以按 Elsie 眼中的美味度排序;
// Elsie 的派会被 Bessie 收到, 所以按 Bessie 眼中的美味度排序.
struct RankedPie {
    long long taste;
    int id;
};
RankedPie bessiePies[MAXN];  // Bessie 的派, 按 tasteElsie 升序
RankedPie elsiePies[MAXN];   // Elsie 的派, 按 tasteBessie 升序
int bessieCount, elsieCount;

// 并查集: 表中已定案的位置指向下一个还没定案的位置.
// 每块派只会定案一次, 靠它把整张表在总共 O(n) 次移动里扫完.
int nextFreeBessie[MAXN + 1];
int nextFreeElsie[MAXN + 1];

int pending[2 * MAXN];  // 宽度优先搜索的队列

// 返回表中第一个 taste >= bound 的位置, 全部小于 bound 时返回 count
int FirstAtLeast(RankedPie table[], int count, long long bound) {
    int low = 0, high = count;
    while (low < high) {
        int mid = (low + high) / 2;
        if (table[mid].taste >= bound) high = mid;
        else low = mid + 1;
    }
    return low;
}

// 按美味度升序排列; 美味度相同时谁先谁后不影响结果
bool TasteLess(const RankedPie &x, const RankedPie &y) {
    return x.taste < y.taste;
}

// 返回 >= p 的第一个还没定案的位置, 顺带把路径上的位置直接挂到根上
int FindNextFree(int parent[], int p) {
    int root = p;
    while (parent[root] != root) root = parent[root];
    while (parent[p] != root) {
        int next = parent[p];
        parent[p] = root;
        p = next;
    }
    return root;
}

int main() {
    scanf("%d%lld", &n, &d);
    for (int i = 0; i < 2 * n; i++) {
        scanf("%lld%lld", &tasteBessie[i], &tasteElsie[i]);
        fewestGifts[i] = -1;
    }

    // 收礼者认为到手的派美味度为 0 时交换立刻结束, 这就是唯一的幸福收场.
    // 收场的派从 1 开始计数, 它就是反向搜索的起点.
    int head = 0, tail = 0;
    for (int i = 0; i < n; i++) {
        if (tasteElsie[i] == 0) {
            fewestGifts[i] = 1;
            pending[tail++] = i;
        }
        if (tasteBessie[n + i] == 0) {
            fewestGifts[n + i] = 1;
            pending[tail++] = n + i;
        }
    }

    // 已经定案的派不必再进有序表
    for (int i = 0; i < n; i++) {
        if (fewestGifts[i] == -1) {
            bessiePies[bessieCount].taste = tasteElsie[i];
            bessiePies[bessieCount].id = i;
            bessieCount++;
        }
    }
    for (int i = 0; i < n; i++) {
        if (fewestGifts[n + i] == -1) {
            elsiePies[elsieCount].taste = tasteBessie[n + i];
            elsiePies[elsieCount].id = n + i;
            elsieCount++;
        }
    }
    for (int i = 0; i < bessieCount; i++) nextFreeBessie[i] = i;
    nextFreeBessie[bessieCount] = bessieCount;
    for (int i = 0; i < elsieCount; i++) nextFreeElsie[i] = i;
    nextFreeElsie[elsieCount] = elsieCount;

    sort(bessiePies, bessiePies + bessieCount, TasteLess);
    sort(elsiePies, elsiePies + elsieCount, TasteLess);

    // 在"送礼顺序"图上反向求最短路: 已知 current 的最短长度,
    // 就去找所有能当它上一块的派. 每块派第一次被找到时就是最短的, 所以用队列.
    while (head < tail) {
        int current = pending[head++];
        if (current < n) {
            // current 是 Bessie 送出的派, 上一块只能是 Elsie 的派.
            // Elsie 送出上一块时比的是 Bessie 眼中的美味度, 于是
            // tasteBessie[上一块] <= tasteBessie[current] <= tasteBessie[上一块] + d.
            long long low = tasteBessie[current] - d;
            int pos = FirstAtLeast(elsiePies, elsieCount, low);
            pos = FindNextFree(nextFreeElsie, pos);
            while (pos < elsieCount && elsiePies[pos].taste <= tasteBessie[current]) {
                int previous = elsiePies[pos].id;
                fewestGifts[previous] = fewestGifts[current] + 1;
                pending[tail++] = previous;
                nextFreeElsie[pos] = pos + 1;  // 这个位置的派已经定案, 以后跳过
                pos = FindNextFree(nextFreeElsie, pos + 1);
            }
        } else {
            // current 是 Elsie 送出的派, 上一块只能是 Bessie 的派,
            // 同理要用 Elsie 眼中的美味度做区间判断
            long long low = tasteElsie[current] - d;
            int pos = FirstAtLeast(bessiePies, bessieCount, low);
            pos = FindNextFree(nextFreeBessie, pos);
            while (pos < bessieCount && bessiePies[pos].taste <= tasteElsie[current]) {
                int previous = bessiePies[pos].id;
                fewestGifts[previous] = fewestGifts[current] + 1;
                pending[tail++] = previous;
                nextFreeBessie[pos] = pos + 1;
                pos = FindNextFree(nextFreeBessie, pos + 1);
            }
        }
    }

    for (int i = 0; i < n; i++) printf("%d\n", fewestGifts[i]);
    return 0;
}
