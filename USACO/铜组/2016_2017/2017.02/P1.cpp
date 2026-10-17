#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    // side[cow] 记录每头牛最近一次看到的所在侧, -1 表示还没出现过.
    vector<int> side(11, -1);
    int answer = 0;
    for (int i = 0; i < n; ++i) {
        int cow, s;
        scanf("%d%d", &cow, &s);
        // 上次在另一侧才算一次过路, 同一侧重复记录不算.
        if (side[cow] + s == 1) {
            ++answer;
        }
        side[cow] = s;
    }
    printf("%d\n", answer);
    return 0;
}
