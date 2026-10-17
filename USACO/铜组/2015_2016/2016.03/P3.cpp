#include <algorithm>
#include <cstdio>
#include <limits>
#include <vector>

using namespace std;

// 删掉一头牛后，剩下牛的最小包围盒面积。枚举删哪一头，
// 用前缀/后缀包围盒 O(1) 得到"去掉第 i 头"后的包围盒。
struct Point {
    int X, Y;
};

struct Bound {
    int X0, X1, Y0, Y1; // 包围盒的左右上下边界
};

Bound emptyBound() {
    return {numeric_limits<int>::max(),
            numeric_limits<int>::min(),
            numeric_limits<int>::max(),
            numeric_limits<int>::min()};
}

Bound fromPoint(const Point& p) {
    return {p.X, p.X, p.Y, p.Y};
}

// 合并两个包围盒，得到同时包含两者的最小矩形。
Bound combine(const Bound& b0, const Bound& b1) {
    return {min(b0.X0, b1.X0),
            max(b0.X1, b1.X1),
            min(b0.Y0, b1.Y0),
            max(b0.Y1, b1.Y1)};
}

int sizeOf(const Bound& b) {
    return (b.X1 - b.X0) * (b.Y1 - b.Y0);
}

int main() {
    int n;
    scanf("%d", &n);
    vector<Point> cows(n);
    for (Point& c : cows) scanf("%d%d", &c.X, &c.Y);

    // prefix[i]：前 i 头牛的包围盒；suffix[i]：从第 i 头到最后的包围盒
    vector<Bound> prefix(n + 1), suffix(n + 1);
    prefix[0] = emptyBound();
    for (int i = 1; i <= n; ++i) {
        prefix[i] = combine(prefix[i - 1], fromPoint(cows[i - 1]));
    }
    suffix[n] = emptyBound();
    for (int i = n - 1; i >= 0; --i) {
        suffix[i] = combine(suffix[i + 1], fromPoint(cows[i]));
    }

    int answer = numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) {
        // 去掉第 i 头：左边 prefix[i] 与右边 suffix[i+1] 的并
        Bound without = combine(prefix[i], suffix[i + 1]);
        answer = min(answer, sizeOf(without));
    }
    printf("%d\n", answer);
    return 0;
}