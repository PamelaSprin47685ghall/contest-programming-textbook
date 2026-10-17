#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

int score[MAXN + 2];            // 第 i 题的分数（下标从 1 开始）
long long suffix_sum[MAXN + 2]; // suffix_sum[i] = 第 i 题到第 N 题的分数和
int suffix_min[MAXN + 2];       // suffix_min[i] = 第 i 题到第 N 题的最低分

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &score[i]);

    // 从后往前递推后缀和与后缀最小值，这样枚举每个 K 时都能 O(1) 读出
    // 剩下部分的分数和与其中的最低分，不必对每个 K 重新扫描一遍。
    suffix_sum[n + 1] = 0;
    suffix_min[n + 1] = 10001; // 比任何分数都大，作为 min 的单位元
    for (int i = n; i >= 1; i--) {
        suffix_sum[i] = suffix_sum[i + 1] + score[i];
        suffix_min[i] = min(suffix_min[i + 1], score[i]);
    }

    // 被吃掉前 K 题后剩下 N-K 题，去掉一个最低分后还剩 N-K-1 题参与平均。
    // 先用交叉相乘比较分数大小（避免浮点误差）找出最高平均分。
    long long best_num = -1, best_den = 1;
    for (int k = 1; k <= n - 2; k++) {
        long long num = suffix_sum[k + 1] - suffix_min[k + 1];
        long long den = n - k - 1;
        if (num * best_den > best_num * den) {
            best_num = num;
            best_den = den;
        }
    }

    // 再从小到大输出所有与最高平均分相等的 K。k 递增即输出有序。
    for (int k = 1; k <= n - 2; k++) {
        long long num = suffix_sum[k + 1] - suffix_min[k + 1];
        long long den = n - k - 1;
        if (num * best_den == best_num * den) printf("%d\n", k);
    }
    return 0;
}
