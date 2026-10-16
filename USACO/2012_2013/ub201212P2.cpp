#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

char buffer[80];

int main() {
    int n;
    scanf("%d", &n);
    std::vector<std::string> original(n);
    std::vector<std::string> sortedAscending(n);   // 每个串正排后整体升序.
    std::vector<std::string> sortedDescending(n);  // 每个串倒排后整体升序.

    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        original[i] = buffer;
        sortedAscending[i] = buffer;
        std::sort(sortedAscending[i].begin(), sortedAscending[i].end());
        sortedDescending[i] = sortedAscending[i];
        std::reverse(sortedDescending[i].begin(), sortedDescending[i].end());
    }
    std::sort(sortedAscending.begin(), sortedAscending.end());
    std::sort(sortedDescending.begin(), sortedDescending.end());

    for (std::string& s : original) {
        // 该串正排后能排到的最前位置: 在倒排序列里找它 (倒排序列是字典序最小的排列).
        std::sort(s.begin(), s.end());
        int earliest = (int)(std::lower_bound(sortedDescending.begin(), sortedDescending.end(), s)
                             - sortedDescending.begin());
        // 该串倒排后能排到的最后位置: 在正排序列里找它.
        std::reverse(s.begin(), s.end());
        int latest = (int)(std::upper_bound(sortedAscending.begin(), sortedAscending.end(), s)
                           - sortedAscending.begin()) - 1;
        printf("%d %d\n", earliest + 1, latest + 1);
    }
    return 0;
}