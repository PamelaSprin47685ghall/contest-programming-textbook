#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

char buffer[80];

int main() {
    int n;
    scanf("%d", &n);
    vector<string> original(n);
    vector<string> sortedAscending(n);   // 每个串正排后整体升序.
    vector<string> sortedDescending(n);  // 每个串倒排后整体升序.

    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        original[i] = buffer;
        sortedAscending[i] = buffer;
        sort(sortedAscending[i].begin(), sortedAscending[i].end());
        sortedDescending[i] = sortedAscending[i];
        reverse(sortedDescending[i].begin(), sortedDescending[i].end());
    }
    sort(sortedAscending.begin(), sortedAscending.end());
    sort(sortedDescending.begin(), sortedDescending.end());

    for (string& s : original) {
        // 该串正排后能排到的最前位置: 在倒排序列里找它 (倒排序列是字典序最小的排列).
        sort(s.begin(), s.end());
        int earliest = (int)(lower_bound(sortedDescending.begin(), sortedDescending.end(), s)
                             - sortedDescending.begin());
        // 该串倒排后能排到的最后位置: 在正排序列里找它.
        reverse(s.begin(), s.end());
        int latest = (int)(upper_bound(sortedAscending.begin(), sortedAscending.end(), s)
                           - sortedAscending.begin()) - 1;
        printf("%d %d\n", earliest + 1, latest + 1);
    }
    return 0;
}
