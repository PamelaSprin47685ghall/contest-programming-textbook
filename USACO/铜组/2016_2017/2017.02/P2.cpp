#include <cstdio>
#include <string>
#include <vector>

using namespace std;

char Buffer[80];

int main() {
    // 每头牛恰好出现两次, position[cow] 存这两个下标.
    vector<int> position[26];
    scanf("%s", Buffer);
    string s = Buffer;
    for (int i = 0; i < (int)s.size(); ++i) {
        position[s[i] - 'A'].emplace_back(i);
    }
    int answer = 0;
    // 两头牛的路交叉当且仅当一头的一个端点插在另一头的两个端点之间.
    for (const vector<int>& first : position) {
        for (const vector<int>& second : position) {
            if (first[0] < second[0] && second[0] < first[1] &&
                first[1] < second[1]) {
                ++answer;
            }
        }
    }
    printf("%d\n", answer);
    return 0;
}
