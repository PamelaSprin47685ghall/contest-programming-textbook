#include <algorithm>
#include <cstdio>
#include <vector>

struct Gift {
    long long price;   // 原价
    long long shipping;
    long long total;   // 原价 + 运费
};

// 读入并排序, 同时把 total 变成前缀和: total[i] = 前 i+1 件礼物总价之和.
std::vector<Gift> ReadAndPrefixSum(int n) {
    std::vector<Gift> gifts(n);
    for (Gift& gift : gifts) {
        scanf("%lld%lld", &gift.price, &gift.shipping);
        gift.total = gift.price + gift.shipping;
    }
    auto cheaper = [](const Gift& a, const Gift& b) {
        return a.total < b.total;
    };
    std::sort(gifts.begin(), gifts.end(), cheaper);
    for (int i = 1; i < n; ++i) gifts[i].total += gifts[i - 1].total;
    return gifts;
}

// 若第 discounted 件礼物用半价券, 买前 buy 件的最小花费.
long long Money(const std::vector<Gift>& gifts, int buy, int discounted) {
    long long couponPrice = gifts[discounted].price / 2 + gifts[discounted].shipping;
    if (buy == 1) return couponPrice;

    int lastIndex = buy - 1;  // 前 buy 件里最后一件的下标
    // 半价礼物在前 buy 件之外 (或恰好是最后一件) 时, 前 buy-1 件原价买,
    // 再单独用半价买第 discounted 件.
    if (buy <= discounted + 1) {
        return gifts[lastIndex - 1].total + couponPrice;
    }
    // 半价礼物在前 buy 件之内: 前缀和减去它原价与半价的差价.
    long long discount = gifts[discounted].price + gifts[discounted].shipping - couponPrice;
    return gifts[lastIndex].total - discount;
}

int main() {
    int n, budget;
    scanf("%d%d", &n, &budget);
    std::vector<Gift> gifts = ReadAndPrefixSum(n);

    int answer = 0;
    // 枚举哪件礼物用半价券, 对每件二分能买的最大数量.
    for (int discounted = 0; discounted < n; ++discounted) {
        int lower = 0, upper = n;
        while (lower < upper) {
            int mid = lower + (upper - lower + 1) / 2;
            if (Money(gifts, mid, discounted) <= budget) {
                lower = mid;
            } else {
                upper = mid - 1;
            }
        }
        answer = (lower > answer) ? lower : answer;
    }
    printf("%d\n", answer);
    return 0;
}