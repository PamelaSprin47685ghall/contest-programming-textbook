#include <cstdio>
#include <cmath>
#include <queue>
#include <algorithm>

using namespace std;

const int MAXN = 50000;
const double PI = 3.14159265358979324;
const double TWO_PI = 2.0 * PI;

struct Arc {
    double from; // 这头奶牛能看见的谷仓圆弧的起始角 (逆时针方向)
    double to;   // 结束角, 即 from + 2 * alpha

    // 按起始角从小到大排序, 供 sort 使用
    bool operator<(const Arc &other) const {
        return from < other.from;
    }
};

// 保存结束角的小根堆: priority_queue 默认是大根堆, 把比较反过来即可
struct Endpoint {
    double angle;

    bool operator<(const Endpoint &other) const {
        return angle > other.angle;
    }
};

int n, radius;
Arc arc[MAXN];

int main() {
    scanf("%d %d", &n, &radius);
    for (int i = 0; i < n; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        double dist = sqrt((double)x * x + (double)y * y);
        // 从奶牛处看谷仓, 两条切线张角的一半; 这两条切线之间的那段圆弧
        // 就是这头奶牛能看见的谷仓边缘
        double alpha = acos(radius / dist);
        double from = atan2(y, x) - alpha;
        if (from < 0) from += TWO_PI; // 统一到 [0, 2*PI)
        arc[i].from = from;
        arc[i].to = from + 2.0 * alpha;
    }
    sort(arc, arc + n);

    // 想象一条贴着谷仓转动的切线: 切线所在角度等于当前处理的 arc[i].from 时,
    // 结束角在此之前 (= 已经转到切线背面去) 的奶牛都看不见了, 出堆。
    // 两头奶牛能互相看见 <=> 它们的可见圆弧相交, 于是每处理一头奶牛, 堆里
    // 留下的就是"能看见这头奶牛所看到的那段圆弧"的那些奶牛, 把它们数进来。
    // 绕两圈才计数是为了让跨越 0 度 (即圆弧回绕) 的奶牛也被正确地算一次。
    priority_queue<Endpoint> pq;
    long long answer = 0;
    for (int lap = 0; lap < 2; lap++) {
        for (int i = 0; i < n; i++) {
            while (!pq.empty() && pq.top().angle < arc[i].from) pq.pop();
            if (lap == 1) answer += (long long)pq.size();
            Endpoint e;
            e.angle = arc[i].to;
            pq.push(e);
            arc[i].from += TWO_PI; // 第二圈: 角度整体加一圈
            arc[i].to += TWO_PI;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
