#include <cstdio>

using namespace std;

const int MAXN = 1000005;

int parent[MAXN];   // 第 i 个词 = 第 parent[i] 个词末尾加一个字符
int childCnt[MAXN]; // 有多少个词是把该词当作前缀再添一个字符得到的
int length[MAXN];   // 每个词的长度

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 1; i <= N; ++i) {
        scanf("%d", &parent[i]);
        length[i] = length[parent[i]] + 1;
        ++childCnt[parent[i]];
    }

    // 词库里装的正是没有孩子（不是任何其它词的前缀）的那些词。
    int M = 0;
    for (int i = 0; i <= N; ++i) {
        if (childCnt[i] == 0) ++M;
    }

    for (int q = 0; q < M; ++q) {
        int x;
        scanf("%d", &x);
        // 读到的前缀 x 只会不断向上收缩：x 还有兄弟时，读到 x 就已经能唯一确定；
        // x 是独生子时，读到它的父亲就够了，于是继续往上收，直到撞上分叉点或空串。
        while (true) {
            if (x == 0) {
                printf("0\n"); // 读到空串已经是词库里的答案（只剩这一个词可猜）
                break;
            }
            if (childCnt[parent[x]] == 1) {
                x = parent[x];
                childCnt[x] = 0; // 收缩出的这段前缀从此不再单独作为词被读
                continue;
            }
            printf("%d\n", length[x]);
            --childCnt[parent[x]]; // x 已从词库移除，父亲的候选孩子少一个
            break;
        }
    }
    return 0;
}
