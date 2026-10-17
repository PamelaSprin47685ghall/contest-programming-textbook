#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

struct Constraint {
    string cow1;
    string cow2;
};

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    vector<Constraint> constraints(n);
    for (int i = 0; i < n; ++i) {
        char name1[64], name2[64], junk[64];
        scanf("%s", name1);
        for (int j = 0; j < 4; ++j) scanf("%s", junk);
        scanf("%s", name2);
        constraints[i].cow1 = name1;
        constraints[i].cow2 = name2;
    }

    // 8 头奶牛的名字
    vector<string> cows = {
        "Bessie", "Buttercup", "Belinda", "Beatrice",
        "Bella", "Blue", "Betsy", "Sue"
    };

    // 按照字典序从小到大排序, 保证全排列搜索最先得到字典序最小的方案
    sort(cows.begin(), cows.end());

    do {
        bool valid = true;
        for (const Constraint& c : constraints) {
            // 找到两头牛在当前排列中的位置
            int pos1 = -1, pos2 = -1;
            for (int i = 0; i < 8; ++i) {
                if (cows[i] == c.cow1) pos1 = i;
                if (cows[i] == c.cow2) pos2 = i;
            }
            if (abs(pos1 - pos2) != 1) {
                valid = false;
                break;
            }
        }

        if (valid) {
            for (int i = 0; i < 8; ++i) {
                printf("%s\n", cows[i].c_str());
            }
            return 0;
        }
    } while (next_permutation(cows.begin(), cows.end()));

    return 0;
}
