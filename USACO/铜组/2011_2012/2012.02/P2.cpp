#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Rectangle {
    int left, right, bottom, top;
};

// 读入矩形. 注意: 输入顺序是 (left, top, right, bottom),
// 即第 2 个数是最高的 y, 第 4 个数是最低的 y.
Rectangle ReadRectangle() {
    int x0, y1, x1, y0;
    scanf("%d%d%d%d", &x0, &y1, &x1, &y0);
    Rectangle r;
    r.left = x0;
    r.right = x1;
    r.bottom = y0;
    r.top = y1;
    return r;
}

// 读入 n 个矩形, 返回离散化后的 x 坐标列表和 y 坐标列表.
// 离散化: 把所有用到的坐标排序去重, 这样网格大小只与坐标个数有关.
void Discretize(const vector<Rectangle>& rects,
                vector<int>& xs, vector<int>& ys) {
    for (const Rectangle& r : rects) {
        xs.push_back(r.left);
        xs.push_back(r.right);
        ys.push_back(r.bottom);
        ys.push_back(r.top);
    }
    sort(xs.begin(), xs.end());
    xs.resize(unique(xs.begin(), xs.end()) - xs.begin());
    sort(ys.begin(), ys.end());
    ys.resize(unique(ys.begin(), ys.end()) - ys.begin());
}

int main() {
    int n;
    scanf("%d", &n);
    vector<Rectangle> rects(n);
    for (Rectangle& r : rects) r = ReadRectangle();

    vector<int> xs, ys;
    Discretize(rects, xs, ys);

    // 网格单元 (i, j) 表示 [xs[i], xs[i+1]) x [ys[j], ys[j+1]) 的区间.
    vector<vector<bool>> covered(
        xs.size() - 1, vector<bool>(ys.size() - 1, false));

    // 每个矩形把覆盖到的网格单元标记为真.
    for (const Rectangle& r : rects) {
        int leftIndex = lower_bound(xs.begin(), xs.end(), r.left) - xs.begin();
        int rightIndex = lower_bound(xs.begin(), xs.end(), r.right) - xs.begin();
        int bottomIndex = lower_bound(ys.begin(), ys.end(), r.bottom) - ys.begin();
        int topIndex = lower_bound(ys.begin(), ys.end(), r.top) - ys.begin();
        for (int i = leftIndex; i < rightIndex; ++i) {
            for (int j = bottomIndex; j < topIndex; ++j) {
                covered[i][j] = true;
            }
        }
    }

    // 累加所有被覆盖单元的真实面积 (离散化区间宽度之积).
    long long answer = 0;
    for (int i = 0; i < (int)xs.size() - 1; ++i) {
        for (int j = 0; j < (int)ys.size() - 1; ++j) {
            if (covered[i][j]) {
                answer += (long long)(xs[i + 1] - xs[i]) * (ys[j + 1] - ys[j]);
            }
        }
    }
    printf("%lld\n", answer);
    return 0;
}