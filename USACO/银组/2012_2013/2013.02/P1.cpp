#include <cstdio>
#include <set>
#include <algorithm>

using namespace std;

const int MAXN = 50005;
const int MAXC = 1000005;

int N;
int baleX[MAXN], baleY[MAXN];

// 坐标范围到 1,000,000, 开不下二维数组, 所以把每个格子的坐标编码成一个整数放进 set.
set<long long> bales;

long long Encode(int x, int y) {
    // 乘数比 y 的最大取值大, 保证不同格子的编码不冲突.
    return (long long)x * 2000003LL + y;
}

bool IsBale(int x, int y) {
    return bales.find(Encode(x, y)) != bales.end();
}

// 方向按逆时针编号: 0 东, 1 北, 2 西, 3 南.
// 这样 (d+1)%4 一定是左转, (d+3)%4 一定是右转.
const int DX[4] = {1, 0, -1, 0};
const int DY[4] = {0, 1, 0, -1};

// 沿方向 d 走一条单位边时, 这条边左手边与右手边格子的左下角相对当前格点的偏移.
// 例: 向北走时左边是西侧格子 (x-1,y), 右边是东侧格子 (x,y).
const int LX[4] = {0, -1, -1, 0};
const int LY[4] = {0, 0, -1, -1};
const int RX[4] = {0, 0, -1, -1};
const int RY[4] = {-1, 0, 0, -1};

// 判断从格点 (x,y) 沿方向 d 走的这条边是不是"贴着区域左侧"的边界边:
// 左手边必须是干草垛 (紧贴区域), 右手边必须是空地 (否则这条边夹在区域内部).
// 洞的边界也是"左边干草垛, 右边空地", 靠行进方向的选择把它们排除在外.
bool IsBorder(int x, int y, int d) {
    return IsBale(x + LX[d], y + LY[d]) && !IsBale(x + RX[d], y + RY[d]);
}

int main() {
    scanf("%d", &N);
    int startX = MAXC, startY = MAXC;
    for (int i = 0; i < N; ++i) {
        scanf("%d %d", &baleX[i], &baleY[i]);
        bales.insert(Encode(baleX[i], baleY[i]));
        // 最下 (y 最小) 且最左 (x 最小) 的格子: 它的左下角是标准的外边界顶点,
        // 从那里沿底边向东走, 区域正好在左手边.
        if (baleY[i] < startY || (baleY[i] == startY && baleX[i] < startX)) {
            startY = baleY[i];
            startX = baleX[i];
        }
    }

    // 沿着外边界走一圈, 每一步都是一条单位边, 步数就是周长.
    int x = startX, y = startY, d = 0, perimeter = 0;
    do {
        x += DX[d];
        y += DY[d];
        ++perimeter;
        // 能右转就右转, 其次直行, 最后左转.
        // 关键在于两条对角干草垛相碰时 (外边界与洞的边界共用这个格点),
        // 优先右转才会沿着外侧走, 优先左转就会拐进洞里, 把洞的周长也数进去.
        int nd = (d + 3) % 4;
        if (!IsBorder(x, y, nd)) {
            nd = d;
            if (!IsBorder(x, y, nd)) nd = (d + 1) % 4;
        }
        d = nd;
        // 回到出发点且方向与出发时相同, 说明整圈走完, 且不会漏算也不会重复绕洞.
    } while (!(x == startX && y == startY && d == 0));

    printf("%d\n", perimeter);
    return 0;
}
