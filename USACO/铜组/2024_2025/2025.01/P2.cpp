#include <cstdio>
#include <cstring>

using namespace std;

#define MAXN 1000005

int a[MAXN], pref[MAXN], cnt[MAXN];
long long ans = 0;

int main() {
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++) {
        pref[i] = pref[i-1];
        cnt[a[i]]++;
        if (cnt[a[i]] == 1)
            pref[i]++;
    }
    memset(cnt, 0, sizeof cnt);
    for (int i = n; i >= 1; i--) {
        cnt[a[i]]++;
        if (cnt[a[i]] == 2) {
            ans += pref[i-1];
        }
    }
    for (int i = 1; i <= n; i++) {
        if (cnt[i] >= 3)
            ans--;
    }
    printf("%lld\n", ans);
}
