#include <cstdio>
#include <string>

using namespace std;

char Buffer[101];

// 从原串 s 出发, 尝试所有可能的"取一半再拼接"方式得到更短的串.
// 每得到一个串就计数一次, 递归展开; 返回能得到的串的总数减 1(去掉原始串).
struct SecretCounter {
    int answer = 0;

    void Count(const string& s) {
        ++answer;
        int length = s.size();
        // 长度 2 及以下或偶数长度的串无法继续缩写.
        if (length <= 2 || length % 2 == 0) return;
        int half = length / 2;
        string left = s.substr(0, half);          // 前半
        string right = s.substr(half + 1, half);  // 后半
        string leftShift = s.substr(1, half);     // 去掉首字符的前半
        string rightShift = s.substr(half, half); // 去掉末字符的后半
        // 缩写方式: 取一半, 再拼上前/后半. 只有拼接后仍等于原串的一半
        // 才合法, 即两个半段相等.
        if (left == right) {
            Count(s.substr(half, half + 1));
            Count(s.substr(0, half + 1));
        }
        if (left == rightShift) Count(s.substr(half, half + 1));
        if (leftShift == right) Count(s.substr(0, half + 1));
    }
};

int main() {
    scanf("%s", Buffer);
    SecretCounter counter;
    counter.Count(Buffer);
    printf("%d\n", counter.answer - 1);
    return 0;
}