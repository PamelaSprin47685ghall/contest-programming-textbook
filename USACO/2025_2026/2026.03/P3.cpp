#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;

    string t;
    cin >> t;

    vector<string> s(n);
    for (int i = 0; i < n; i++) cin >> s[i];

    vector<vector<int>> has(n, vector<int>(26, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            has[i][s[i][j] - 'a']++;
        }
    }

    vector<array<int, 4>> ops;

    auto op = [&](int type, int a, int b, int c) {
        // store 1-indexed for printing
        ops.push_back({type, a + 1, b + 1, c + 1});

        if (type == 1) {
            swap(s[a][b], s[a][c]);
        } else {
            has[a][s[a][c] - 'a']--;
            has[b][s[b][c] - 'a']--;
            swap(s[a][c], s[b][c]);
            has[a][s[a][c] - 'a']++;
            has[b][s[b][c] - 'a']++;
        }
    };

    // Swap within s1 to fix as many positions as possible
    for (int i = 0; i < m; i++) {
        if (s[0][i] != t[i]) {
            for (int j = 0; j < m; j++) {
                if (s[0][j] == t[i] && s[0][j] != t[j]) {
                    op(1, 0, i, j);
                    break;
                }
            }
        }
    }

    // Import needed characters from other rows
    for (int i = 0; i < m; i++) {
        if (s[0][i] != t[i]) {
            for (int j = 1; j < n; j++) {
                if (has[j][t[i] - 'a']) {
                	// find the column
                    int idx = int(find(s[j].begin(), s[j].end(), t[i]) - s[j].begin());
                    op(1, j, i, idx);
                    op(2, 0, j, i);
                    break;
                }
            }
        }
    }

    printf("%d\n", (int)ops.size());
    for (auto &v : ops) {
        printf("%d %d %d %d\n", v[0], v[1], v[2], v[3]);
    }
}

int main() {
    cin.tie(0) -> sync_with_stdio(0);

    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}