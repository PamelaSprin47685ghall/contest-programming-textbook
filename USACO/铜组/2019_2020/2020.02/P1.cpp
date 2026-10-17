#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace std;

struct Point {
    int x, y;
};

int main() {
    int n;
    scanf("%d", &n);
    vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &pts[i].x, &pts[i].y);
    }

    int maxTwiceArea = 0;

    // 枚举直角顶点 i, 以及平行于 x 轴的另一点 j, 平行于 y 轴的另一点 k
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (pts[i].y != pts[j].y) continue;  // ij 平行于 x 轴
            for (int k = 0; k < n; ++k) {
                if (pts[i].x != pts[k].x) continue;  // ik 平行于 y 轴

                int twiceArea = abs(pts[j].x - pts[i].x) * abs(pts[k].y - pts[i].y);
                if (twiceArea > maxTwiceArea) {
                    maxTwiceArea = twiceArea;
                }
            }
        }
    }

    printf("%d\n", maxTwiceArea);
    return 0;
}
