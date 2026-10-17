// USACO 2020 US Open Contest, Silver P3: The Moo Particle
#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

struct Particle {
    long long x, y;
};

int n;
Particle particle[MAXN];
long long minLeft[MAXN];    // minLeft[i]：排序后前 i+1 个粒子的最小 y
long long maxRight[MAXN];   // maxRight[i]：排序后第 i..n-1 个粒子的最大 y

bool cmpParticle(const Particle &a, const Particle &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld %lld", &particle[i].x, &particle[i].y);
    }

    // 把能互相作用的粒子连边。一次作用等于删掉一个还有邻居的点，
    // 所以每个连通分量至少剩一个粒子，且恰能只靠删叶子做到只剩一个。
    // 于是答案就是连通分量个数，关键在 O(N log N) 内数出它。
    sort(particle, particle + n, cmpParticle);

    minLeft[0] = particle[0].y;
    for (int i = 1; i < n; ++i) {
        minLeft[i] = min(minLeft[i - 1], particle[i].y);
    }
    maxRight[n - 1] = particle[n - 1].y;
    for (int i = n - 2; i >= 0; --i) {
        maxRight[i] = max(maxRight[i + 1], particle[i].y);
    }

    // 按 x 排序后，第 i 与第 i+1 个粒子不在同一连通分量，
    // 当且仅当前缀最小 y 大于后缀最大 y（此时两边的 x、y 都无法配对）
    int answer = 1;
    for (int i = 0; i + 1 < n; ++i) {
        if (minLeft[i] > maxRight[i + 1]) {
            ++answer;
        }
    }

    printf("%d\n", answer);
    return 0;
}
