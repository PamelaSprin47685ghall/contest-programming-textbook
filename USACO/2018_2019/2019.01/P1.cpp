#include <algorithm>
#include <cstdio>
#include <vector>

struct SwapMove {
    int a, b, g;
};

int main() {
    int n;
    scanf("%d", &n);
    std::vector<SwapMove> moves(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d", &moves[i].a, &moves[i].b, &moves[i].g);
    }

    // 枚举坚果最初可能在的位置 1, 2, 3
    int maxScore = 0;
    for (int startPos = 1; startPos <= 3; ++startPos) {
        int currentPos = startPos;
        int score = 0;
        for (const SwapMove& m : moves) {
            // 如果交换的两只贝壳中包含当前坚果所在贝壳, 则坚果位置转移
            if (m.a == currentPos) {
                currentPos = m.b;
            } else if (m.b == currentPos) {
                currentPos = m.a;
            }
            if (m.g == currentPos) {
                ++score;
            }
        }
        if (score > maxScore) {
            maxScore = score;
        }
    }

    printf("%d\n", maxScore);
    return 0;
}
