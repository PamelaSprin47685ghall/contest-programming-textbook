#include <cstdio>

using namespace std;

const int MAXN = 100005;

int color[MAXN];
int firstPos[MAXN];  // 颜色 c 第一次出现的位置
int lastPos[MAXN];   // 颜色 c 最后一次出现的位置

int main() {
    int n;
    scanf("%d", &n);

    for (int c = 0; c <= n; ++c) {
        firstPos[c] = n + 2;  // 没出现过的颜色永远不会触发"入栈"
        lastPos[c] = -1;
    }
    // 把画布两端各补一格底色 0: 每种真实颜色都被底色区间包住,
    // 栈里因此始终垫着这层底色, 不必单独处理空栈.
    color[0] = 0;
    color[n + 1] = 0;
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &color[i]);
    }
    for (int i = 0; i <= n + 1; ++i) {
        int c = color[i];
        if (i < firstPos[c]) {
            firstPos[c] = i;
        }
        if (i > lastPos[c]) {
            lastPos[c] = i;
        }
    }

    int stack[MAXN];
    int top = 0;     // 栈中颜色个数, 也就是当前嵌套的层数
    int depth = 0;   // 出现过的最大嵌套层数
    for (int i = 0; i <= n + 1; ++i) {
        int c = color[i];
        if (i == firstPos[c]) {
            stack[top] = c;  // 颜色 c 的区间从这里开始
            ++top;
            if (top > depth) {
                depth = top;
            }
        }
        if (stack[top - 1] != c) {
            // 栈顶不是 c: 颜色 c 的区间被别人的区间"横穿", 不可能由分层涂色得到
            printf("-1\n");
            return 0;
        }
        if (i == lastPos[c]) {
            --top;  // 颜色 c 的区间到这里结束
        }
    }

    // depth 把最外层的底色 0 也算作一层, 而复制这层底色不算一轮, 所以要减一
    printf("%d\n", depth - 1);
    return 0;
}
