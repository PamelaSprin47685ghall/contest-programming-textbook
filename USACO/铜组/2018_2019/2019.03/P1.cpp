#include <algorithm>
#include <cmath>
#include <cstdio>

int main() {
    int barnR = 0, barnC = 0;
    int lakeR = 0, lakeC = 0;
    int rockR = 0, rockC = 0;

    for (int r = 0; r < 10; ++r) {
        char row[16];
        scanf("%s", row);
        for (int c = 0; c < 10; ++c) {
            if (row[c] == 'B') {
                barnR = r;
                barnC = c;
            } else if (row[c] == 'L') {
                lakeR = r;
                lakeC = c;
            } else if (row[c] == 'R') {
                rockR = r;
                rockC = c;
            }
        }
    }

    int dist = std::abs(barnR - lakeR) + std::abs(barnC - lakeC);

    // 只有当 B 与 L 在同一行或同一列, 且岩石 R 恰好夹在两者正中间时, 必须额外绕行 2 步.
    bool blockedInRow = (barnR == lakeR && barnR == rockR &&
                         std::min(barnC, lakeC) < rockC && rockC < std::max(barnC, lakeC));
    bool blockedInCol = (barnC == lakeC && barnC == rockC &&
                         std::min(barnR, lakeR) < rockR && rockR < std::max(barnR, lakeR));

    if (blockedInRow || blockedInCol) {
        printf("%d\n", dist + 1);
    } else {
        printf("%d\n", dist - 1);
    }

    return 0;
}
