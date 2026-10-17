#include <cstdio>

using namespace std;

const int MAXN = 1000005;

char s[MAXN];
char t[MAXN];

// 用数组模拟双端队列。[head, tail) 为有效区间。
// 队列中每个元素是 (位置, 净数量)：位置严格递增；
// 净数量 > 0 表示这个位置有一批 s 中为 1、t 中为 0 的下标，需要向右移动；
// 净数量 < 0 表示有一批 s 中为 0、t 中为 1 的下标，需要向左移动。
// 每次迭代最多入队两个元素，所以开 2*MAXN 足够。
int qPos[2 * MAXN];
int qCnt[2 * MAXN];
int head;
int tail;

int iabs(int x) {
    return x < 0 ? -x : x;
}

int main() {
    int N, K;
    scanf("%d %d", &N, &K);
    scanf("%s", s);
    scanf("%s", t);

    head = MAXN;
    tail = MAXN;
    long long ans = 0;

    for (int i = 0; i < N; i++) {
        // 队首位置比 i 更靠左：它已经无法在 K 的步长内与 i 或更右的位置配对，
        // 只能整批向右挪 K 步，代价是它承载的净数量（每个元素记 1 次移动）。
        if (tail > head && qPos[head] < i) {
            ans += iabs(qCnt[head]);
            qPos[tail] = qPos[head] + K;
            qCnt[tail] = qCnt[head];
            tail++;
            head++;
        }
        if (s[i] != t[i]) {
            int dif = (s[i] == '1') ? 1 : -1;
            if (tail > head && iabs(qCnt[head] + dif) <= iabs(qCnt[head])) {
                // 与队首方向相反：贪心地和最小的位置配对，直接抵消。
                qCnt[head] += dif;
                if (qCnt[head] == 0) head++;
            } else {
                // 无法配对：这是一批新元素，插入到队首（位置已等于 i 则并进去）。
                if (!(tail > head && qPos[head] == i)) {
                    head--;
                    qPos[head] = i;
                    qCnt[head] = 0;
                }
                qCnt[head] += dif;
            }
        }
    }
    printf("%lld\n", ans);
    return 0;
}
