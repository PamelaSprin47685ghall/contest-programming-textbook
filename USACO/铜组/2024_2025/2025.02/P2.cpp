#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; cin >> N;
    vector<int> A(N);
    for (int i = 0; i < N; i++) cin >> A[i];

    // cnt[i] = # of times the number `i` appears in A
    vector<int> cnt(N+1);
    for (int x : A) {
        cnt[x]++;
    }

    // missing_lt_i = how many numbers in [0..i) are missing from A?
    int missing_lt_i = 0;

    for (int i = 0; i <= N; i++) {
        cout << max(cnt[i], missing_lt_i) << "\n";
        bool is_i_missing_from_A = cnt[i] == 0;
        if (is_i_missing_from_A) {
            missing_lt_i++;
        }
    }
}