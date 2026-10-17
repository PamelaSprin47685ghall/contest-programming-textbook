#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXC = 100005;

int targetCount, commandCount;
bool isTarget[2 * MAXC + 5];      // 该坐标（整体加上 commandCount 平移后）是否有目标
bool inPrefix[2 * MAXC + 5];      // 该目标是否已被未改动命令的前缀打掉
bool inSuffix[5][2 * MAXC + 5];   // 后缀整体平移 d-2 时能打到的、前缀没打过的目标
bool inBuffer[5][2 * MAXC + 5];   // 后缀能打到但此刻仍算在前缀里的目标，先暂存
int suffixSize[5];
int position[MAXC + 1];           // 执行完前 t 条命令后 Bessie 的位置
int firstHitTime[MAXC];           // 第 t 条命令处第一次打掉的目标坐标，-1 表示没有
char commands[MAXC];

int main() {
    scanf("%d%d", &targetCount, &commandCount);
    for (int i = 0; i < targetCount; ++i) {
        int x;
        scanf("%d", &x);
        isTarget[x + commandCount] = true;
    }
    scanf("%s", commands);

    position[0] = commandCount;   // 平移坐标避免出现负下标
    for (int i = 0; i < commandCount; ++i) {
        int step = (commands[i] == 'L') ? -1 : ((commands[i] == 'R') ? 1 : 0);
        position[i + 1] = position[i] + step;
        firstHitTime[i] = -1;
    }

    int prefixHits = 0;
    for (int i = 0; i < commandCount; ++i) {
        int here = position[i];
        if (commands[i] == 'F' && isTarget[here] && !inPrefix[here]) {
            inPrefix[here] = true;   // 同一个目标只算一次
            firstHitTime[i] = here;
            ++prefixHits;
        }
    }

    int best = prefixHits;   // 一条命令都不改的答案
    int current = position[commandCount];
    for (int i = commandCount - 1; i >= 0; --i) {
        // 撤销第 i 条命令的移动，current 变回时刻 i 的位置
        if (commands[i] == 'L') {
            ++current;
        } else if (commands[i] == 'R') {
            --current;
        }
        // 时刻 i 原本第一次打掉的目标从此离开前缀集合
        if (firstHitTime[i] != -1) {
            inPrefix[current] = false;
            --prefixHits;
            // 后缀也打到了它，现在可以正式计入后缀
            for (int d = 0; d < 5; ++d) {
                if (inBuffer[d][current]) {
                    inBuffer[d][current] = false;
                    inSuffix[d][current] = true;
                    ++suffixSize[d];
                }
            }
        }

        // 改动只会让时刻 i 之后的位置整体平移，平移量最大为 2
        if (commands[i] == 'L') {
            // 改成 F：脚下这个目标可能被新打掉，之后整体右移 1
            int extra = (isTarget[current] && !inPrefix[current] && !inSuffix[3][current]) ? 1 : 0;
            best = max(best, prefixHits + extra + suffixSize[3]);
            // 改成 R：少一枪，之后整体右移 2
            best = max(best, prefixHits + suffixSize[4]);
        } else if (commands[i] == 'R') {
            // 改成 F：脚下这个目标可能被新打掉，之后整体左移 1
            int extra = (isTarget[current] && !inPrefix[current] && !inSuffix[1][current]) ? 1 : 0;
            best = max(best, prefixHits + extra + suffixSize[1]);
            // 改成 L：少一枪，之后整体左移 2
            best = max(best, prefixHits + suffixSize[0]);
        } else {
            // 改成 L / R：少一枪，之后整体左移 1 / 右移 1
            best = max(best, prefixHits + suffixSize[1]);
            best = max(best, prefixHits + suffixSize[3]);
        }

        // 把第 i 条命令在平移 d-2 时能打到的目标并入后缀（位置必落在当前坐标 ±2 内）
        if (commands[i] == 'F') {
            for (int j = current - 2; j <= current + 2; ++j) {
                if (j < 0 || j > 2 * commandCount || !isTarget[j]) {
                    continue;
                }
                int d = j - current + 2;
                if (inPrefix[j]) {
                    inBuffer[d][j] = true;   // 前缀还没放手，先记在缓冲区
                } else if (!inSuffix[d][j]) {
                    inSuffix[d][j] = true;
                    ++suffixSize[d];
                }
            }
        }
    }
    printf("%d\n", best);
    return 0;
}
