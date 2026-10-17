#include <cstdio>
#include <algorithm>

using namespace std;

// N <= 100000，速度 <= 10^6，L <= 25000，所以 L*v 最大 2.5x10^10，要用 long long
const int MAXN = 100000;

int speed[MAXN];
int fracValue[MAXN];      // 小数部分的分子：L*v 除以最快速度的余数
int distinctFrac[MAXN];   // 去重后的余数，升序

long long tree[MAXN + 1];   // 树状数组，统计已经处理过的奶牛里各名次有多少头
int treeSize = 0;

void add(int position) {
    for (int i = position; i <= treeSize; i += i & (-i)) ++tree[i];
}

long long prefixSum(int position) {
    long long total = 0;
    for (int i = position; i > 0; i -= i & (-i)) total += tree[i];
    return total;
}

// 在去重升序数组里找 value 的名次（从 1 开始）。value 一定在里面，
// 二分找到第一个不小于它的位置就是它自己。
int rankOf(int value) {
    int low = 0, high = treeSize - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (distinctFrac[mid] < value) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low + 1;
}

int main() {
    int n = 0;
    long long laps = 0, track = 0;
    // 跑道长度 track 只影响比赛的绝对时长，圈数与超越次数都不依赖它，读了备用
    scanf("%d %lld %lld", &n, &laps, &track);
    int maxSpeed = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &speed[i]);
        if (speed[i] > maxSpeed) maxSpeed = speed[i];
    }

    // 按速度升序处理，速度相同就是跑过的圈数相同，彼此不会互相超越。
    sort(speed, speed + n);
    for (int i = 0; i < n; ++i) fracValue[i] = (int)(laps * speed[i] % maxSpeed);

    for (int i = 0; i < n; ++i) distinctFrac[i] = fracValue[i];
    sort(distinctFrac, distinctFrac + n);
    treeSize = 0;
    for (int i = 0; i < n; ++i) {
        if (treeSize == 0 || distinctFrac[treeSize - 1] != distinctFrac[i]) {
            distinctFrac[treeSize] = distinctFrac[i];
            ++treeSize;
        }
    }

    // 圈数 L(i) = laps * speed[i] / maxSpeed。对速度更慢的奶牛 j，
    // floor(L(i) - L(j)) 就是奶牛 i 套过奶牛 j 的圈数，也就是交叉次数。
    // 写成 floor_i - floor_j，但小数部分大的那头会多算 1，需要减回去。
    long long total = 0;         // 答案
    long long floorSum = 0;      // 已处理奶牛的整数圈数之和
    for (int i = 0; i < n; ++i) {
        long long value = laps * speed[i];
        long long wholeLaps = value / maxSpeed;
        // 按速度升序，前面 i 头奶牛都是更慢的，每头贡献 wholeLaps - 它的整数圈数
        total += (long long)i * wholeLaps - floorSum;
        // 小数部分比当前奶牛大的那些，floor(L(i)-L(j)) 其实比整数差少 1
        long long notGreater = prefixSum(rankOf(fracValue[i]));
        total -= (long long)i - notGreater;
        floorSum += wholeLaps;
        add(rankOf(fracValue[i]));
    }

    printf("%lld\n", total);
    return 0;
}
