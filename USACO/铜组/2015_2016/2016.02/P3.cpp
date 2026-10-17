#include <algorithm>
#include <cstdio>
#include <limits>
#include <vector>

using namespace std;

// 坐标范围很大但奶牛数很少，先把所有 X、Y 离散化成排名，
// 再用二维前缀和快速统计任意矩形区域里的奶牛数。
struct Point {
    int X, Y;
};

// 去重排序，返回所有互不相同的坐标值。
vector<int> uniqueValues(vector<int> values) {
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    return values;
}

// 坐标 v 在离散化后的排名（0 起）。
int rankOf(const vector<int>& values, int v) {
    return lower_bound(values.begin(), values.end(), v) - values.begin();
}

int main() {
    int n;
    scanf("%d%*d", &n); // 第二个数是坐标上界，离散化后不再需要
    vector<Point> cows(n);
    for (Point& c : cows) scanf("%d%d", &c.X, &c.Y);

    vector<int> xs, ys;
    for (const Point& c : cows) {
        xs.push_back(c.X);
        ys.push_back(c.Y);
    }
    vector<int> uniqueX = uniqueValues(xs);
    vector<int> uniqueY = uniqueValues(ys);
    int xCount = uniqueX.size();
    int yCount = uniqueY.size();

    // pSum[i][j]：左下 (0,0) 到右上 (i,j) 矩形内的奶牛数
    vector<vector<int>> pSum(xCount, vector<int>(yCount, 0));
    for (const Point& c : cows) {
        ++pSum[rankOf(uniqueX, c.X)][rankOf(uniqueY, c.Y)];
    }
    for (int i = 0; i < xCount; ++i) {
        for (int j = 1; j < yCount; ++j) {
            pSum[i][j] += pSum[i][j - 1];
        }
    }
    for (int i = 1; i < xCount; ++i) {
        for (int j = 0; j < yCount; ++j) {
            pSum[i][j] += pSum[i - 1][j];
        }
    }

    // 栅栏放在排名 i 与 i+1、排名 j 与 j+1 之间，四个象限
    // 的奶牛数由前缀和相减得到，代价取其中最大者。
    int answer = numeric_limits<int>::max();
    for (int i = 0; i < xCount; ++i) {
        for (int j = 0; j < yCount; ++j) {
            int s00 = pSum[i][j];
            int s01 = pSum[i][yCount - 1] - pSum[i][j];
            int s10 = pSum[xCount - 1][j] - pSum[i][j];
            int s11 = pSum[xCount - 1][yCount - 1] - s00 - s01 - s10;
            int worst = max(max(s00, s01), max(s10, s11));
            answer = min(answer, worst);
        }
    }
    printf("%d\n", answer);
    return 0;
}