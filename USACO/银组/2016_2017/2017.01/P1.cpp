#include <cstdio>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

const int MAXN = 10000;

int n, tmax;
int d[MAXN + 5];

// 判断舞台大小为 k 时, 整场演出能否在 tmax 内结束
bool possible(int k) {
    // 小根堆存放台上每头奶牛的结束时刻, 堆顶即最早空出的位置
    priority_queue<int, vector<int>, greater<int> > finish;
    int earliest = 0;               // 下一头奶牛最早能登台的时刻
    for (int i = 1; i <= n; i++) {
        if ((int)finish.size() == k) {
            // 舞台已满, 必须等最早结束的那头奶牛离场
            earliest = finish.top();
            finish.pop();
        }
        // 这头奶牛最早只能在 earliest 开始, 若它仍超时, 则本 k 不可行
        if (earliest + d[i] > tmax) return false;
        finish.push(earliest + d[i]);
    }
    return true;
}

int main() {
    scanf("%d%d", &n, &tmax);
    for (int i = 1; i <= n; i++) scanf("%d", &d[i]);

    // 多一个位置只会让奶牛更早登场, 所以可行性关于 k 单调, 可以二分
    int lo = 1, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (possible(mid)) hi = mid;
        else lo = mid + 1;
    }
    printf("%d\n", lo);
    return 0;
}
