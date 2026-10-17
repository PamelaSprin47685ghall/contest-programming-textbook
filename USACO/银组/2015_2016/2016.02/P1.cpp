#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1005;

int N;
int C[MAXN];   // C[i]：在 i 号房间门口排队进场的奶牛数
int que[MAXN]; // 等待入住的奶牛队列，记录她在"剪开后的链"上的进场位置

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) scanf("%d", &C[i]);

    // 最优解中一定存在一道没有任何奶牛跨过的门：把这道门剪开，圆环就变成
    // 一条链。于是只需要枚举剪开的位置（n 种），在每个链上贪心求最少能量。
    long long answer = -1;
    for (int start = 0; start < N; ++start) {
        int head = 0, tail = 0;
        long long energy = 0;
        bool valid = true;
        for (int k = 0; k < N; ++k) {
            int room = (start + k) % N; // 链上第 k 个房间
            // 每个房间门口排队的奶牛只能从这个门进场，所以她们的进场位置就是 k
            for (int t = 0; t < C[room]; ++t) que[tail++] = k;
            if (head == tail) {
                // 轮到这间房时一头待入住的奶牛都没有，说明这个剪法不可能成立
                valid = false;
                break;
            }
            // 谁先进场谁走的路多，让她先住下：两条路线交换后总能量不会变大
            int enter = que[head++];
            energy += (long long)(k - enter) * (k - enter);
        }
        if (valid && (answer < 0 || energy < answer)) answer = energy;
    }
    printf("%lld\n", answer);
    return 0;
}
