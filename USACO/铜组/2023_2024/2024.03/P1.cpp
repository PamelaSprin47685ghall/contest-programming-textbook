#include <cstdio>
#include <string>
#include <vector>

using namespace std;

const int INF = 1000000000;

int main() {
    int n, q;
    if (scanf("%d%d", &n, &q) != 2) return 0;

    char buf[16];
    vector<string> s(n);
    for (int i = 0; i < n; ++i) {
        scanf("%s", buf);
        s[i] = buf;
    }

    vector<int> group(n);
    int groupNo = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == "or") {
            ++groupNo;
        } else if (i % 2 == 0) {
            group[i] = groupNo;
        }
    }

    vector<int> firstFalse(groupNo + 1, INF);
    vector<int> lastFalse(groupNo + 1, -1);
    for (int i = 0; i < n; i += 2) {
        int g = group[i];
        if (s[i] == "false") {
            if (firstFalse[g] == INF) {
                firstFalse[g] = i;
            }
            lastFalse[g] = i;
        }
    }

    int totalFirstTrue = INF;
    int totalLastTrue = -1;
    for (int i = 0; i <= groupNo; ++i) {
        if (firstFalse[i] == INF) {
            if (totalFirstTrue == INF) {
                totalFirstTrue = i;
            }
            totalLastTrue = i;
        }
    }

    while (q--) {
        int l, r;
        scanf("%d%d%s", &l, &r, buf);
        string target = buf;
        --l; --r;

        int lGroup = group[l];
        int rGroup = group[r];

        // 如果在被替换区间外侧已经存在恒为 true 的与块, 整个表达式恒为 true
        if (totalFirstTrue < lGroup || totalLastTrue > rGroup) {
            printf("%c", target == "true" ? 'Y' : 'N');
            continue;
        }

        if (target == "true") {
            // 检查 lGroup 在 l 之前是否已经含有 false, 以及 rGroup 在 r 之后是否含有 false
            printf("%c", firstFalse[lGroup] < l || lastFalse[rGroup] > r ? 'N' : 'Y');
        } else {
            // 总是可以通过把该区间替换为 false 使得包含该区间的这部分与块为 false
            printf("Y");
        }
    }
    printf("\n");
    return 0;
}
