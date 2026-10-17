#include <cassert>
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (int tc = 0; tc < T; ++tc) {
        int N, K;
        cin >> N >> K;
        vector<int> cnt(N);
        for (int i = 0; i < N; ++i) {
            int x;
            cin >> x;
            ++cnt.at(x - 1);
        }
        if (K < 0) {
            reverse(begin(cnt), end(cnt));
            K *= -1;
        }
        assert(0 < K && K <= N);
        int64_t ans = 0;

        for (int i = 0; i < K; ++i) {  // process remainder i mod K
            int64_t cur_min = i;
            int cnt_min = 0;
            while (cur_min < N || cnt_min > 0) {
                if (cur_min < N) cnt_min += cnt.at(cur_min);
                if (cnt_min > 0) ans += cnt_min - 1;
                cur_min += K;
                if (cnt_min > 0) --cnt_min;
            }
        }

        cout << ans << "\n";
    }
}