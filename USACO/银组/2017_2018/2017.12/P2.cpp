#include <cstdio>
#include <algorithm>
#include <map>

using namespace std;

const int MAXN = 100000;

struct Measurement {
    int day;   // 测量发生的天
    int cow;   // 被测量的奶牛编号
    int delta; // 相对上次测量产奶量的变化量
};

Measurement measure[MAXN];

// 记录按天数升序排列，题目保证同一天至多一次测量，因此不会有并列。
bool ByDay(const Measurement &a, const Measurement &b) {
    return a.day < b.day;
}

int main() {
    int n, g;
    scanf("%d%d", &n, &g);
    for (int i = 0; i < n; i++) {
        char token[20];
        scanf("%d%d%s", &measure[i].day, &measure[i].cow, token);
        // 变化量带正负号，交给 sscanf 解析出带符号整数。
        sscanf(token, "%d", &measure[i].delta);
    }

    sort(measure, measure + n, ByDay);

    // 产奶量记为其相对初始值 G 的偏移量：所有未在日志中出现的牛永远停在 0。
    // cnt[level] = 处于该偏移量的奶牛头数，背景牛群取 n+1 头（足够多，
    // 保证它们永远不会被耗尽），从而无需单独处理"其他奶牛"。
    map<int, int> cnt;
    cnt[0] = n + 1;
    map<int, int> level; // 奶牛编号 -> 当前偏移量，未出现过的牛默认 0

    int answer = 0;
    for (int i = 0; i < n; i++) {
        int &cur = level[measure[i].cow];

        // 改动前：这头牛是否处于最高产奶量层，以及该层原本有几头牛。
        bool was_top = (cur == cnt.rbegin()->first);
        int was_cnt = cnt[cur];
        if (--cnt[cur] == 0) cnt.erase(cur);

        cur += measure[i].delta;

        // 改动后：这头牛是否处于最高层，以及该层现在有几头牛。
        int now_cnt = ++cnt[cur];
        bool is_top = (cur == cnt.rbegin()->first);

        if (was_top) {
            // 原本它是最高层唯一的牛，改完后仍独占最高层，照片不变；
            // 其余情况（跌下最高层、并列、或最高层变为多头牛）都要更换照片。
            if (!is_top || was_cnt != 1 || now_cnt != 1) answer++;
        } else if (is_top) {
            // 它从非最高层升到了最高层，照片必须换。
            answer++;
        }
        // 既不在最高层、升完也不在最高层的情况不影响展示。
    }

    printf("%d\n", answer);
    return 0;
}
