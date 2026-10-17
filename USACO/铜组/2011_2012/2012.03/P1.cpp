#include <algorithm>
#include <cstdio>
#include <string>

using namespace std;

// 二进制大数: 低位存前面 (Data[0] 是最低位), 方便逐位相加进位.
struct BigBinary {
    string digits;

    // 从字符串 (高位在前) 读入, 反转成低位在前的存储.
    void FromString(const string& s) {
        digits = s;
        reverse(digits.begin(), digits.end());
    }

    // 把另一个二进制数加到自身 (逐位相加, 处理进位).
    void Add(const BigBinary& other) {
        int carry = 0;
        for (int i = 0; i < (int)digits.size(); ++i) {
            int otherDigit = (i < (int)other.digits.size())
                                 ? other.digits[i] - '0'
                                 : 0;
            carry += (digits[i] - '0') + otherDigit;
            digits[i] = '0' + carry % 2;
            carry /= 2;
        }
        // 最高位进位后多出一位.
        if (carry != 0) digits += '0' + carry;
    }

    string ToString() const {
        string result(digits);
        reverse(result.begin(), result.end());
        return result;
    }
};

int main() {
    char buffer[1001];
    scanf("%s", buffer);
    string s = buffer;

    // N * 17 = N * 16 + N = N 后面补四个 0, 再与 N 相加.
    BigBinary shifted;
    shifted.FromString(s + "0000");
    BigBinary original;
    original.FromString(s);

    BigBinary answer = shifted;
    answer.Add(original);
    printf("%s\n", answer.ToString().c_str());
    return 0;
}