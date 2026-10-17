#include <cstdio>

using namespace std;

const int MAXN = 7505;

char lineup[MAXN];     // 队伍序列（G / H）
int gpos[MAXN];        // 每个 G 在整串中的下标，按从左到右排列
int gcnt;              // G 的总数
int touched[MAXN];     // 本轮改过哪些 e 的下标，最后一次性清零
long long e[2 * MAXN]; // e[s] = 2 ×（已匹配的对中 u_j + v_j 恰好为 s 的对数）

int main() {
    scanf("%s", lineup);
    int n = 0;
    while (lineup[n] != '\0') ++n;
    gcnt = 0;
    for (int i = 0; i < n; ++i)
        if (lineup[i] == 'G') gpos[gcnt++] = i;

    long long total = 0;
    // 固定中间的 G：lc = rc 表示这段里有奇数个 G，lc 就是正中那个；
    // rc = lc + 1 表示有偶数个 G，正中的两个是 lc 和 lc + 1。
    // 固定中点后，每个 G 和谁配对就与左右端点无关了：
    // 左边第 j 个 G 永远配右边第 j 个 G，于是贡献只与 l + r 有关。
    for (int lc = 0; lc < gcnt; ++lc) {
        for (int rc = lc; rc <= lc + 1 && rc < gcnt; ++rc) {
            int nt = 0;
            long long F = 0; // F = Σ_j |u_j + v_j - (l + r)|，即当前 s = l + r 处的总代价
            long long d = 0; // d = F(s + 1) - F(s)，随 s 增大而更新的差分
            // k 是"向外扩的层数"：第 k 层的配对是 (gpos[lc - k], gpos[rc + k])
            for (int k = 0; lc - k >= 0 && rc + k < gcnt; ++k) {
                int uk = gpos[lc - k];  // 左数第 k + 1 个配对的左端点 u
                int uk1 = (lc - k == 0) ? -1 : gpos[lc - k - 1]; // 再往左一个 G（作为左端点的下界）
                int vk = gpos[rc + k];  // 右数第 k + 1 个配对的右端点 v
                int vk1 = (rc + k + 1 == gcnt) ? n : gpos[rc + k + 1]; // 再往右一个 G（作为右端点的上界）
                if (uk < vk) {
                    // 中间那个单独的 G 不与任何 G 配对，所以 lc == rc 且 k == 0 时不算配对
                    e[uk + vk] += 2; // 加进这一对后，d 在跨越 s = uk + vk 处要多 1
                    ++d;
                    touched[nt++] = uk + vk;
                }
                // 这一层里所有合法的子段：l ∈ (uk1, uk]，r ∈ [vk, vk1)
                // 它们恰好以 (uk, vk) 为一对配对端点，G 的个数也正好确定
                for (int l = uk; l > uk1; --l) {
                    for (int r = vk; r < vk1; ++r) {
                        if (lc == rc) {
                            if (((r - l) & 1) == 0) // 长度为奇数才能把正中的 G 放到正中
                                total += F + ((l + r) / 2 > gpos[lc] ? (l + r) / 2 - gpos[lc] : gpos[lc] - (l + r) / 2);
                            else
                                --total; // 无法构成回文，贡献 -1
                        } else {
                            total += F;
                        }
                        F += d;              // s 增加 1
                        d += e[l + r + 1];   // 差分再更新
                    }
                    // 把 r 退回 vk，恢复 (l, vk) 对应的状态，供下一个 l 使用
                    for (int r = vk1; r > vk; --r) { d -= e[l + r]; F -= d; }
                    d -= e[l + vk]; F -= d;
                }
                // 状态推到 (uk1, vk1)，正好是下一层开始时的 s
                for (int r = vk; r < vk1; ++r) { F += d; d += e[uk1 + r + 1]; }
            }
            for (int t = 0; t < nt; ++t) e[touched[t]] = 0; // 只清掉动过的位置
        }
    }
    printf("%lld\n", total);
    return 0;
}
