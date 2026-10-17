#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXV = 105;

int X, Y, K, M;
bool can[MAXV][MAXV]; // can[a][b]：用至多当前步数能否得到（X 桶 a 单位，Y 桶 b 单位）
bool nxt[MAXV][MAXV];

int main() {
    scanf("%d%d%d%d", &X, &Y, &K, &M);

    can[0][0] = true;
    for (int step = 0; step < K; ++step) {
        for (int a = 0; a <= X; ++a)
            for (int b = 0; b <= Y; ++b) nxt[a][b] = false;

        for (int a = 0; a <= X; ++a) {
            for (int b = 0; b <= Y; ++b) {
                if (!can[a][b]) continue;
                // 允许"至多 K 步"，所以多出来的步数可以什么都不做，状态原地保留
                nxt[a][b] = true;
                nxt[X][b] = true; // 装满 X 桶
                nxt[0][b] = true; // 倒空 X 桶
                nxt[a][Y] = true; // 装满 Y 桶
                nxt[a][0] = true; // 倒空 Y 桶
                // X 桶倒进 Y 桶：直到 X 空或 Y 满
                int pourToY = min(a, Y - b);
                nxt[a - pourToY][b + pourToY] = true;
                // Y 桶倒进 X 桶：直到 Y 空或 X 满
                int pourToX = min(X - a, b);
                nxt[a + pourToX][b - pourToX] = true;
            }
        }
        for (int a = 0; a <= X; ++a)
            for (int b = 0; b <= Y; ++b) can[a][b] = nxt[a][b];
    }

    int answer = M; // 两个桶都空着，与 M 相差 M
    for (int a = 0; a <= X; ++a) {
        for (int b = 0; b <= Y; ++b) {
            if (!can[a][b]) continue;
            int diff = a + b - M; // 两桶总量与 M 的误差
            if (diff < 0) diff = -diff;
            if (diff < answer) answer = diff;
        }
    }
    printf("%d\n", answer);
    return 0;
}
