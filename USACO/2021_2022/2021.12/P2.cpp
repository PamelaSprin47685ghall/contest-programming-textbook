#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> p(n);
    std::vector<int> t(n);
    for (int i = 0; i < n; ++i) scanf("%d", &p[i]);
    for (int i = 0; i < n; ++i) scanf("%d", &t[i]);

    // diff[i]: 第 i 个隔间需要的温度变化量 (目标温度 - 当前温度)
    std::vector<int> diff(n);
    for (int i = 0; i < n; ++i) {
        diff[i] = p[i] - t[i];
    }

    // 任何一次区间加减操作只能处理全正区间或全负区间 (正负不能混在同一步消除)
    // 因此符号相同且绝对值增加时才需要启动新的操作层
    long long answer = 0;
    int prev = 0;
    for (int i = 0; i < n; ++i) {
        int d = diff[i];
        if (d >= 0 && prev >= 0) {
            if (d > prev) answer += (d - prev);
        } else if (d <= 0 && prev <= 0) {
            if (d < prev) answer += (prev - d);
        } else if (d >= 0 && prev < 0) {
            answer += d;
        } else if (d < 0 && prev > 0) {
            answer += -d;
        }
        prev = d;
    }

    printf("%lld\n", answer);
    return 0;
}
