#include <cstdio>
#include <algorithm>
#include <set>

using namespace std;

const int MAXN = 50005;

struct Cow {
    int x;  // 位置
    int h;  // 高度
};

Cow cows[MAXN];

// 按位置排序后，一段距离内的奶牛才是连续的一段，便于用滑动窗口维护
bool cmpByPos(const Cow &a, const Cow &b) {
    return a.x < b.x;
}

int main() {
    int n;
    long long d;
    scanf("%d %lld", &n, &d);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &cows[i].x, &cows[i].h);
    }
    sort(cows, cows + n, cmpByPos);

    // 两个 multiset 分别存放两个滑动窗口里的奶牛高度：
    // rightWin = [x_i, x_i + D]，leftWin = [x_i - D, x_i]
    multiset<int> leftWin, rightWin;
    int add = 0;    // rightWin 下一个待加入的奶牛下标
    int drop = 0;   // leftWin 中最左边还在窗口内的奶牛下标
    int ans = 0;
    for (int i = 0; i < n; i++) {
        while (add < n && (long long)cows[add].x <= (long long)cows[i].x + d) {
            rightWin.insert(cows[add].h);
            add++;
        }
        while ((long long)cows[drop].x + d < cows[i].x) {
            leftWin.erase(leftWin.find(cows[drop].h));
            drop++;
        }
        leftWin.insert(cows[i].h);
        // 当前牛自己只有 h_i 高，绝不会满足 >= 2*h_i，
        // 所以窗口里包含自己也不会造成误判
        if (*leftWin.rbegin() >= 2LL * cows[i].h && *rightWin.rbegin() >= 2LL * cows[i].h) {
            ans++;
        }
        // 之后的奶牛位置更大，它不该再出现在右侧窗口里
        rightWin.erase(rightWin.find(cows[i].h));
    }
    printf("%d\n", ans);
    return 0;
}
