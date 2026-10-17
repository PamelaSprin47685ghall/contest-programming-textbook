#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXSUM = 2005;                               // 总草量上限 20 * 100
const int MAXN = 25;                                   // 草捆数量上限

int n;
int bale[MAXN];

// reachable[b2][b3] 为真表示存在一种分配, 使第二个和第三个 barn 的草量分别是 b2 和 b3;
// 第一个 barn 的量由总量推得, 不必存进状态, 状态数因此降到 O(sum^2).
bool reachable[MAXSUM][MAXSUM];

int main() {
    scanf("%d", &n);
    int total = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &bale[i]);
        total += bale[i];
    }

    reachable[0][0] = true;
    for (int i = 0; i < n; i++) {
        int size = bale[i];
        // 倒序枚举, 读到的都是"还没考虑第 i 捆"的旧状态, 保证每捆只用一次.
        for (int b2 = total; b2 >= 0; b2--)
            for (int b3 = total; b3 >= 0; b3--) {
                bool ok = false;
                if (b2 >= size && reachable[b2 - size][b3]) ok = true;
                if (b3 >= size && reachable[b2][b3 - size]) ok = true;
                if (ok) reachable[b2][b3] = true;
            }
    }

    int answer = total;
    for (int b2 = 0; b2 <= total; b2++)
        for (int b3 = 0; b3 <= total; b3++)
            if (reachable[b2][b3]) {
                int b1 = total - b2 - b3;              // 剩下的草只能分给第一个 barn
                answer = min(answer, max(b1, max(b2, b3)));
            }
    printf("%d\n", answer);
    return 0;
}
