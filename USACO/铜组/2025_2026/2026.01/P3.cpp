#include <algorithm>
#include <ios>
#include <iostream>
#include <vector>

using namespace std;

void solve() {

    int k;
    int n;
    int q;

    cin >> n >> k >> q;

    int mx_sum = 0;
    vector<vector<int>> sums(n, vector<int>(n));
    vector<vector<int>> vals(n, vector<int>(n));

    for (int i = 0; i < q; ++i) {
        int c;
        int r;
        int v;
        cin >> r >> c >> v;
        --r;
        --c;
        const int diff = v - vals[r][c];
        vals[r][c] = v;
        for (int j = max(r - k + 1, 0); j <= min(r, n - k); ++j) {
            for (int l = max(c - k + 1, 0); l <= min(c, n - k); ++l) {
                sums[j][l] += diff;
                mx_sum = max(mx_sum, sums[j][l]);
            }
        }
        cout << mx_sum << '\n';
    }

}

int main() {

    cin.tie(nullptr);

    ios_base::sync_with_stdio(false);

    solve();

    return 0;

}