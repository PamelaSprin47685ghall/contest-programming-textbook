#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>

// 对每个起点模拟爆炸扩散：第 round 轮能引爆距离任一已爆点不超过 round 的牛。
// 用两个集合分别维护"已爆炸"和"未爆炸"的牛，逐轮推进。
struct Cows {
    int N;
    std::vector<int> X;
};

void read(Cows& c) {
    scanf("%d", &c.N);
    c.X.resize(c.N);
    for (int& x : c.X) scanf("%d", &x);
    std::sort(c.X.begin(), c.X.end());
}

// 从第 no 头牛开始爆炸，返回最终被引爆的牛数。
// 第 round 轮只由上一轮刚引爆的牛（front）向外扩散：
// 与 front 中任一牛距离不超过 round 的牛在本轮引爆。
int explode(const Cows& c, int no) {
    std::set<int> remaining(c.X.begin(), c.X.end());
    std::set<int> front; // 上一轮刚引爆的牛
    front.insert(no);
    remaining.erase(no);
    int answer = 1; // 起点本身
    for (int round = 1; !front.empty(); ++round) {
        std::set<int> nextFront;
        std::set<int> stillRemaining;
        for (int x : remaining) {
            bool near = false;
            for (int q : front) {
                if (std::abs(q - x) <= round) {
                    near = true;
                    break;
                }
            }
            if (near) {
                ++answer;
                nextFront.insert(x);
            } else {
                stillRemaining.insert(x);
            }
        }
        front.swap(nextFront);
        remaining.swap(stillRemaining);
    }
    return answer;
}

int main() {
    Cows c;
    read(c);
    int answer = 0;
    for (int i = 0; i < c.N; ++i) {
        // 起点是坐标值 X[i]，而不是下标 i
        answer = std::max(answer, explode(c, c.X[i]));
    }
    printf("%d\n", answer);
    return 0;
}