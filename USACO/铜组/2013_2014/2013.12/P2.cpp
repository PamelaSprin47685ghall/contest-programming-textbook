#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

// 枚举 X, Y 两头牛, 对每个差值 d, 统计 Z 落在 [Y+d, Y+2d] 的数量.
// 排序后二分找到区间两端的位置, 相减即得该区间的牛数.
struct Cows {
    vector<int> positions;

    void Read() {
        int n;
        scanf("%d", &n);
        positions.resize(n);
        for (int& p : positions) scanf("%d", &p);
        sort(positions.begin(), positions.end());
    }

    int Solve() const {
        int answer = 0;
        int count = (int)positions.size();
        for (int i = 0; i < count; ++i) {
            for (int j = i + 1; j < count; ++j) {
                if (positions[i] == positions[j]) continue;
                int d = positions[j] - positions[i];
                // Z 必须满足 Y+d <= Z <= Y+2d, 即 Z 在 [Y+d, Y+2d+1) 内.
                int left = CountLess(positions[j] + d);
                int right = CountLess(positions[j] + d + d + 1);
                answer += right - left;
            }
        }
        return answer;
    }

    // 位置严格小于 end 的牛的数量.
    int CountLess(int end) const {
        return lower_bound(positions.begin(), positions.end(), end) -
               positions.begin();
    }
};

int main() {
    Cows cows;
    cows.Read();
    printf("%d\n", cows.Solve());
    return 0;
}