#include <cstdio>
#include <string>

char buffer[1000001];

// 判断 s0 是否以 s1 结尾。
bool endsWith(const std::string& s0, const std::string& s1) {
    if (s0.size() < s1.size()) return false;
    return s0.substr(s0.size() - s1.size()) == s1;
}

int main() {
    scanf("%s", buffer);
    std::string s = buffer;
    scanf("%s", buffer);
    std::string t = buffer;
    std::string result;
    // 逐字符构造结果；每加一个字符就检查是否形成待删词，形成则删掉。
    for (char c : s) {
        result += c;
        if (endsWith(result, t)) result.resize(result.size() - t.size());
    }
    printf("%s\n", result.c_str());
    return 0;
}
