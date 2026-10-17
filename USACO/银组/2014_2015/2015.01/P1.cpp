#include <algorithm>
#include <cstdio>
#include <set>
#include <utility>
#include <vector>

using namespace std;

// 每头奶牛都等价于在 FJ 的视线 (直线 x=0) 上停留的一段时间:
// 右端点走到 x=0 时进入视线, 左端点走过 x=0 时离开视线.
// 于是问题变成: 扫描时间轴, 每个"有牛在视线上"的时刻, 记录下 y 最小的那头牛.
// y 互不相同, 所以"最靠前"的奶牛唯一, 不会出现并列看不清的情况.
int main() {
    int N;
    scanf("%d", &N);

    // 事件用带符号的 y 编码: 正数表示进入视线, 负数表示离开,
    // 这样排序后按时间顺序处理即可, 同一时刻也能一眼看出是进还是出.
    vector<pair<long long, int> > events;
    for (int i = 0; i < N; i++) {
        int x, y, r;
        scanf("%d %d %d", &x, &y, &r);
        long long enter = -(long long)(x + 1) * r;  // 右端点到达 x=0 的时刻
        long long leave = -(long long)x * r;        // 左端点到达 x=0 的时刻
        events.push_back(make_pair(enter, y));
        events.push_back(make_pair(leave, -y));
    }
    sort(events.begin(), events.end());

    set<int> visible;  // 此刻压在视线上的奶牛; 队首就是最靠前 (y 最小) 的那头
    vector<char> seen(1000001, 0);  // y 各不相同, 可以直接用 y 标记这头牛是否被看到过
    int answer = 0;
    for (int i = 0; i < (int)events.size(); ) {
        // 同一时刻的进出事件必须一起处理: 那一瞬间两头奶牛都盖在视线上,
        // 谁都不能漏, 处理完再看这一瞬间最靠前的是谁.
        int j = i;
        while (j < (int)events.size() && events[j].first == events[i].first) {
            int y = events[j].second;
            if (y > 0) visible.insert(y);
            else visible.erase(-y);
            j++;
        }
        if (!visible.empty()) {
            int front = *visible.begin();
            if (!seen[front]) {
                seen[front] = 1;
                answer++;
            }
        }
        i = j;
    }

    printf("%d\n", answer);
    return 0;
}
