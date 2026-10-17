#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

void solve() {

    int k;
    int n;
    int q;

    scanf("%d %d %d", &n, &k, &q);

    int mx_sum = 0;
    vector<vector<int>> sums(n, vector<int>(n));
    vector<vector<int>> vals(n, vector<int>(n));

    for (int i = 0; i < q; ++i) {
        int c;
        int r;
        int v;
        scanf("%d %d %d", &r, &c, &v);
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
        printf("%d\n", mx_sum);
    }

}

int main() {

    solve();

    return 0;

}
