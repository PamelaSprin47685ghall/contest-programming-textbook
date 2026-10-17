#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

int n;
long long m;
long long startPos[MAXN];  // 每头奶牛的上车位置, 末尾还有一个虚拟的 "下车后空车回到 0" 的起点
long long endPos[MAXN];    // 每头奶牛的下车位置, 末尾还有虚拟行程的终点

int main() {
    scanf("%d %lld", &n, &m);
    long long total = 0;
    for (int i = 0; i < n; i++) {
        long long s, t;
        scanf("%lld %lld", &s, &t);
        startPos[i] = s;
        endPos[i] = t;
        // 载着客人时无论如何都得从 s 开到 t, 这段路程一定被走一次
        total += s > t ? s - t : t - s;
    }

    // Bessie 最后要停在 M, 相当于她还要把一辆空车从 M 送回 0;
    // 空车行驶的里程只取决于把每个 "下车点" 和下一个 "上车点" 怎么配对
    startPos[n] = m;
    endPos[n] = 0;

    // 把上车位置和下车位置分别排序后对应配对, 总空驶距离最小:
    // 若存在两对交错 (a1 < a2 而 b1 > b2), 交换它们只会让 |a1-b1|+|a2-b2| 更小
    sort(startPos, startPos + n + 1);
    sort(endPos, endPos + n + 1);
    for (int i = 0; i <= n; i++)
        total += startPos[i] > endPos[i] ? startPos[i] - endPos[i] : endPos[i] - startPos[i];

    printf("%lld\n", total);
    return 0;
}
