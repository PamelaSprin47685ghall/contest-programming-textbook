#include <algorithm>
#include <cstdio>

// 计算在终点限速不超过 x 的条件下跑完 k 米所需的最少秒数
int Solve(long long k, long long x) {
    long long distLeft = 0;   // 从起点开始加速跑过的距离
    long long distRight = 0;  // 终点前减速跑过的距离
    long long timeSpent = 0;

    for (long long speed = 1; ; ++speed) {
        // 加速段前进 speed
        distLeft += speed;
        ++timeSpent;
        if (distLeft + distRight >= k) return timeSpent;

        // 如果当前速度已经超过了终点限速 x, 那么在靠近终点时也需要以该速度降速跑过对应的距离
        if (speed >= x) {
            distRight += speed;
            ++timeSpent;
            if (distLeft + distRight >= k) return timeSpent;
        }
    }
}

int main() {
    long long k;
    int n;
    scanf("%lld%d", &k, &n);
    for (int i = 0; i < n; ++i) {
        long long x;
        scanf("%lld", &x);
        printf("%d\n", Solve(k, x));
    }
    return 0;
}
