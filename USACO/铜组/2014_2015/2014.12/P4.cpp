#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

struct Sample {
    char kind;      // 'S' 表示会学习，'N' 表示不会。
    int position;
};

// 按位置排序；同位置时按字母排序，保证相邻样本种类不同时能稳定取中点。
bool operator<(const Sample& s0, const Sample& s1) {
    if (s0.position != s1.position) return s0.position < s1.position;
    return s0.kind < s1.kind;
}

int main() {
    int n, a, b;
    scanf("%d%d%d", &n, &a, &b);
    vector<Sample> samples(n), events;
    for (int i = 0; i < n; ++i) {
        scanf(" %cS", &samples[i].kind);
        scanf("%d", &samples[i].position);
    }
    sort(samples.begin(), samples.end());
    // 两种相邻奶牛的分界点取中点；isN 为真时中点偏右，保证分界落在正确一侧。
    auto midpoint = [](int left, int right, bool isN) {
        return left + (right - left + 1 + (isN ? 1 : 0)) / 2;
    };
    for (int i = 1; i < (int)samples.size(); ++i) {
        if (samples[i - 1].kind == samples[i].kind) continue;
        Sample event{samples[i].kind,
                     midpoint(samples[i - 1].position, samples[i].position,
                              samples[i].kind == 'N')};
        events.push_back(event);
    }
    // 用两个哨兵事件圈出 [a, b] 查询区间。
    events.push_back(Sample{'Z', a});
    events.push_back(Sample{'Z', b + 1});
    sort(events.begin(), events.end());
    Sample last{samples[0].kind, 0};
    int answer = 0;
    bool inRange = false;
    for (const Sample& e : events) {
        // 当前处于查询区间且该段是 'S'，则这段位置都计入答案。
        if (inRange && last.kind == 'S') answer += e.position - last.position;
        if (e.kind != 'Z') last.kind = e.kind;
        last.position = e.position;
        if (e.kind == 'Z') inRange = !inRange;
    }
    printf("%d\n", answer);
    return 0;
}
