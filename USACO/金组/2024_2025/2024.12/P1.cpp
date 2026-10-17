#include <cstdio>
#include <cmath>

using namespace std;

const int MAXN = 100005;

int N;
int label[MAXN];      // label[i]：第 i 头牛的标签

int firstPos[MAXN];   // firstPos[L]：标签 L 第一次出现的位置（0 表示该标签没出现）
int nextSame[MAXN];   // nextSame[i]：i 之后下一个同标签牛的位置（0 表示没有）
int labelCount[MAXN]; // labelCount[L]：标签 L 出现的次数

// 出现次数多的标签（"大标签"）用 nextPos 一步跳到下一组，
// 出现次数少的标签（"小标签"）先把自己的坐标拷进 positionList 再贪心。
int nextPos[MAXN + 2];
int positionList[MAXN];

int diff[MAXN + 2];      // 小标签贡献的差分数组
int bigAnswer[MAXN + 1]; // 大标签逐个 x 直接算出的组数之和

// 小标签在参数 x 下需要几个组：坐标已按递增排好，能塞进当前组就尽量塞。
// 贪心是对的：第一头牛必然落在某个组里，而最靠左的那个组只有尽量向右多塞，
// 留给后面的牛才最少。
int groupsInList(int k, int x) {
    int groups = 0;
    int left = 0;
    while (left < k) {
        int right = left;
        while (right < k && positionList[right] - positionList[left] <= x) {
            ++right;
        }
        ++groups;
        left = right;
    }
    return groups;
}

// 大标签在参数 x 下需要几个组：下一组的起点必须是第一个坐标超过"当前组起点 + x"
// 的同标签牛，用 nextPos 一步跳过去。每次跳跃至少前进 x，所以一个 x 只要 O(N/x) 步。
int groupsByJump(int x) {
    int groups = 0;
    int cur = nextPos[1];
    while (cur <= N) {
        ++groups;
        if (cur + x + 1 > N) {
            break; // 右边再没有同标签的牛了
        }
        cur = nextPos[cur + x + 1];
    }
    return groups;
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; ++i) {
        scanf("%d", &label[i]);
        ++labelCount[label[i]];
    }
    // 把同标签的牛按位置递增串成链表：这样不必为每个标签开一个动态数组，
    // 需要时顺着链表走一遍就是递增的坐标序列。
    for (int i = N; i >= 1; --i) {
        nextSame[i] = firstPos[label[i]];
        firstPos[label[i]] = i;
    }

    // 阈值取 sqrt(N)：超过它的标签最多 N/sqrt(N) = sqrt(N) 个，
    // 不超过它的标签每个只需 O(k^2 log N)，两类规模恰好互相制约。
    int threshold = (int)sqrt((double)N) + 1;

    for (int L = 1; L <= N; ++L) {
        int k = labelCount[L];
        if (k == 0) {
            continue;
        }
        if (k >= threshold) {
            // 大标签：预处理"i 处及之后的第一个标签 L 的位置"
            nextPos[N + 1] = N + 1; // 哨兵：表示不存在
            for (int i = N; i >= 1; --i) {
                if (label[i] == L) {
                    nextPos[i] = i;
                } else {
                    nextPos[i] = nextPos[i + 1];
                }
            }
            for (int x = 1; x <= N; ++x) {
                bigAnswer[x] += groupsByJump(x);
            }
        } else {
            // 小标签：它在 x 下的组数随 x 单调不升，而且组数 >= m 的那些 x 恰好是一段前缀。
            // 于是对每个 m 二分出这段前缀的右端点 lo，再用差分记下"该标签给 [1, lo] 每个 x 贡献一组"。
            // 累加 1..k 的贡献，第 x 位得到的就是该标签在 x 下的组数。
            int idx = 0;
            for (int p = firstPos[L]; p != 0; p = nextSame[p]) {
                positionList[idx++] = p;
            }
            int hi = N + 1; // 二分上界：组数 >= m 的 x 一定小于上一轮二分出的右端点
            for (int m = 1; m <= k; ++m) {
                int lo = 0; // 恒有 groupsInList(k, lo) >= m，因为 x = 0 时一组一头牛
                while (lo < hi - 1) {
                    int mid = (lo + hi) / 2;
                    if (groupsInList(k, mid) >= m) {
                        lo = mid;
                    } else {
                        hi = mid;
                    }
                }
                ++diff[1];
                --diff[lo + 1];
                hi = lo + 1; // 组数要求更多时，需要的 x 只会更小
            }
        }
    }

    for (int x = 1; x <= N; ++x) {
        diff[x] += diff[x - 1];
        printf("%d\n", diff[x] + bigAnswer[x]);
    }
    return 0;
}
