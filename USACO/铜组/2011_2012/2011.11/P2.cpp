#include <algorithm>
#include <cstdio>
#include <string>

using namespace std;

// 按 base 进制把字符串逐位累加成十进制整数.
int StringToInt(const string& s, int base) {
    int value = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        value = value * base + (s[i] - '0');
    }
    return value;
}

// 把十进制整数 x 转成 base 进制字符串.
string IntToString(int x, int base) {
    if (x == 0) return "0";
    string digits;
    while (x > 0) {
        digits += '0' + x % base;
        x /= base;
    }
    reverse(digits.begin(), digits.end());
    return digits;
}

// 统计两个等长字符串不同字符的个数; 长度不同返回 -1.
int HammingDistance(const string& a, const string& b) {
    if (a.size() != b.size()) return -1;
    int difference = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        if (a[i] != b[i]) ++difference;
    }
    return difference;
}

int main() {
    char buffer[80];
    scanf("%s", buffer);
    string binary = buffer;
    scanf("%s", buffer);
    string ternary = buffer;

    // 枚举二进制里哪一位写错 (翻转), 转成十进制再转三进制,
    // 若与给出的三进制恰好差 1 位, 该数就是答案.
    for (int i = 0; i < (int)binary.size(); ++i) {
        binary[i] = (binary[i] == '0') ? '1' : '0';
        int number = StringToInt(binary, 2);
        string ternaryGuess = IntToString(number, 3);
        if (HammingDistance(ternary, ternaryGuess) == 1) {
            printf("%d\n", number);
            return 0;
        }
        binary[i] = (binary[i] == '0') ? '1' : '0';
    }
    printf("-1\n");
    return 0;
}