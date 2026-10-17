#include <cstdio>
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

// 取绝对值, 参数与返回值均为 64 位整数.
long long AbsLL(long long v) {
    return v < 0 ? -v : v;
}

vector<pair<long long, int> > events;  // (断点位置, 该点处斜率的变化量)

int main() {
    int N;
    scanf("%d", &N);

    long long base = 0;  // 完全不用传送带时的总路程
    for (int i = 0; i < N; i++) {
        long long a, b;
        scanf("%lld%lld", &a, &b);
        base += AbsLL(a - b);

        // 第 i 堆的路程: min(|a-b|, |a| + |b-y|) = |a-b| - max(0, C - |y-b|), C = |a-b| - |a|.
        // 也就是说, 在 y 处建传送带能为这堆省下一个三角形的"高度", 峰值为 C, 支撑区间为 [b-C, b+C].
        long long C = AbsLL(a - b) - AbsLL(a);
        if (C <= 0) continue;  // 传送到 0 号点的路费已经不比直达便宜, 不可能有节省

        events.push_back(make_pair(b - C, 1));   // 进入三角形左坡
        events.push_back(make_pair(b, -2));      // 峰顶, 由升转降
        events.push_back(make_pair(b + C, 1));   // 离开三角形右坡
    }
    sort(events.begin(), events.end());

    // 所有节省量之和是分段线性的, 断点均为整数, 最大值必在某个断点处取到,
    // 因此沿着 y 轴从左往右扫描, 在每个断点处记录当前节省量即可.
    long long bestSaving = 0;
    long long saving = 0;
    long long slope = 0;
    long long prevY = events.empty() ? 0 : events[0].first;
    for (size_t i = 0; i < events.size(); ) {
        long long pos = events[i].first;
        saving += slope * (pos - prevY);  // 顺着当前斜率推进到该断点
        prevY = pos;
        // 同一位置上的所有断裂一次性生效, 节省量在断点处连续, 更新前后取值相同.
        while (i < events.size() && events[i].first == pos) {
            slope += events[i].second;
            i++;
        }
        if (saving > bestSaving) bestSaving = saving;
    }

    printf("%lld\n", base - bestSaving);
    return 0;
}
