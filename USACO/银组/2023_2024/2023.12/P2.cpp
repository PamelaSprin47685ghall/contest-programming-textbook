#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 500005;

int barnCount, cycleLen;
int cycleA[MAXN], cycleB[MAXN];
bool usedByA[MAXN], usedByB[MAXN];
int posInA[MAXN];    // 标签在 Annabelle 的环中的下标，没出现就是 -1
int matchCount[MAXN];   // matchCount[r]：B 的环相对 A 的环旋转 r 位后重合的标签数

// 统计把 B 的环（正读或反读）旋转后，最多有多少个位置与 A 的环标签相同。
// 标签 v 在 A 中下标为 i、在 B 中下标为 j 时，只有旋转量 r = (j - i) mod len 才让它对齐。
int maxRotationMatches(const int *cycleB, int len, bool reversed) {
    for (int r = 0; r < len; ++r) {
        matchCount[r] = 0;
    }
    for (int j = 0; j < len; ++j) {
        int value = reversed ? cycleB[len - 1 - j] : cycleB[j];
        if (posInA[value] < 0) {
            continue;   // 这个标签不在 A 的环里，无论怎么转都对不上
        }
        int shift = j - posInA[value];
        if (shift < 0) {
            shift += len;
        }
        ++matchCount[shift];
    }
    int best = 0;
    for (int r = 0; r < len; ++r) {
        best = max(best, matchCount[r]);
    }
    return best;
}

int main() {
    scanf("%d%d", &barnCount, &cycleLen);
    for (int i = 0; i < cycleLen; ++i) {
        scanf("%d", &cycleA[i]);
        usedByA[cycleA[i]] = true;
    }
    for (int i = 0; i < cycleLen; ++i) {
        scanf("%d", &cycleB[i]);
        usedByB[cycleB[i]] = true;
    }
    for (int v = 1; v <= barnCount; ++v) {
        posInA[v] = -1;
    }
    for (int i = 0; i < cycleLen; ++i) {
        posInA[cycleA[i]] = i;
    }

    // 环上的 K 个谷仓恰好用掉 K 条边，环外的谷仓与环内毫无连接；
    // 环外谷仓的标签只要两边都没用过，就可以自由地一一对应，全部取相同
    int answer = 0;
    for (int v = 1; v <= barnCount; ++v) {
        if (!usedByA[v] && !usedByB[v]) {
            ++answer;
        }
    }

    // 环的方向没有规定，正读反读都要试
    int forward = maxRotationMatches(cycleB, cycleLen, false);
    int backward = maxRotationMatches(cycleB, cycleLen, true);
    answer += max(forward, backward);
    printf("%d\n", answer);
    return 0;
}
