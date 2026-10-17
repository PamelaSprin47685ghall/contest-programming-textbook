#include <cstdio>

using namespace std;

const int MAXN = 100005;              // 每根试管最多装 N 单位液体
const int MAXSTEP = 2 * MAXN + 5;     // 压缩后的层数与步数上限

int n, queryType;
int layer[3][MAXSTEP];                // 两根试管与烧杯：从底到顶的颜色，相邻两层颜色必不同
int layerCount[3];
int moveSrc[MAXSTEP], moveDst[MAXSTEP];
int moveCount;
char tubeText[MAXN];                  // 读入的原始液体串

// 倒液一次：把 source 顶部同色的连续若干单位整体倒到 dst 上
void pour(int source, int dst) {
    moveSrc[moveCount] = source;
    moveDst[moveCount] = dst;
    ++moveCount;
    int color = layer[source][layerCount[source] - 1];
    --layerCount[source];
    // dst 顶层同色时并进那一层，两层的分界本来就不存在，层数不增加
    if (layerCount[dst] == 0 || layer[dst][layerCount[dst] - 1] != color) {
        layer[dst][layerCount[dst]] = color;
        ++layerCount[dst];
    }
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &queryType);
        for (int i = 0; i < 2; ++i) {
            scanf("%s", tubeText);
            layerCount[i] = 0;
            for (int j = 0; tubeText[j] != '\0'; ++j) {
                int color = tubeText[j] - '0';
                if (layerCount[i] > 0 && layer[i][layerCount[i] - 1] == color) {
                    continue;   // 同色相邻单位属于同一层，只记一次
                }
                layer[i][layerCount[i]] = color;
                ++layerCount[i];
            }
        }
        layerCount[2] = 0;

        // 两根试管底层同色时，最底层那层最终必须整体搬走一次，答案比层数之和多 1。
        // 在 tube 0 最底部补一层相反颜色，就把这次额外搬运记在了账上：
        // 它是最底层，永远不会被倒出去，只会占掉 tube 0 底层的位置
        if (layer[0][0] == layer[1][0]) {
            for (int j = layerCount[0]; j > 0; --j) {
                layer[0][j] = layer[0][j - 1];
            }
            layer[0][0] = 3 - layer[0][0];
            ++layerCount[0];
        }

        // 每倒一次，压缩后的总层数最多减少 1；剩得比两层多时必须借道烧杯，再花一次
        int answer = layerCount[0] + layerCount[1] - 2;
        if (answer > 1) {
            ++answer;
        }
        printf("%d\n", answer);
        if (queryType == 1) {
            continue;
        }

        moveCount = 0;
        // 第 1 步：两根顶部同色时，把较长的那根整段倒进另一根
        if (layer[0][layerCount[0] - 1] == layer[1][layerCount[1] - 1]) {
            if (layerCount[0] > layerCount[1]) {
                pour(0, 1);
            } else {
                pour(1, 0);
            }
        }
        int longTube = -1;
        if (layerCount[0] > 1) {
            longTube = 0;
        } else if (layerCount[1] > 1) {
            longTube = 1;
        }
        if (longTube >= 0) {
            // 第 2 步：借道烧杯，此后两根试管顶部一定同色
            pour(longTube, 2);
            // 第 3 步：底层颜色与烧杯底层不同的那根试管先清到只剩一层：
            // 每倒一次要么让它在烧杯里并层、要么让它在邻管上并层，层数都减一
            int keep = 0;
            if (layer[keep][0] == layer[2][0]) {
                keep = 1;
            }
            while (layerCount[keep] > 1) {
                if (layer[keep][layerCount[keep] - 1] == layer[2][0]) {
                    pour(keep, 2);
                } else {
                    pour(keep, keep ^ 1);
                }
            }
            // 第 4 步：另一根同样清到只剩一层
            keep ^= 1;
            while (layerCount[keep] > 1) {
                if (layer[keep][layerCount[keep] - 1] == layer[2][0]) {
                    pour(keep, 2);
                } else {
                    pour(keep, keep ^ 1);
                }
            }
            // 第 5 步：烧杯里全是一种颜色，倒回 keep 收尾
            pour(2, keep);
        }
        for (int i = 0; i < moveCount; ++i) {
            printf("%d %d\n", moveSrc[i] + 1, moveDst[i] + 1);
        }
    }
    return 0;
}
