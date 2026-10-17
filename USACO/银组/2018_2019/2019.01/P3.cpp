#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n;
long long leftEnd[MAXN];   // 山底左端点 x - y
long long rightEnd[MAXN];  // 山底右端点 x + y
int order[MAXN];           // 按山底左端点排序后的下标

// 左端点升序; 左端点相同时, 右端点大的在前, 因为它会遮住右端点小的那座山.
bool CompareMountains(int a, int b) {
    if (leftEnd[a] != leftEnd[b]) {
        return leftEnd[a] < leftEnd[b];
    }
    return rightEnd[a] > rightEnd[b];
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        long long x, y;
        scanf("%lld%lld", &x, &y);
        leftEnd[i] = x - y;
        rightEnd[i] = x + y;
        order[i] = i;
    }
    sort(order, order + n, CompareMountains);

    // 45 度的等腰三角形等价于区间 [x-y, x+y]: 山 i 被山 j 遮挡当且仅当该区间
    // 被 j 的区间包含. 排序后, 出现在当前山之前的山左端点都不大于它, 所以
    // 当前山只要右端点严格大于此前所有山的右端点就不会被包含, 即为可见.
    // 因此只需扫描并维护历史上最大的右端点.
    long long maxRight = -1;
    int visible = 0;
    for (int i = 0; i < n; ++i) {
        if (rightEnd[order[i]] > maxRight) {
            ++visible;
            maxRight = rightEnd[order[i]];
        }
    }
    printf("%d\n", visible);
    return 0;
}
