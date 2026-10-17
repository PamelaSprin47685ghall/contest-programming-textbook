#include <cstdio>
#include <vector>

using namespace std;

// 一段连续同 ID 的牛. Good 表示删掉 GoodID 这种牛后, 本段能与
// 前面合并出的最长连续段长度; 没有可合并对象时 GoodID = -1.
struct Segment {
    int id;
    int bad;      // 本段自身长度
    int good;
    int goodID;
};

int main() {
    int n;
    scanf("%d", &n);
    vector<int> cows(n);
    for (int& cow : cows) scanf("%d", &cow);

    vector<Segment> segments;
    int answer = 0;

    // 把连续相同的 ID 合并成一段, 逐段计算.
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && cows[j] == cows[i]) ++j;

        Segment seg;
        seg.id = cows[i];
        seg.bad = j - i;
        seg.goodID = -1;
        seg.good = 0;
        if (seg.bad > answer) answer = seg.bad;

        // 与前面隔一段的段比较: 若 ID 相同, 删掉中间那种牛就能合并.
        int size = (int)segments.size();
        if (size >= 2 && segments[size - 2].id == seg.id) {
            seg.goodID = segments[size - 1].id;
            int extend = segments[size - 2].bad;
            // 若中间段本身也连着更早的同 ID 段, 可以一并接上.
            if (seg.goodID == segments[size - 2].goodID) {
                extend = segments[size - 2].good;
            }
            seg.good = seg.bad + extend;
            if (seg.good > answer) answer = seg.good;
        }

        segments.push_back(seg);
        i = j;
    }

    printf("%d\n", answer);
    return 0;
}