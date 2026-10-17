#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// 一次产奶记录.
struct Record {
    int Day;
    string Cow;
    int Delta;
};

int main() {
    int n;
    scanf("%d", &n);
    vector<Record> records(n);
    char buffer[80];
    for (int i = 0; i < n; ++i) {
        scanf("%d%s%d", &records[i].Day, buffer, &records[i].Delta);
        records[i].Cow = buffer;
    }
    // 按天数排序, 同一天的多条记录要一起结算.
    sort(records.begin(), records.end(),
              [](const Record& left, const Record& right) {
                  return left.Day < right.Day;
              });

    unordered_map<string, int> milk;
    milk["Bessie"] = 7;
    milk["Elsie"] = 7;
    milk["Mildred"] = 7;
    // 当前展示在墙上的牛.
    unordered_set<string> display = {"Bessie", "Elsie", "Mildred"};

    int changes = 0;
    int i = 0;
    while (i < n) {
        // 把同一天的所有记录一次性应用, 再比较展示墙.
        int day = records[i].Day;
        while (i < n && records[i].Day == day) {
            milk[records[i].Cow] += records[i].Delta;
            ++i;
        }
        // 找当前产奶量最高的值. 产奶量可能为负, 所以从第一个值开始比.
        int best = milk.begin()->second;
        for (const auto& entry : milk) best = max(best, entry.second);
        unordered_set<string> nextDisplay;
        for (const auto& entry : milk) {
            if (entry.second == best) nextDisplay.insert(entry.first);
        }
        if (nextDisplay != display) ++changes;
        display = nextDisplay;
    }
    printf("%d\n", changes);
    return 0;
}