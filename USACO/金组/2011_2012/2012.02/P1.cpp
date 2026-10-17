#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 50000;

struct Cow {
    int price;    // 不用优惠券的价格 P_i
    int discount; // 用优惠券之后的价格 C_i
};

struct Entry {
    long long cost; // 堆用来排序的关键值
    int id;         // 这头奶牛的编号

    // priority_queue 默认是大根堆, 把比较反过来, 它就变成了小根堆
    bool operator<(const Entry &other) const {
        return cost > other.cost;
    }
};

int n, k;
long long budget;
Cow cows[MAXN];
bool bought[MAXN];

int main() {
    scanf("%d %d %lld", &n, &k, &budget);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &cows[i].price, &cows[i].discount);
    }

    priority_queue<Entry> byDiscount; // 按优惠价排序: 抽堆顶就是"下一头用券买的奶牛"
    priority_queue<Entry> byPrice;    // 按原价排序:   抽堆顶就是"下一头全价买的奶牛"
    priority_queue<Entry> recover;    // 把一张券从某头奶牛身上撤回来的代价 P_i - C_i
    for (int i = 0; i < n; i++) {
        Entry cheap;
        cheap.cost = cows[i].discount;
        cheap.id = i;
        byDiscount.push(cheap);
        Entry full;
        full.cost = cows[i].price;
        full.id = i;
        byPrice.push(full);
    }
    // 一开始手上有 k 张券, 还没有用在任何奶牛身上, 所以"撤回来"的代价是 0
    for (int i = 0; i < k; i++) {
        Entry free;
        free.cost = 0;
        free.id = -1;
        recover.push(free);
    }

    int count = 0;
    while (count < n) {
        // 堆里的奶牛可能已经被买走了, 先清掉
        while (!byDiscount.empty() && bought[byDiscount.top().id]) byDiscount.pop();
        while (!byPrice.empty() && bought[byPrice.top().id]) byPrice.pop();
        if (byDiscount.empty() || byPrice.empty()) break;

        long long withCoupon = recover.top().cost + byDiscount.top().cost;
        long long withoutCoupon = byPrice.top().cost;
        if (withCoupon < withoutCoupon) {
            // 让这头奶牛用券: 先从最不值钱的地方撤回一张券, 再记下它自己的撤回代价
            if (budget < withCoupon) break;
            budget -= withCoupon;
            int id = byDiscount.top().id;
            recover.pop();
            Entry back;
            back.cost = cows[id].price - cows[id].discount;
            back.id = id;
            recover.push(back);
            bought[id] = true;
        } else {
            if (budget < withoutCoupon) break;
            budget -= withoutCoupon;
            bought[byPrice.top().id] = true;
        }
        count++;
    }

    printf("%d\n", count);
    return 0;
}
