#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1005;
const int INF = 1000000000;

struct Cow {
    int X, Y;
};

int N;
Cow cow[MAXN];
int candY[MAXN]; // 候选的东西向栅栏高度（都是偶数，不会穿过奶牛）

// 按 x 坐标排序：南北向栅栏自左向右移动时，奶牛依次从右边进入左边。
bool cmpByX(const Cow& a, const Cow& b) {
    return a.X < b.X;
}

int main() {
    scanf("%d", &N);
    int minY = INF;
    for (int i = 0; i < N; ++i) {
        scanf("%d%d", &cow[i].X, &cow[i].Y);
        if (cow[i].Y < minY) minY = cow[i].Y;
    }
    sort(cow, cow + N, cmpByX);

    // 栅栏的高度只需要取每头奶牛的 y+1（偶数、且紧贴奶牛上方），
    // 再加上 y_min-1（最下面一块区域为空）。重复的高度不影响答案。
    int cnt = 0;
    candY[cnt++] = minY - 1;
    for (int i = 0; i < N; ++i) candY[cnt++] = cow[i].Y + 1;

    int answer = N; // 四块区域里最多也就 N 头奶牛
    for (int t = 0; t < cnt; ++t) {
        int b = candY[t];
        int below = 0; // 栅栏下方的奶牛总数
        for (int i = 0; i < N; ++i)
            if (cow[i].Y < b) ++below;
        int above = N - below;

        int leftBelow = 0; // 已经移到栅栏左边的奶牛中，位于下方的头数
        for (int i = 0; i <= N; ++i) {
            // 栅栏放在第 i 头奶牛左边，此时左边共有 i 头奶牛。
            // x 坐标是奇数、栅栏坐标是偶数，所以同 x 的奶牛必定在同一边：
            // 只有当第 i 头奶牛与第 i-1 头奶牛的 x 不同时，这个前缀才是合法栅栏位置。
            bool legal = (i == 0 || i == N || cow[i - 1].X != cow[i].X);
            if (legal) {
                int leftAbove = i - leftBelow;
                int rightBelow = below - leftBelow;
                int rightAbove = above - leftAbove;
                int worst = max(max(leftBelow, leftAbove), max(rightBelow, rightAbove));
                if (worst < answer) answer = worst;
            }
            if (i < N && cow[i].Y < b) ++leftBelow; // 把第 i 头奶牛并入左侧
        }
    }
    printf("%d\n", answer);
    return 0;
}
