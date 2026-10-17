#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Cow {
    int a, b;
};

// 按 A 排序后, 检查每头牛的 B 是否同时大于左边最大值且小于右边最小值.
struct Cows {
    int n;
    vector<Cow> data;
    vector<int> prefixMaxB, suffixMinB;

    void Read() {
        scanf("%d", &n);
        data.resize(n);
        for (Cow& cow : data) scanf("%d%d", &cow.a, &cow.b);
    }

    int Compute() {
        sort(data.begin(), data.end(), [](const Cow& c0, const Cow& c1) {
            if (c0.a != c1.a) return c0.a < c1.a;
            return c0.b < c1.b;
        });
        prefixMaxB.resize(n);
        prefixMaxB[0] = data[0].b;
        for (int i = 1; i < n; ++i) {
            prefixMaxB[i] = max(prefixMaxB[i - 1], data[i].b);
        }
        suffixMinB.resize(n);
        suffixMinB[n - 1] = data[n - 1].b;
        for (int i = n - 2; i >= 0; --i) {
            suffixMinB[i] = min(suffixMinB[i + 1], data[i].b);
        }
        int answer = 0;
        for (int i = 0; i < n; ++i) {
            bool leftOK = (i == 0 || prefixMaxB[i - 1] <= data[i].b);
            bool rightOK = (i == n - 1 || data[i].b <= suffixMinB[i + 1]);
            if (leftOK && rightOK) ++answer;
        }
        return answer;
    }
};

int main() {
    Cows cows;
    cows.Read();
    printf("%d\n", cows.Compute());
    return 0;
}