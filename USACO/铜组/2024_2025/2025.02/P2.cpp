#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int N; scanf("%d", &N);
    vector<int> A(N);
    for (int i = 0; i < N; i++) scanf("%d", &A[i]);

    // cnt[i] = # of times the number `i` appears in A
    vector<int> cnt(N+1);
    for (int x : A) {
        cnt[x]++;
    }

    // missing_lt_i = how many numbers in [0..i) are missing from A?
    int missing_lt_i = 0;

    for (int i = 0; i <= N; i++) {
        printf("%d\n", max(cnt[i], missing_lt_i));
        bool is_i_missing_from_A = cnt[i] == 0;
        if (is_i_missing_from_A) {
            missing_lt_i++;
        }
    }
}
