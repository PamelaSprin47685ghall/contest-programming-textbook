#include <cstdio>
#include <limits>
#include <vector>

// 枚举最终高度区间 [low, low+17], 计算把所有山修进区间的代价, 取最小.
struct Mountains {
    std::vector<int> heights;

    void Read() {
        int n;
        scanf("%d", &n);
        heights.resize(n);
        for (int& h : heights) scanf("%d", &h);
    }

    int Solve() const {
        int answer = std::numeric_limits<int>::max();
        for (int low = 0; low <= 100; ++low) {
            answer = std::min(answer, Cost(low));
        }
        return answer;
    }

    // 把低于 low 的山垫高, 高于 high 的山削低, 代价是高度差的平方和.
    int Cost(int low) const {
        int high = low + 17;
        int answer = 0;
        for (int h : heights) {
            if (h < low) answer += (low - h) * (low - h);
            if (high < h) answer += (h - high) * (h - high);
        }
        return answer;
    }
};

int main() {
    Mountains mountains;
    mountains.Read();
    printf("%d\n", mountains.Solve());
    return 0;
}