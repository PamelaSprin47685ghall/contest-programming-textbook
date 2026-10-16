#include <algorithm>
#include <cstdio>
#include <vector>

// 一段必须被同一张照片覆盖的区间.
struct Segment {
    int begin, end;
};

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    std::vector<Segment> segments;
    segments.resize(k);
    for (int i = 0; i < k; ++i) {
        scanf("%d%d", &segments[i].begin, &segments[i].end);
        if (segments[i].end < segments[i].begin) {
            std::swap(segments[i].begin, segments[i].end);
        }
    }
    // 按右端点排序, 贪心: 每张照片尽量覆盖到最右, 减少照片数.
    std::sort(segments.begin(), segments.end(), [](const Segment& s0, const Segment& s1) {
        return s0.end < s1.end;
    });
    int coveredUntil = 0;
    int answer = 0;
    for (const Segment& s : segments) {
        if (s.begin <= coveredUntil) continue;
        coveredUntil = s.end - 1;
        ++answer;
    }
    // 照片本身占一张, 加上额外需要的张数.
    printf("%d\n", answer + 1);
    return 0;
}