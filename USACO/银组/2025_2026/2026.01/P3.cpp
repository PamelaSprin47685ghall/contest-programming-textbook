#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1000005;

char windowParity[MAXN];

void solve() {
    int n, k;
    scanf("%d %d", &n, &k);
    scanf("%s", windowParity);

    // 每隔 K 位，后一位由窗口奇偶性唯一决定。每条链只有首项 0/1 两种选法。
    int minOnes = 0;
    int chosenParity = 0;
    int smallestGap = n;
    for (int start = 0; start < k; ++start) {
        int onesIfZero = 0;
        int length = 0;
        int bit = 0;
        for (int index = start; index < n; index += k) {
            onesIfZero += bit;
            ++length;
            if (index + k < n) {
                int left = windowParity[index + 1] - '0';
                int right = windowParity[index] - '0';
                bit ^= left ^ right;
            }
        }
        int onesIfOne = length - onesIfZero;
        minOnes += min(onesIfZero, onesIfOne);
        if (onesIfOne < onesIfZero) chosenParity ^= 1;
        int gap = onesIfZero - onesIfOne;
        if (gap < 0) gap = -gap;
        if (gap < smallestGap) smallestGap = gap;
    }

    int need = windowParity[0] - '0';
    int minAnswer = minOnes;
    if (chosenParity != need) minAnswer += smallestGap;

    // 全取另一面就是最大方案；它的首 K 位奇偶性还要再和 r_1 对一次。
    int maxAnswer = n - minOnes;
    int flippedParity = (chosenParity + k) & 1;
    if (flippedParity != need) maxAnswer -= smallestGap;
    printf("%d %d\n", minAnswer, maxAnswer);
}

int main() {
    int testCount;
    scanf("%d", &testCount);
    while (testCount--) solve();
    return 0;
}
