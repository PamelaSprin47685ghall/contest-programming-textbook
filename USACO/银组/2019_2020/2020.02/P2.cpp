// USACO 2020 February Contest, Silver P2: Triangles
#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;
const long long MOD = 1000000007;

int n;
int coordX[MAXN], coordY[MAXN];
int order[MAXN];             // 排序用的点下标
int groupKey[MAXN];          // 这一遍的分组关键字（同 x 或同 y）
int groupValue[MAXN];        // 组内用来算距离的另一个坐标
long long valueBuf[MAXN];    // 组内坐标的升序副本
long long prefix[MAXN + 1];  // 组内前缀和
long long sumX[MAXN], sumY[MAXN]; // 与 i 同 x 的所有点、与 i 同 y 的所有点分别到 i 的距离和

// 先按分组关键字排，同组内按另一个坐标升序，这样距离和可以用前缀和一次算出
bool cmpByKeyValue(int a, int b) {
    if (groupKey[a] != groupKey[b]) return groupKey[a] < groupKey[b];
    if (groupValue[a] != groupValue[b]) return groupValue[a] < groupValue[b];
    return a < b;
}

// 对每个点 i 求：同一组（同关键字）内所有点的另一坐标与 i 的距离之和，写进 result[i]
void computeGroupSums(long long result[]) {
    for (int i = 0; i < n; ++i) order[i] = i;
    sort(order, order + n, cmpByKeyValue);
    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && groupKey[order[j]] == groupKey[order[i]]) ++j;
        int len = j - i;
        for (int t = 0; t < len; ++t) valueBuf[t] = groupValue[order[i + t]];
        prefix[0] = 0;
        for (int t = 0; t < len; ++t) prefix[t + 1] = prefix[t] + valueBuf[t];
        // 组内已升序：左边 t 个都比它小，右边 len-1-t 个都比它大
        for (int t = 0; t < len; ++t) {
            long long leftSum = valueBuf[t] * t - prefix[t];
            long long rightSum = (prefix[len] - prefix[t + 1]) - valueBuf[t] * (len - 1 - t);
            result[order[i + t]] = leftSum + rightSum;
        }
        i = j;
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &coordX[i], &coordY[i]);
    }

    for (int i = 0; i < n; ++i) {
        groupKey[i] = coordX[i];
        groupValue[i] = coordY[i];
    }
    computeGroupSums(sumX);

    for (int i = 0; i < n; ++i) {
        groupKey[i] = coordY[i];
        groupValue[i] = coordX[i];
    }
    computeGroupSums(sumY);

    // 以 i 为直角顶点时，两条直角边分别平行于 y 轴与 x 轴，
    // 所有合法三角形的 2 倍面积之和正好是两条边长度和的乘积，
    // 且每个合法三角形的直角顶点唯一，不会重复计数。
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        answer = (answer + sumX[i] % MOD * (sumY[i] % MOD)) % MOD;
    }
    printf("%lld\n", answer);
    return 0;
}
