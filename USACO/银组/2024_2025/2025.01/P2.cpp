#include <cstdio>
#include <algorithm>

using namespace std;

// 思路：把每个 a_i 换成它对 M 的余数。可以证明最优的 x 一定能取成某个余数本身：
// 若 x 不等于任何余数，则 g(x) 在该点附近是凹的，两侧必有取值不比它差的点。
// 于是只需枚举 x = 各个余数：对所有与 x 在模 M 意义下距离不超过 M/2 的元素求和，
// 距离取 min((a_i - x) mod M, (x - a_i) mod M)。
// 为了避开取模的边界处理，把排好序的余数数组复制一份、每个元素加 M 接在后面，
// 这样"模环"被拉直成一条线，任意 x 的贡献都落在连续的一段窗口里，
// 用前缀和与二分即可 O(1) 求出，总时间复杂度 O(N log N)。
const int MAXTOTAL = 500005; // 所有测试用例的 N 之和上限

int a[MAXTOTAL];
long long doubled[2 * MAXTOTAL]; // 余数数组 + 每个元素加 M 的副本
long long prefix[2 * MAXTOTAL];

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int n;
        long long m;
        scanf("%d %lld", &n, &m);
        for (int i = 0; i < n; ++i) {
            long long value;
            scanf("%lld", &value);
            a[i] = (int)(value % m);
        }
        sort(a, a + n);
        int total = 2 * n;
        for (int i = 0; i < n; ++i) {
            doubled[i] = a[i];
            doubled[i + n] = a[i] + m;
        }
        prefix[0] = doubled[0];
        for (int i = 1; i < total; ++i) {
            prefix[i] = prefix[i - 1] + doubled[i];
        }
        long long answer = -1; // 负数表示还没取到任何候选
        for (int i = 0; i < total; ++i) {
            long long x = doubled[i];
            // x 两侧各半个模长以内的元素都各自朝 x 靠拢：左边一律加，右边一律减
            long long lowBound = x - m / 2;
            long long highBound = x + (m - 1) / 2;
            // 窗口越界说明这个 x 的完整邻域要跨过 2M 边界，它一定已在另一个下标处被枚举
            if (lowBound < 0 || highBound >= 2 * m) {
                continue;
            }
            int lo = (int)(lower_bound(doubled, doubled + total, lowBound) - doubled);
            long long lowSum = (i > 0 ? prefix[i - 1] : 0) - (lo > 0 ? prefix[lo - 1] : 0);
            long long lowCount = i - lo;
            int hi = (int)(upper_bound(doubled, doubled + total, highBound) - doubled);
            long long highSum = prefix[hi - 1] - prefix[i];
            long long highCount = hi - 1 - i;
            // 左侧 lowCount 个元素都要加到 x，右侧 highCount 个都要减到 x
            long long cost = lowCount * x - lowSum + highSum - highCount * x;
            if (answer < 0 || cost < answer) {
                answer = cost;
            }
        }
        printf("%lld\n", answer);
    }
    return 0;
}
