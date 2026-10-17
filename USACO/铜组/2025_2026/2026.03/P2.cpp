#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

const long long MOD = 1000000007LL;

char digits[200005];

void solve() {
    scanf("%s", digits);
    string s = digits;
    long long moves = 0;
    int n = (int)s.size();
    vector<long long> power(n);
    power[0] = 1;
    for (int i = 1; i < n; ++i) power[i] = (power[i - 1] * 2) % MOD;

    // 不是 0/1 的数字先按奇偶压成 0/1，这一步只算一次。
    bool binary = true;
    for (int i = 0; i < n; ++i) {
        if (s[i] != '0' && s[i] != '1') binary = false;
    }
    if (!binary) {
        ++moves;
        for (int i = 0; i < n; ++i) s[i] = char(((s[i] - '0') % 2) + '0');
    }

    reverse(s.begin(), s.end());
    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') {
            if (i == 0) ++moves;
            else moves = (moves + power[i] + power[i - 1]) % MOD;
        }
    }
    printf("%lld\n", moves);
}

int main() {
    int testCount;
    scanf("%d", &testCount);
    while (testCount--) solve();
    return 0;
}
