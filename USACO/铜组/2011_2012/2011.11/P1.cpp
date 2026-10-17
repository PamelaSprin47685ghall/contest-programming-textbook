#include <cstdio>

// 把 11 月 11 日 11:11 记为基准时刻, 计算给定时刻距基准的分钟数.
int MinutesSince(int day, int hour, int minute) {
    return (day * 24 + hour) * 60 + minute;
}

int main() {
    int day, hour, minute;
    scanf("%d%d%d", &day, &hour, &minute);

    int start = MinutesSince(11, 11, 11);
    int answer = MinutesSince(day, hour, minute) - start;

    // Bessie 在基准之前到达时, 比赛尚未开始, 输出 -1.
    if (answer < 0) answer = -1;
    printf("%d\n", answer);
    return 0;
}