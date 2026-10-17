#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int k, n;
    scanf("%d%d", &k, &n);

    // rank[session][cow]: 在第 session 轮训练中, 奶牛 cow 的名次 (1-based)
    vector<vector<int>> ranking(k, vector<int>(n + 1));
    for (int s = 0; s < k; ++s) {
        for (int r = 1; r <= n; ++r) {
            int cow;
            scanf("%d", &cow);
            ranking[s][cow] = r;
        }
    }

    // 枚举所有不同的奶牛对 (A, B)
    int consistentPairs = 0;
    for (int a = 1; a <= n; ++a) {
        for (int b = 1; b <= n; ++b) {
            if (a == b) continue;

            // 检查在全部 K 轮训练中, A 是否每一次都排在 B 前面
            bool alwaysBetter = true;
            for (int s = 0; s < k; ++s) {
                if (ranking[s][a] > ranking[s][b]) {
                    alwaysBetter = false;
                    break;
                }
            }

            if (alwaysBetter) {
                ++consistentPairs;
            }
        }
    }

    printf("%d\n", consistentPairs);
    return 0;
}
