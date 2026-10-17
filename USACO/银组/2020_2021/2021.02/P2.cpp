#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 65536;

int n, k;
int cycle[MAXN + 5];  // 每位祖先所属的「12 年周期」编号
int gap[MAXN + 5];    // 相邻两个被访问周期之间可以整段跳过的空闲周期数

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; ++i) {
        int yearsAgo;
        scanf("%d", &yearsAgo);
        // 第 x 年（x 年前）属于周期 ceil(x/12)：要见到这一年的祖先，
        // 必须完整地走完该周期里的 12 年，故答案为周期数乘 12
        cycle[i] = (yearsAgo + 11) / 12;
    }
    sort(cycle, cycle + n);
    int m = 0;  // 去重，得到所有必须经过的周期
    for (int i = 0; i < n; ++i) {
        if (m == 0 || cycle[i] != cycle[m - 1]) {
            cycle[m++] = cycle[i];
        }
    }
    // 时间只能向过去单向推进：先去最远的周期，路上用最多 K-1 次跳跃
    // 整段跳过空闲周期，因此要跳过最长的 K-1 段空隙
    int last = 0;  // 现在的年份是周期 0，也算一个「已到达」的位置
    int cnt = 0;
    for (int i = 0; i < m; ++i) {
        gap[cnt++] = cycle[i] - last - 1;
        last = cycle[i];
    }
    sort(gap, gap + cnt);
    long long answer = cycle[m - 1];
    for (int i = cnt - 1, used = 0; i >= 0 && used < k - 1; --i, ++used) {
        answer -= gap[i];
    }
    printf("%lld\n", answer * 12);
    return 0;
}
