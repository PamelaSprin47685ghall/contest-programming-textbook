#include <cstdio>

using namespace std;

const int MAXN = 100005;

int n;
int p[MAXN];       // 初始排列, 内部用 0-based 编号
int bit[MAXN + 2]; // 树状数组: 标记哪些牛已经站到最终位置上

void add(int pos) {
    for (pos++; pos <= n; pos += pos & -pos) bit[pos]++;
}

// 已经归位的牛里, 编号 <= pos 的有几头
int query(int pos) {
    int s = 0;
    for (pos++; pos > 0; pos -= pos & -pos) s += bit[pos];
    return s;
}

int main() {
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &p[i]);
        p[i]--;
    }
    // j: 最长递增后缀的起点. 这 n-j 头牛相对顺序本来就已经正确, 一步都不用动;
    // 而每次指挥只会把队首的牛往后插, 最长递增后缀的起点最多左移一位, 所以至少 j 步
    int j = n - 1;
    while (j > 0 && p[j - 1] < p[j]) j--;
    printf("%d\n", j);

    // 不变式: 队伍 = [还没处理的 p[i..j-1]] ++ [已归位的牛, 编号递增]
    // 后缀里的牛一开始就已经站在最终位置上
    for (int i = j; i < n; i++) add(p[i]);
    for (int i = 0; i < j; i++) {
        // 队首的 p[i] 往后挪: 先越过剩下的 j-1-i 头未处理的牛, 再越过已归位里编号更小的牛,
        // 正好插到自己该在的位置上, 不变式得以保持
        int steps = (j - 1 - i) + query(p[i]);
        if (i + 1 < j) printf("%d ", steps);
        else printf("%d\n", steps);
        add(p[i]);
    }
    if (j == 0) printf("\n");
    return 0;
}
