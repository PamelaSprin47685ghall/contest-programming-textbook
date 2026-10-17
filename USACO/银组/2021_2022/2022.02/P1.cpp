#include <bitset>
#include <cstdio>

using namespace std;

const int MAXN = 505;  // N 最大 500, 多开几个位置防止越界

int n;
int prefLen[MAXN];         // 每头牛"不比原本那份差"的礼物个数(含它自己那份)
int pref[MAXN][MAXN];      // 对应的偏好列表前缀, 按偏好从高到低
bitset<MAXN> reach[MAXN];  // reach[i][j]: 从奶牛(礼物) i 出发能走到 j

// 建模: 建图, 顶点是奶牛(也就是礼物), 对每头牛 i 与它清单里不比自己那份差的
// 礼物 j 连一条边 i -> j. 一个合法的重新分配方案等价于把这张图的顶点分成
// 若干简单环(每头牛把礼物让给指向的那头牛). 于是"牛 i 能拿到礼物 j"当且仅当
// 存在包含边 i -> j 的简单环, 也等价于存在一条 j -> i 的路径.
// 这里用 bitset 做 Floyd-Warshall 传递闭包, 一行 505 位只占 8 个 word,
// 比逐个点做 DFS 快得多.
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        for (int k = 0; k < n; ++k) {
            scanf("%d", &pref[i][k]);
        }
        // 只保留"不比原本那份差"的前缀(含它本来拿到的那份, 自己那份永远能保住),
        // 更差的礼物永远不可能到手, 后面的都不用记
        int pos = 0;
        while (pref[i][pos] != i) ++pos;
        prefLen[i] = pos + 1;
    }

    for (int i = 1; i <= n; ++i) {
        for (int k = 0; k < prefLen[i]; ++k) reach[i].set(pref[i][k]);
    }
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            if (reach[i].test(k)) reach[i] |= reach[k];
        }
    }

    for (int i = 1; i <= n; ++i) {
        // 清单本身就是按偏好排好的, 第一个"能绕回自己"的礼物就是答案
        for (int k = 0; k < prefLen[i]; ++k) {
            int gift = pref[i][k];
            if (reach[gift].test(i)) {
                printf("%d\n", gift);
                break;
            }
        }
    }
    return 0;
}
