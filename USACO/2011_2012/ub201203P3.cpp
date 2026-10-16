#include <cstdio>
#include <string>
#include <unordered_set>
#include <vector>

// 一条指令序列的效果: 先位移 (x, y), 再整体逆时针转 direction 次.
struct Effect {
    int x = 0;
    int y = 0;
    int direction = 0;

    // 把自身的效果整体逆时针转 90 度.
    Effect Rotate() const {
        // 逆时针旋转 90 度: (x, y) -> (-y, x), 朝向 +1.
        return Effect{-y, x, (direction + 1) % 4};
    }

    // 先执行 that, 再执行自身: 自身要按 that 的朝向旋转后再叠加位移.
    Effect Combine(const Effect& that) const {
        Effect result = that;
        for (int i = 0; i < direction; ++i) result = result.Rotate();
        result.x += x;
        result.y += y;
        return result;
    }
};

// 两个效果只要位移相同就视为相同终点 (最终朝向无关紧要).
struct EffectEq {
    bool operator()(const Effect& a, const Effect& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

struct EffectHash {
    size_t operator()(const Effect& e) const {
        return e.x * (size_t)9875321 + e.y;
    }
};

// 单条指令的效果: F 前进一格, L/R 原地转向.
Effect SingleEffect(char command) {
    if (command == 'F') return Effect{0, 1, 0};
    if (command == 'L') return Effect{0, 0, 1};
    return Effect{0, 0, 3};  // R: 逆时针 3 次即顺时针 1 次
}

// 把一条序列的效果合并起来.
Effect SequenceEffect(const std::string& s, int begin, int end) {
    Effect result;
    for (int i = begin; i < end; ++i) result = result.Combine(SingleEffect(s[i]));
    return result;
}

int main() {
    char buffer[100001];
    scanf("%s", buffer);
    std::string s = buffer;

    // 后缀效果: suffix[i] = 从 i 到末尾的序列效果.
    std::vector<Effect> suffix((int)s.size() + 1);
    for (int i = (int)s.size() - 1; i >= 0; --i) {
        suffix[i] = SingleEffect(s[i]).Combine(suffix[i + 1]);
    }

    // 枚举每个位置改成另外两种指令之一, 收集所有不同的终点.
    std::unordered_set<Effect, EffectHash, EffectEq> endpoints;
    Effect prefix;  // 当前位置之前的序列效果
    const char commands[3] = {'F', 'L', 'R'};

    for (int i = 0; i < (int)s.size(); ++i) {
        if (i > 0) prefix = prefix.Combine(SingleEffect(s[i - 1]));
        for (int c = 0; c < 3; ++c) {
            if (commands[c] == s[i]) continue;  // 必须真的改掉这一位
            Effect total = prefix.Combine(SingleEffect(commands[c]));
            total = total.Combine(suffix[i + 1]);
            endpoints.insert(total);
        }
    }

    printf("%d\n", (int)endpoints.size());
    return 0;
}