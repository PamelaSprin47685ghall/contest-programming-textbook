#include <cstdio>
#include <string>
#include <vector>

// 一段括号串的净效果: right 个未匹配右括号在前, left 个未匹配左括号在后.
struct Effect {
    int right = 0;
    int left = 0;

    // 合并两段效果: 相接处的右括号和左括号会互相抵消.
    Effect Combine(const Effect& that) const {
        int removed = (left < that.right) ? left : that.right;
        Effect result;
        result.right = right + that.right - removed;
        result.left = left + that.left - removed;
        return result;
    }
};

// 单个字符的效果.
Effect EffectOf(char c) {
    Effect e;
    if (c == '(') e.left = 1;
    else e.right = 1;
    return e;
}

int main() {
    char buffer[100001];
    scanf("%s", buffer);
    std::string s = buffer;

    // suffix[i]: 从 i 到末尾这一段串的净效果.
    std::vector<Effect> suffix(s.size() + 1);
    for (int i = (int)s.size() - 1; i >= 0; --i) {
        suffix[i] = EffectOf(s[i]).Combine(suffix[i + 1]);
    }

    int answer = 0;
    Effect prefix;  // prefix: 0..i-1 这一段串的净效果.
    for (int i = 0; i < (int)s.size(); ++i) {
        if (0 < i) prefix = prefix.Combine(EffectOf(s[i - 1]));
        // 翻转第 i 个字符后, 若整串效果为 (0,0) 则是合法的括号序列.
        char flipped = (s[i] == '(') ? ')' : '(';
        Effect total = prefix.Combine(EffectOf(flipped)).Combine(suffix[i + 1]);
        if (total.right == 0 && total.left == 0) ++answer;
    }
    printf("%d\n", answer);
    return 0;
}