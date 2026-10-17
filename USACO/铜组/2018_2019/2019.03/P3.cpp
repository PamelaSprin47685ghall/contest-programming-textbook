#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

struct CowSubpop {
    vector<string> traits;

    bool HasTrait(const string& t) const {
        for (const string& s : traits) {
            if (s == t) return true;
        }
        return false;
    }
};

int main() {
    int n;
    scanf("%d", &n);

    vector<CowSubpop> cows(n);
    vector<string> allTraits;
    char buf[80];

    for (int i = 0; i < n; ++i) {
        int k;
        scanf("%d", &k);
        cows[i].traits.resize(k);
        for (int j = 0; j < k; ++j) {
            scanf("%s", buf);
            cows[i].traits[j] = buf;
            allTraits.push_back(cows[i].traits[j]);
        }
    }

    // 特征去重
    sort(allTraits.begin(), allTraits.end());
    allTraits.erase(unique(allTraits.begin(), allTraits.end()), allTraits.end());

    // 检查是否有任何一对特征 (A, B) 发生交叉:
    // 即存在同时拥有 (A, B) 的个体, 拥有 (A 且非 B) 的个体, 以及 (B 且非 A) 的个体.
    int traitCount = allTraits.size();
    for (int i = 0; i < traitCount; ++i) {
        for (int j = i + 1; j < traitCount; ++j) {
            bool hasBoth = false;
            bool hasOnlyA = false;
            bool hasOnlyB = false;

            for (const CowSubpop& cow : cows) {
                bool a = cow.HasTrait(allTraits[i]);
                bool b = cow.HasTrait(allTraits[j]);
                if (a && b) hasBoth = true;
                if (a && !b) hasOnlyA = true;
                if (!a && b) hasOnlyB = true;
            }

            if (hasBoth && hasOnlyA && hasOnlyB) {
                printf("no\n");
                return 0;
            }
        }
    }

    printf("yes\n");
    return 0;
}
