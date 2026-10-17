#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Cow {
    int begin;
    int duration;
};

// 按开始挤奶的时间排序, 先到的先处理.
bool operator<(const Cow& first, const Cow& second) {
    return first.begin < second.begin;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<Cow> cows(n);
    for (Cow& cow : cows) {
        scanf("%d%d", &cow.begin, &cow.duration);
    }
    sort(cows.begin(), cows.end());
    // 若下一头牛来晚了就等它, 否则接着上一头结束的时刻继续挤.
    int finish = 0;
    for (const Cow& cow : cows) {
        finish = max(finish, cow.begin);
        finish += cow.duration;
    }
    printf("%d\n", finish);
    return 0;
}
