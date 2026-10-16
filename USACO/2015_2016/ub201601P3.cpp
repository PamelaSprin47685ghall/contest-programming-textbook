#include <cstdio>
#include <limits>
#include <unordered_map>

// 记录每个坐标上次被割草的时间，重复经过同一坐标时，
// 两次时间之差就是割草机绕一圈回到原处的时间。
struct Point {
    int X, Y;
};

struct PointHash {
    size_t operator()(const Point& p) const {
        return p.X * size_t(9875321) + p.Y;
    }
};

struct PointEq {
    bool operator()(const Point& p0, const Point& p1) const {
        return p0.X == p1.X && p0.Y == p1.Y;
    }
};

int main() {
    int n;
    scanf("%d", &n);

    std::unordered_map<char, Point> step;
    step['E'] = Point{1, 0};
    step['N'] = Point{0, 1};
    step['W'] = Point{-1, 0};
    step['S'] = Point{0, -1};

    int x = 0, y = 0, time = 1;
    std::unordered_map<Point, int, PointHash, PointEq> lastVisit;
    lastVisit[Point{x, y}] = time++;

    int answer = std::numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) {
        char d;
        int count;
        scanf(" %c%d", &d, &count);
        Point move = step[d];
        for (int j = 0; j < count; ++j) {
            x += move.X;
            y += move.Y;
            int& prevTime = lastVisit[Point{x, y}];
            if (prevTime != 0) {
                answer = std::min(answer, time - prevTime);
            }
            prevTime = time++;
        }
    }
    if (answer == std::numeric_limits<int>::max()) {
        printf("-1\n");
    } else {
        printf("%d\n", answer);
    }
    return 0;
}