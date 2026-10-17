#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Cow {
    int s, t, c;
};

struct AC {
    int a, b, p, cost;
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    vector<Cow> cows(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d", &cows[i].s, &cows[i].t, &cows[i].c);
    }
    vector<AC> acs(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d%d", &acs[i].a, &acs[i].b, &acs[i].p, &acs[i].cost);
    }

    int minCost = 1000000000;

    // m <= 10, 直接暴力枚举使用空调的所有 2^m 种子集状态
    for (int mask = 0; mask < (1 << m); ++mask) {
        int currentCost = 0;
        int cool[101] = {};

        for (int i = 0; i < m; ++i) {
            if ((mask >> i) & 1) {
                currentCost += acs[i].cost;
                for (int pos = acs[i].a; pos <= acs[i].b; ++pos) {
                    cool[pos] += acs[i].p;
                }
            }
        }

        // 验证每头牛所在的每个隔间制冷量是否均满足要求
        bool valid = true;
        for (const Cow& cow : cows) {
            for (int pos = cow.s; pos <= cow.t; ++pos) {
                if (cool[pos] < cow.c) {
                    valid = false;
                    break;
                }
            }
            if (!valid) break;
        }

        if (valid && currentCost < minCost) {
            minCost = currentCost;
        }
    }

    printf("%d\n", minCost);
    return 0;
}
