#include <cstdio>

using namespace std;

const int MAXN = 200000; // 拖拉机数上限
const int MAXLOG = 18;   // 2^18 = 262144 > 2*10^5，任何跳跃都能拆成这些二的幂

int N, Q;
int leftEnd[MAXN], rightEnd[MAXN]; // 每个区间的左右端点（编号按左、右端点同时递增）
char endpointString[2 * MAXN + 5]; // 端点序列（L 表示左端点，R 表示右端点）
int specialPrefix[MAXN];           // 前 i 个区间里特殊区间的个数

int rightParent[MAXN]; // 从 i 向右贪心一步能到达的最靠右区间
int leftParent[MAXN];  // 从 i 向左贪心一步能到达的最靠左区间

int upRight[MAXLOG][MAXN];        // 向右跳 2^k 步停在哪
long long sumRight[MAXLOG][MAXN]; // 向右跳 2^k 步，落点处的特殊前缀和之和
int upLeft[MAXLOG][MAXN];         // 向左跳 2^k 步停在哪
long long sumLeft[MAXLOG][MAXN];  // 向左跳 2^k 步，落点左侧（不含）的特殊区间数之和

int main() {
    scanf("%d%d", &N, &Q);
    scanf("%s", endpointString);

    int lCnt = 0, rCnt = 0;
    for (int i = 0; i < 2 * N; ++i) {
        if (endpointString[i] == 'L') leftEnd[lCnt++] = i;
        else rightEnd[rCnt++] = i;
    }

    scanf("%s", endpointString);
    for (int i = 0; i < N; ++i) {
        specialPrefix[i] = (i > 0 ? specialPrefix[i - 1] : 0) + (endpointString[i] == '1' ? 1 : 0);
    }

    // 右端点随编号递增，所以从 i 一步能到达的最右区间是"左端点不超过 rightEnd[i]"的最大编号。
    // 由于两个端点各自递增，这个编号随 i 单调不减，可以用双指针求出
    int p = 0;
    for (int i = 0; i < N; ++i) {
        while (p + 1 < N && leftEnd[p + 1] <= rightEnd[i]) ++p;
        rightParent[i] = p;
    }

    // 对称地，从 i 一步能向左到达的最靠左区间是"右端点不小于 leftEnd[i]"的最小编号
    p = N - 1;
    for (int i = N - 1; i >= 0; --i) {
        while (p - 1 >= 0 && rightEnd[p - 1] >= leftEnd[i]) --p;
        leftParent[i] = p;
    }

    for (int i = 0; i < N; ++i) {
        upRight[0][i] = rightParent[i];
        sumRight[0][i] = specialPrefix[rightParent[i]];
        upLeft[0][i] = leftParent[i];
        // 向左落点 x 左侧（严格小于 x）的特殊区间个数，就是区间 x 的"前缀区间特殊数"
        sumLeft[0][i] = (leftParent[i] == 0) ? 0 : specialPrefix[leftParent[i] - 1];
    }
    for (int k = 1; k < MAXLOG; ++k) {
        for (int i = 0; i < N; ++i) {
            int midR = upRight[k - 1][i];
            upRight[k][i] = upRight[k - 1][midR];
            sumRight[k][i] = sumRight[k - 1][i] + sumRight[k - 1][midR];

            int midL = upLeft[k - 1][i];
            upLeft[k][i] = upLeft[k - 1][midL];
            sumLeft[k][i] = sumLeft[k - 1][i] + sumLeft[k - 1][midL];
        }
    }

    for (int q = 0; q < Q; ++q) {
        int a, b;
        scanf("%d%d", &a, &b);
        --a;
        --b;

        // 贪心向右跳（跳一步到 rightParent）能到达的编号区间里，编号完全覆盖了"跳一步够得着"的范围，
        // 因此 dist(a, b) = 让 a 第一次跳到编号 >= b 所需的步数
        int cur = a, dist = 0;
        for (int k = MAXLOG - 1; k >= 0; --k) {
            if (upRight[k][cur] < b) {
                cur = upRight[k][cur];
                dist += (1 << k);
            }
        }
        ++dist; // 再走最后一步就能落到编号 >= b 的位置

        // 落在最短路径上的编号 i（1 <= i <= dist-1 步处）恰好构成区间 [leftParent 跳 dist-i 步, rightParent 跳 i 步]，
        // 于是特殊点总数 = sum_i(前缀和[rightParent 走 i 步]) - sum_i(前缀和[leftParent 走 dist-i 步]的左邻)
        long long sumFromA = 0, sumFromB = 0;
        int steps = dist - 1;
        int curA = a, curB = b;
        for (int k = 0; k < MAXLOG; ++k) {
            if ((steps >> k) & 1) {
                sumFromA += sumRight[k][curA];
                curA = upRight[k][curA];
                sumFromB += sumLeft[k][curB];
                curB = upLeft[k][curB];
            }
        }

        long long cnt = sumFromA - sumFromB; // 中间那些层上的特殊区间数
        if (endpointString[a] == '1') ++cnt; // 端点 a 与 b 单独计入
        if (endpointString[b] == '1') ++cnt;
        printf("%d %lld\n", dist, cnt);
    }
    return 0;
}
