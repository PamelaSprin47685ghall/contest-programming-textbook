#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;
const int INF = 1000000005;     // 比任何可能答案都大, 当作"做不到"的哨兵

struct Bale {
    int pos;    // 位置
    int size;   // 大小
};

Bale bale[MAXN];

// 按位置排序后, 贝茜左边的干草堆一定是一段前缀, 右边是一段后缀
bool cmpByPos(const Bale &a, const Bale &b) {
    return a.pos < b.pos;
}

int main() {
    int N, B;
    scanf("%d %d", &N, &B);
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &bale[i].size, &bale[i].pos);
    }
    sort(bale, bale + N, cmpByPos);

    // start: 第一块位于贝茜右边的干草堆. 左边的干草堆是 [0, start-1], 右边的是 [start, N-1]
    int start = 0;
    while (start < N && bale[start].pos <= B) start++;

    int result = INF;

    // 情况一: 加高的干草堆取右边第 j 块, 左边的 bale[i] 自己就拦得住她.
    // 她被夹在 bale[i] 和 bale[j] 之间时, 向左助跑的距离最多是
    // bale[j].pos - bale[i].pos, 撞不破 bale[i] 就说明这个距离不超过 bale[i].size.
    // 要让右边的 bale[j] 也撞不破, 它的新大小至少要等于这个距离,
    // 所以需要补 bale[j].pos - bale[i].pos - bale[j].size.
    int j = start;
    for (int i = start - 1; i >= 0; i--) {
        // i 往左移时 bale[i].pos 变小, 能被她撞破的右边干草堆只会更多,
        // 所以 j 只需单向前进, 两个指针一共走 O(N) 步
        while (j < N && bale[j].pos <= bale[i].pos + bale[i].size) {
            result = min(result, bale[j].pos - bale[i].pos - bale[j].size);
            j++;
        }
    }

    // 情况二: 加高的干草堆取左边第 j 块, 右边的 bale[i] 自己就拦得住她.
    // 对称地, 需要补 bale[i].pos - bale[j].pos - bale[j].size.
    j = start - 1;
    for (int i = start; i < N; i++) {
        while (j >= 0 && bale[i].pos - bale[i].size <= bale[j].pos) {
            result = min(result, bale[i].pos - bale[j].pos - bale[j].size);
            j--;
        }
    }

    if (result == INF) {
        // 没有任何一对干草堆能把她夹住, 加多少干草都没用
        printf("-1\n");
    } else {
        // 补出来是负的, 说明那块干草堆本来就撞不破, 不需要加
        printf("%d\n", max(result, 0));
    }
    return 0;
}
