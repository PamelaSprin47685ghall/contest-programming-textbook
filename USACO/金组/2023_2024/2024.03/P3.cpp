#include <cstdio>

using namespace std;

const int MAXN = 505;
const int MOD = 1000000007;

int N;
long long pA[MAXN];   // a 的前缀和
long long pB[MAXN];   // b 的前缀和
int dp[MAXN][MAXN];
int psm[MAXN][MAXN];  // 按行排序后的列前缀和：psm[r][c] = dp[r 序前 r+1 行][c] 之和
int rowOrd[MAXN][MAXN];  // rowOrd[i]：0..i-1 按 (pA[i]-pA[x])/(i-x) 升序的下标
int colOrd[MAXN][MAXN];  // colOrd[j]：0..j-1 按 (pB[j]-pB[x])/(j-x) 升序的下标
int tmp[MAXN];

// 排序用到的全局量：以 sortC 为右端点、sortP 为前缀和的平均分数比较
long long *sortP;
int sortC;

// (sortP[sortC]-sortP[x])/(sortC-x) < (sortP[sortC]-sortP[y])/(sortC-y)
// 用交叉相乘比较，避免除法误差；分母恒正
bool fracLess(int x, int y) {
    return (sortP[sortC] - sortP[x]) * (sortC - y) < (sortP[sortC] - sortP[y]) * (sortC - x);
}

// 归并排序；n 很小，自己写一个即可，比较函数依赖上面两个全局量
void mergeSort(int *a, int lo, int hi) {
    if (hi - lo <= 1) return;
    int mid = (lo + hi) / 2;
    mergeSort(a, lo, mid);
    mergeSort(a, mid, hi);
    int i = lo;
    int j = mid;
    int t = lo;
    while (i < mid && j < hi) {
        if (fracLess(a[j], a[i])) tmp[t++] = a[j++];
        else tmp[t++] = a[i++];
    }
    while (i < mid) tmp[t++] = a[i++];
    while (j < hi) tmp[t++] = a[j++];
    for (int k = lo; k < hi; k++) a[k] = tmp[k];
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        long long x;
        scanf("%lld", &x);
        pA[i] = pA[i - 1] + x;
    }
    for (int i = 1; i <= N; i++) {
        long long x;
        scanf("%lld", &x);
        pB[i] = pB[i - 1] + x;
    }

    // 预处理：对每个 i，把 0..i-1 按"以 i 为终点的子数组平均"升序排序
    for (int i = 1; i <= N; i++) {
        for (int v = 0; v < i; v++) {
            rowOrd[i][v] = v;
            colOrd[i][v] = v;
        }
        sortP = pA;
        sortC = i;
        mergeSort(rowOrd[i], 0, i);
        sortP = pB;
        sortC = i;
        mergeSort(colOrd[i], 0, i);
    }

    dp[0][0] = 1;
    for (int i = 1; i <= N; i++) {
        // 把第 i 行上方的行按"到 i 的平均值"升序排好后，对每一列做前缀和
        for (int r = 0; r < i; r++) {
            int row = rowOrd[i][r];
            for (int c = 0; c <= N; c++) {
                int v = dp[row][c];
                if (r > 0) {
                    v += psm[r - 1][c];
                    if (v >= MOD) v -= MOD;
                }
                psm[r][c] = v;
            }
        }
        for (int j = 1; j <= N; j++) {
            // 对固定的 (i,j)，列 c 按 (pB[j]-pB[c])/(j-c) 升序处理时，满足
            // (pA[i]-pA[x])/(i-x) <= (pB[j]-pB[c])/(j-c) 的行 x 是排好序的前缀，长度 r 单调不减
            int r = 0;
            for (int t = 0; t < j; t++) {
                int c = colOrd[j][t];
                while (r < i) {
                    int row = rowOrd[i][r];
                    // 交叉相乘比较，分母 (i-row)、(j-c) 均为正
                    if ((pA[i] - pA[row]) * (j - c) <= (pB[j] - pB[c]) * (i - row)) r++;
                    else break;
                }
                if (r > 0) {
                    dp[i][j] += psm[r - 1][c];
                    if (dp[i][j] >= MOD) dp[i][j] -= MOD;
                }
            }
        }
    }
    printf("%d\n", dp[N][N]);
    return 0;
}
