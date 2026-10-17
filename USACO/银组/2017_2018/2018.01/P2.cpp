#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

struct Shop {
    int quantity; // 这家店最多收购的牛奶量
    int price;    // 每加仑价格（美分）
};

int milk[MAXN];                // 每头奶牛的产奶量
Shop shop[MAXN];               // 每个商店的收购量与单价
int rental[MAXN];              // 每个邻居愿意出的租金
long long maxProfit[MAXN + 1]; // maxProfit[i] = 只让产奶量最高的 i 头牛挤奶能赚的钱

// 都按从大到小排：产奶多的牛先挤、单价高的店先卖、租金高的邻居先租
bool descending(int a, int b) {
    return a > b;
}

bool shopByPrice(const Shop& a, const Shop& b) {
    return a.price > b.price;
}

int main() {
    int n, m, r;
    scanf("%d %d %d", &n, &m, &r);
    for (int i = 0; i < n; i++) scanf("%d", &milk[i]);
    for (int i = 0; i < m; i++) scanf("%d %d", &shop[i].quantity, &shop[i].price);
    for (int i = 0; i < r; i++) scanf("%d", &rental[i]);
    sort(milk, milk + n, descending);
    sort(shop, shop + m, shopByPrice);
    sort(rental, rental + r, descending);

    // 决定让哪些牛挤奶时，只有“产奶量最高的 i 头”这种前缀值得考虑：
    // 产奶少的牛去挤奶不如留给租金高的邻居。挤出的奶当然优先卖给单价高的店，
    // 于是按产奶量从高到低逐头奶牛贪心，把每头牛的奶卖到当前最贵的店。
    // 商店指针 bestShop 只会单向前进：前面的店被卖空了才轮到下一家。
    maxProfit[0] = 0;
    int bestShop = 0;
    for (int i = 0; i < n; i++) {
        maxProfit[i + 1] = maxProfit[i];
        int left = milk[i];
        while (left > 0 && bestShop < m) {
            int sold = min(left, shop[bestShop].quantity);
            maxProfit[i + 1] += (long long)sold * shop[bestShop].price;
            left -= sold;
            shop[bestShop].quantity -= sold;
            if (shop[bestShop].quantity == 0) bestShop++;
        }
    }

    // 对称地，被租走的应该是产奶量最低的 j 头牛，租金从最高的开始拿。
    // maxProfit[n - j] 表示最高的 n - j 头牛挤奶，剩下 j 头租出去，
    // 把前 j 高的租金累加到它上面；租金攒成前缀和，循环里只加一次。
    long long rentSoFar = 0;
    for (int j = 1; j <= r && j <= n; j++) {
        rentSoFar += rental[j - 1];
        maxProfit[n - j] += rentSoFar;
    }

    long long best = 0;
    for (int i = 0; i <= n; i++) {
        if (maxProfit[i] > best) best = maxProfit[i];
    }
    printf("%lld\n", best);
    return 0;
}
