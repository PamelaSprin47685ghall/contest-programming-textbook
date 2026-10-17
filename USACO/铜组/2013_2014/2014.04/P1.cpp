#include <cstdio>
#include <string>

using namespace std;

char Buffer[80];

// 按数值大小比较两个十进制串: 先比长度, 长度相同再按字典序.
bool LessOrEqual(const string& a, const string& b) {
    if (a.size() != b.size()) return a.size() < b.size();
    return a <= b;
}

int main() {
    scanf("%s", Buffer);
    string x = Buffer;
    scanf("%s", Buffer);
    string y = Buffer;

    int answer = 0;
    // 有趣数: 恰好一个数字出现一次, 其余全相同. 枚举主数字 b、
    // 唯一不同数字 j、长度 i、j 所在的位置 k.
    for (int b = 0; b < 10; ++b) {
        for (int length = 3; length <= 17; ++length) {
            for (int j = 0; j < 10; ++j) {
                if (j == b) continue;
                for (int k = 0; k < length; ++k) {
                    string number;
                    for (int pos = 0; pos < length; ++pos) {
                        int digit = pos == k ? j : b;
                        number += '0' + digit;
                    }
                    if (number[0] == '0') continue; // 不能有前导零
                    if (LessOrEqual(x, number) && LessOrEqual(number, y)) {
                        ++answer;
                    }
                }
            }
        }
    }
    printf("%d\n", answer);
    return 0;
}