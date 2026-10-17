// USACO 2020 US Open Contest, Silver P2: Cereal
#include <cstdio>

using namespace std;

const int MAXN = 100000;
const int MAXM = 100000;

int n, m;
int favorite[MAXN + 1], second[MAXN + 1];
int occupied[MAXM + 1];   // occupied[pos]：拿到麦片 pos 的奶牛编号，0 表示还没人拿
int answer[MAXN + 1];     // answer[i]：从第 i 头牛开始排队（即移除了前 i-1 头牛）时拿到麦片的牛数

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        scanf("%d %d", &favorite[i], &second[i]);
    }

    // 从后往前做：处理到 i 时，answer[i] 是"队伍里只有 i..n 号牛"的答案。
    // 每步只是往队首插入一头牛，只需局部调整，不必重新模拟整队。
    int count = 0;   // 当前拿到麦片的牛数
    for (int i = n; i >= 1; --i) {
        int cow = i;              // 正在为它找麦片的牛
        int pos = favorite[i];    // 它当前尝试的麦片

        while (true) {
            if (occupied[pos] == 0) {
                occupied[pos] = cow;   // 这盒还在，直接拿走
                ++count;
                break;
            }
            int other = occupied[pos];
            if (other < cow) {
                // 拿走 pos 的牛排在 cow 前面，cow 抢不到它，只能空手离开
                break;
            }
            // pos 原本属于排在后面的 other，现在归 cow。
            // other 若把 pos 当第一志愿，就退去试第二志愿；
            // 若 pos 正是它的第二志愿，则它什么都没有了。
            occupied[pos] = cow;
            if (pos == second[other]) break;
            cow = other;
            pos = second[other];
        }

        answer[i] = count;
    }

    // 移除前 i 头牛（0 <= i <= n-1）等价于从第 i+1 头牛开始排队
    for (int i = 1; i <= n; ++i) {
        printf("%d\n", answer[i]);
    }
    return 0;
}
