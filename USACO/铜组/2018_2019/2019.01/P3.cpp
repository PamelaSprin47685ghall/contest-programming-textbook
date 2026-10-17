#include <cstdio>
#include <string>
#include <vector>

using namespace std;

struct Animal {
    string name;
    vector<string> traits;
};

// 计算两只动物拥有的共同特征数
int CommonTraits(const Animal& a, const Animal& b) {
    int common = 0;
    for (const string& t1 : a.traits) {
        for (const string& t2 : b.traits) {
            if (t1 == t2) {
                ++common;
                break;
            }
        }
    }
    return common;
}

int main() {
    int n;
    scanf("%d", &n);

    vector<Animal> animals(n);
    char buf[80];
    for (int i = 0; i < n; ++i) {
        int k;
        scanf("%s%d", buf, &k);
        animals[i].name = buf;
        animals[i].traits.resize(k);
        for (int j = 0; j < k; ++j) {
            scanf("%s", buf);
            animals[i].traits[j] = buf;
        }
    }

    // 任意两只动物共有的特征数量的最大值, 再加上 1 即为能回答 YES 的最大次数
    int maxCommon = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int common = CommonTraits(animals[i], animals[j]);
            if (common > maxCommon) {
                maxCommon = common;
            }
        }
    }

    printf("%d\n", maxCommon + 1);
    return 0;
}
