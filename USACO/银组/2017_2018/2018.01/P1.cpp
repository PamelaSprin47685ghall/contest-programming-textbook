#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

// 每个值班端点是一条事件：时刻 time、编号 id 的救生员开始或结束值班
struct Event {
    int time;
    int id;
    bool isStart;
};

Event event[2 * MAXN];
int alone[MAXN]; // alone[i] = 只有第 i 个救生员在岗的总时长，开除他就会损失这么多时间

// 按时刻排序；题目保证所有端点互不相同，所以不会出现同一时刻的并列事件
bool byTime(const Event& a, const Event& b) {
    return a.time < b.time;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int start, end;
        scanf("%d %d", &start, &end);
        event[2 * i].time = start;
        event[2 * i].id = i;
        event[2 * i].isStart = true;
        event[2 * i + 1].time = end;
        event[2 * i + 1].id = i;
        event[2 * i + 1].isStart = false;
    }
    sort(event, event + 2 * n, byTime);

    // 两个相邻事件之间的那段时间里，在岗集合是固定的，可以整体结算。
    // 为了把“独处时间”记到人头上，除了在岗人数 activeCount 之外，
    // 再维护在岗人员编号之和 activeIdSum：只有一个人在岗时，编号之和
    // 恰好等于他的编号，于是不用额外维护集合也能知道是谁在独处。
    int activeCount = 0;
    long long activeIdSum = 0;
    long long covered = 0; // 所有救生员共同覆盖的总时长
    int lastTime = event[0].time;
    for (int i = 0; i < 2 * n; i++) {
        int length = event[i].time - lastTime;
        if (activeCount > 0) covered += length;
        if (activeCount == 1) alone[(int)activeIdSum] += length;
        if (event[i].isStart) {
            activeCount++;
            activeIdSum += event[i].id;
        } else {
            activeCount--;
            activeIdSum -= event[i].id;
        }
        lastTime = event[i].time;
    }

    // 开除第 i 个救生员，剩下的覆盖时长是 covered - alone[i]，取最大值。
    // n = 1 时 alone[0] 就等于 covered，正确输出 0。
    long long best = 0;
    for (int i = 0; i < n; i++) {
        long long remain = covered - alone[i];
        if (remain > best) best = remain;
    }
    printf("%lld\n", best);
    return 0;
}
