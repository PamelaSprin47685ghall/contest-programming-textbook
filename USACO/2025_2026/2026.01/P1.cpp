#include <iostream>
#include <algorithm>

void solve() {
    long long A, B, cA, cB, fA;
    std::cin >> A >> B >> cA >> cB >> fA;
    long long init = B / cB * cA + A;
    if (init >= fA) {
        std::cout << 0 << "\n";
        return;
    }
    long long nA0 = fA - 1 - init;
    long long y = cB - 1 - B % cB;
    if (cA >= cB) {
        y += nA0;
    } else {
        y += nA0 / cA * cB + nA0 % cA;
    }
    std::cout << y + 1 << "\n";
}

int main() {
    int t;
    if (std::cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}
