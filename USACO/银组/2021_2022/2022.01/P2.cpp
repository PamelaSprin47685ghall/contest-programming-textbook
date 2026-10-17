#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 300005;

int height[MAXN];
int stk[MAXN];  // 单调栈, 存下标, 栈里的高度自底向上递减
long long answer;

// 统计所有"右端比左端高"的合法点对: 对每个 i, 与它配对的只能是右侧最近的高个.
void AddContributions(int n) {
    int top = 0;
    for (int i = n - 1; i >= 0; --i) {
        // 比 height[i] 矮的栈顶永远不可能再成为任何位置的"右侧最近的高个", 直接弹掉.
        while (top > 0 && height[stk[top - 1]] < height[i]) --top;
        // 弹完之后栈顶就是 i 右边最近的比 height[i] 高的位置.
        if (top > 0) answer += stk[top - 1] - i + 1;
        stk[top++] = i;
    }
}

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) scanf("%d", &height[i]);
    // 第一类: 右端更高. 第二类(左端更高)把数组翻转后就是第一类, 于是再统计一遍.
    AddContributions(N);
    reverse(height, height + N);
    AddContributions(N);
    printf("%lld\n", answer);
    return 0;
}
