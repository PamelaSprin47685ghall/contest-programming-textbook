#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int chickenCount, cowCount;
multiset<int> chicken;              // 尚未被配对的所有鸡的帮忙时间
vector<pair<int, int> > cow;        // 每头牛记为 (右端点 B, 左端点 A)

int main() {
    scanf("%d%d", &chickenCount, &cowCount);
    for (int i = 0; i < chickenCount; i++) {
        int t;
        scanf("%d", &t);
        chicken.insert(t);
    }
    for (int i = 0; i < cowCount; i++) {
        int a, b;
        scanf("%d%d", &a, &b);
        cow.push_back(make_pair(b, a));     // 先按右端点排序, 故把 B 放在 first
    }

    // 右端点越小的牛能用的鸡越少, 越"紧迫", 因此优先满足它.
    sort(cow.begin(), cow.end());

    int answer = 0;
    for (int i = 0; i < cowCount; i++) {
        int a = cow[i].second, b = cow[i].first;
        // 在时间不少于 A 的鸡里取时间最小的那只: 时间更靠后的鸡留给右端点更晚的牛更有用.
        multiset<int>::iterator it = chicken.lower_bound(a);
        if (it != chicken.end() && *it <= b) {
            answer++;
            chicken.erase(it);
        }
    }
    printf("%d\n", answer);
    return 0;
}
