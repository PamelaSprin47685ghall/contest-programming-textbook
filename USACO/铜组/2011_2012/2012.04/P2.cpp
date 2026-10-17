#include <cstdio>
#include <vector>

using namespace std;

struct Points {
    int n;
    vector<int> x, y;

    void Read() {
        scanf("%d", &n);
        x.resize(n);
        y.resize(n);
        for (int i = 0; i < n; ++i) scanf("%d%d", &x[i], &y[i]);
    }
};

struct Line {
    bool vertical;  // true 表示 x 坐标固定, false 表示 y 坐标固定
    int value;
};

// 判断第 no 个点是否被某条已选直线覆盖.
bool Covered(const Points& points, const vector<Line>& lines, int no) {
    for (const Line& line : lines) {
        int coordinate = line.vertical ? points.x[no] : points.y[no];
        if (coordinate == line.value) return true;
    }
    return false;
}

// 逐个处理点, 未覆盖的点必须新开一条横线或竖线, 总数不超过 3.
bool Search(const Points& points, vector<Line>& lines, int no) {
    if (no == points.n) return true;
    if (Covered(points, lines, no)) return Search(points, lines, no + 1);
    if ((int)lines.size() >= 3) return false;

    // 尝试用竖线或横线覆盖当前点.
    lines.push_back(Line{true, points.x[no]});
    if (Search(points, lines, no + 1)) return true;
    lines.pop_back();

    lines.push_back(Line{false, points.y[no]});
    if (Search(points, lines, no + 1)) return true;
    lines.pop_back();
    return false;
}

int main() {
    Points points;
    points.Read();
    vector<Line> lines;
    printf("%d\n", Search(points, lines, 0) ? 1 : 0);
    return 0;
}