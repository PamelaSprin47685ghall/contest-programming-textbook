#include <cstdio>
#include <string>

char buffer[100001];

int main() {
    scanf("%*d%s", buffer);  // 长度无用，只读字符串。
    std::string s = buffer;
    long long countC = 0, countCO = 0, countCOW = 0;
    // 从左到右：每个 'O' 与前面所有 'C' 组成 CO，每个 'W' 与前面所有 CO 组成 COW。
    for (char c : s) {
        switch (c) {
            case 'C':
                ++countC;
                break;
            case 'O':
                countCO += countC;
                break;
            case 'W':
                countCOW += countCO;
                break;
        }
    }
    printf("%lld\n", countCOW);
    return 0;
}
