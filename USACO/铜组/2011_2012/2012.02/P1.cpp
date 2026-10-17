#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

// 判断把绳子的第 i0 个结与第 i1 个结对折后, 其余结是否两两重合.
// 绳长用结的位置表示, 对折后对称位置的和恒等于 i0+i1 的位置和.
bool CanFold(const vector<int>& knots, int i0, int i1) {
    int target = knots[i0] + knots[i1];
    while (i0 < i1) {
        if (knots[i0] + knots[i1] != target) return false;
        ++i0;
        --i1;
    }
    // 中间单独一个结时, 它必须恰好落在折点, 即两倍位置等于 target.
    if (i0 == i1 && knots[i0] * 2 != target) return false;
    return true;
}

int main() {
    int n;
    scanf("%d%*d", &n);  // %*d 跳过第二个数 (绳子的长度)
    vector<int> knots(n);
    for (int& knot : knots) scanf("%d", &knot);
    sort(knots.begin(), knots.end());

    int answer = 0;
    // 折点可能在两个结中间: 枚举与最后一个结对折的结, 以及与第一个结对折的结.
    for (int i = 0; i < n - 1; ++i) answer += CanFold(knots, i, n - 1);
    for (int i = 1; i < n - 1; ++i) answer += CanFold(knots, 0, i);

    printf("%d\n", answer);
    return 0;
}