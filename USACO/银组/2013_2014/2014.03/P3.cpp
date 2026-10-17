#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100;
const int MAXB = 20;
const int INF = 1000000000;

int N, B;
int breed[MAXB];          // 每个品种一头牛的叫声音量
int volume[MAXN];         // 每个田野记录到的总音量

int main() {
    scanf("%d %d", &N, &B);
    for (int i = 0; i < B; i++) {
        scanf("%d", &breed[i]);
    }
    for (int i = 0; i < N; i++) {
        scanf("%d", &volume[i]);
    }

    // 先把声音的传播链走一遍: 第 i 个田野自有奶牛贡献的音量是
    // volume[i] 减去前一个田野衰减 1 后传过来的部分.
    // 官方测试的实际音量远大于题面写的 100000(可达数百万),
    // 所以这里先按真实数据确定背包需要的大小, 避免越界.
    int maxOwn = 0;
    int carry = 0;            // 前一个田野传过来的音量, 第一个田野前面没有声音
    for (int i = 0; i < N; i++) {
        int own = volume[i] - carry;
        if (own < 0) {        // 传过来的声音比总音量还大, 不可能
            printf("-1\n");
            return 0;
        }
        if (own > maxOwn) {
            maxOwn = own;
        }
        // 本田野的音量传到下一田野衰减 1, 但不会衰减到负数
        carry = volume[i] - 1;
        if (carry < 0) {
            carry = 0;
        }
    }

    // 完全背包: 每头牛贡献 breed[j] 的音量, 求恰好凑出音量 v 所需最少奶牛数.
    vector<int> minCows(maxOwn + 1, INF);
    minCows[0] = 0;
    for (int v = 1; v <= maxOwn; v++) {
        for (int j = 0; j < B; j++) {
            // 从小到大递推, 同一品种可以反复使用
            if (breed[j] <= v && minCows[v - breed[j]] != INF) {
                int cand = minCows[v - breed[j]] + 1;
                if (cand < minCows[v]) {
                    minCows[v] = cand;
                }
            }
        }
    }

    long long total = 0;
    carry = 0;
    for (int i = 0; i < N; i++) {
        int own = volume[i] - carry;
        if (minCows[own] == INF) {     // 这个音量任何牛群组合都凑不出来
            printf("-1\n");
            return 0;
        }
        total += minCows[own];
        carry = volume[i] - 1;
        if (carry < 0) {
            carry = 0;
        }
    }

    printf("%lld\n", total);
    return 0;
}
