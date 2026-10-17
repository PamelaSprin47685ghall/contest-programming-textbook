#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

void solve() {
    int n, k;
    scanf("%d %d", &n, &k);
    vector<int> cnt(n);
    for (int i = 0; i < n; ++i) {
        int value;
        scanf("%d", &value);
        ++cnt[value - 1];
    }
    if (k < 0) {
        reverse(cnt.begin(), cnt.end());
        k = -k;
    }
    long long ans = 0;

    // 余数相同的数才能互相撞车。每条链上多出来的数只能往更大的空位搬。
    for (int i = 0; i < k; ++i) {
        long long cur = i;
        int waiting = 0;
        while (cur < n || waiting > 0) {
            if (cur < n) waiting += cnt[(int)cur];
            if (waiting > 0) ans += waiting - 1;
            cur += k;
            if (waiting > 0) --waiting;
        }
    }

    printf("%lld\n", ans);
}

int main() {
    int testCount;
    scanf("%d", &testCount);
    while (testCount--) solve();
    return 0;
}
