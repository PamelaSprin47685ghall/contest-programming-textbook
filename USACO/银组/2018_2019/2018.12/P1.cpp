#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n, busLimit, capacity;
int arrival[MAXN];

// 判断"最大等待时间不超过 wait"是否可行: 用贪心算出所需的最少巴士数.
// 排序后, 第一辆巴士不必早于"第一位乘客到达后 wait 时刻"发车: 提早发车只会
// 留下本可上车的奶牛; 再晚就会让先到的奶牛超时. 因此它要么装满 capacity 头,
// 要么带走 wait 时间内到达的全部奶牛, 取其中先结束的一批; 后续巴士同理.
bool Feasible(int wait) {
    int buses = 1;
    int firstArrival = arrival[0];  // 当前巴士所载第一位乘客的到达时间
    int firstIndex = 0;             // 她在排序后数组中的下标
    for (int i = 1; i < n; ++i) {
        if (arrival[i] - firstArrival > wait || i + 1 - firstIndex > capacity) {
            ++buses;
            firstArrival = arrival[i];
            firstIndex = i;
        }
    }
    return buses <= busLimit;
}

int main() {
    scanf("%d%d%d", &n, &busLimit, &capacity);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arrival[i]);
    }
    sort(arrival, arrival + n);

    // 可行性关于 wait 单调: 上限放宽后原方案依然合法, 所以能二分最小可行值.
    // 取 wait 为最大到达时间差时, 窗口约束自动满足, 巴士数只受 capacity 限制,
    // 由 MC >= N 保证可行, 因此右端点可以直接用这个差.
    int low = 0, high = arrival[n - 1] - arrival[0];
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (Feasible(mid)) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    printf("%d\n", low);
    return 0;
}
