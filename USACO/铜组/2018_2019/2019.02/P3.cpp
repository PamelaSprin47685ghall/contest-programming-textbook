#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

struct Segment {
    string type;
    int low, high;
};

int main() {
    int n;
    scanf("%d", &n);
    vector<Segment> segs(n);
    char buf[16];
    for (int i = 0; i < n; ++i) {
        scanf("%s%d%d", buf, &segs[i].low, &segs[i].high);
        segs[i].type = buf;
    }

    // 1. 从后往前推算高速入口前 (第 1 段路前) 的可能车流量
    int minFlow = -1000000000;
    int maxFlow = 1000000000;
    for (int i = n - 1; i >= 0; --i) {
        if (segs[i].type == "none") {
            minFlow = max(minFlow, segs[i].low);
            maxFlow = min(maxFlow, segs[i].high);
        } else if (segs[i].type == "on") {
            // 正向时加了 [low, high], 逆向推前则减去 [low, high]
            minFlow -= segs[i].high;
            maxFlow -= segs[i].low;
            minFlow = max(0, minFlow);
        } else if (segs[i].type == "off") {
            // 正向时减了 [low, high], 逆向推前则加上 [low, high]
            minFlow += segs[i].low;
            maxFlow += segs[i].high;
        }
    }
    printf("%d %d\n", minFlow, maxFlow);

    // 2. 从前往后顺推最后一段路后的可能车流量
    minFlow = -1000000000;
    maxFlow = 1000000000;
    for (int i = 0; i < n; ++i) {
        if (segs[i].type == "none") {
            minFlow = max(minFlow, segs[i].low);
            maxFlow = min(maxFlow, segs[i].high);
        } else if (segs[i].type == "on") {
            minFlow += segs[i].low;
            maxFlow += segs[i].high;
        } else if (segs[i].type == "off") {
            minFlow -= segs[i].high;
            maxFlow -= segs[i].low;
            minFlow = max(0, minFlow);
        }
    }
    printf("%d %d\n", minFlow, maxFlow);

    return 0;
}
