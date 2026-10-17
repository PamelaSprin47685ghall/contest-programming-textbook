#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 2000;
const int MAXSUM = 4000;  // A + B 的上界：2000 + 2000

// 一位朋友：收买他能得到的流行度、直接收买需要的 moonie、每个折扣需要的冰淇淋数
struct Friend {
    int popularity;
    int moonieCost;
    int conePerDiscount;
};

Friend friends[MAXN + 5];

// 按 X 升序排列：同样多的冰淇淋给 X 小的朋友能换来最多的折扣，
// 所以"用冰淇淋打折的朋友"一定是排序后的一段前缀，这样才好做一维 DP
bool byConePerDiscount(const Friend &a, const Friend &b) {
    return a.conePerDiscount < b.conePerDiscount;
}

// 状态 j 的含义（排序后处理到若干位朋友时）：
//   j > moonies 表示这一路上只花过冰淇淋，钱还是原始的 moonies，
//                剩下的冰淇淋数是 j - moonies；
//   j <= moonies 表示冰淇淋已经全部花光，钱还剩 j。
// 之所以能这样合并成一维：最优方案里冰淇淋一定先在 X 小的朋友身上用光，
// 之后才动钱，于是"同时用了钱和冰淇淋"的朋友最多只有一个，状态不会出现
// "钱花过一部分、冰淇淋也花过一部分"这种两维都变的情形。
int previous[MAXSUM + 5];
int current[MAXSUM + 5];

int main() {
    int n, moonies, cones;
    scanf("%d %d %d", &n, &moonies, &cones);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d", &friends[i].popularity, &friends[i].moonieCost,
              &friends[i].conePerDiscount);
    }
    sort(friends, friends + n, byConePerDiscount);

    int total = moonies + cones;
    for (int j = 0; j <= total; ++j) {
        previous[j] = -1;  // -1 表示这个状态达不到
    }
    previous[total] = 0;  // 一开始钱和冰淇淋都还没花

    for (int i = 0; i < n; ++i) {
        Friend f = friends[i];
        // 不收买第 i 位朋友：状态原样保留
        for (int j = 0; j <= total; ++j) {
            current[j] = previous[j];
        }
        // 完全用冰淇淋抵掉他索要的 moonie 需要的冰淇淋数
        int conesToFreeHim = f.moonieCost * f.conePerDiscount;
        for (int j = 0; j <= total; ++j) {
            if (previous[j] < 0) {
                continue;
            }
            int gain = previous[j] + f.popularity;
            if (j > moonies) {
                // 只花过冰淇淋的阶段
                if (j - conesToFreeHim >= moonies) {
                    // 冰淇淋够把他全额抵掉，仍然停在"只花冰淇淋"的阶段
                    if (gain > current[j - conesToFreeHim]) {
                        current[j - conesToFreeHim] = gain;
                    }
                } else {
                    // 冰淇淋不够：把剩下的冰淇淋全给他（再多也不够抵完），
                    // 余额用钱补上，从这一步起转入"只花钱"的阶段
                    int discount = (j - moonies) / f.conePerDiscount;
                    int pay = f.moonieCost - discount;
                    if (pay <= moonies && gain > current[moonies - pay]) {
                        current[moonies - pay] = gain;
                    }
                }
            } else if (j >= f.moonieCost) {
                // 冰淇淋已经用完，剩下的朋友只能用钱收买
                if (gain > current[j - f.moonieCost]) {
                    current[j - f.moonieCost] = gain;
                }
            }
        }
        for (int j = 0; j <= total; ++j) {
            previous[j] = current[j];
        }
    }

    // 没花完的钱和冰淇淋都作废，所以在所有可达状态里取最大值
    int answer = 0;
    for (int j = 0; j <= total; ++j) {
        if (previous[j] > answer) {
            answer = previous[j];
        }
    }
    printf("%d\n", answer);
    return 0;
}
