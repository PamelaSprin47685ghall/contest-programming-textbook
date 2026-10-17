#include <cstdio>
#include <algorithm>

using namespace std;

void solve() {
    long long A, B, cA, cB, fA;
    scanf("%lld %lld %lld %lld %lld", &A, &B, &cA, &cB, &fA);
    long long init = B / cB * cA + A;
    if (init >= fA) {
        printf("0\n");
        return;
    }
    long long nA0 = fA - 1 - init;
    long long y = cB - 1 - B % cB;
    if (cA >= cB) {
        y += nA0;
    } else {
        y += nA0 / cA * cB + nA0 % cA;
    }
    printf("%lld\n", y + 1);
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            solve();
        }
    }
    return 0;
}
