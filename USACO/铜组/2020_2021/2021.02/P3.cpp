#include <iostream>
#include <string>

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    for (int t = 0; t < n; ++t) {
        std::string s;
        std::cin >> s;

        // 统计沿闭合路径走一周转过的总拐角和
        // 右转 (顺时针) 记为 +90 度, 左转 (逆时针) 记为 -90 度
        int totalTurn = 0;
        int len = s.size();

        for (int i = 0; i < len; ++i) {
            char c1 = s[i];
            char c2 = s[(i + 1) % len];
            if (c1 == c2) continue;

            if ((c1 == 'N' && c2 == 'E') ||
                (c1 == 'E' && c2 == 'S') ||
                (c1 == 'S' && c2 == 'W') ||
                (c1 == 'W' && c2 == 'N')) {
                totalTurn += 90;
            } else {
                totalTurn -= 90;
            }
        }

        // 完整闭合无自交环路顺时针转角和必为 +360 度, 逆时针为 -360 度
        if (totalTurn > 0) {
            std::cout << "CW\n";
        } else {
            std::cout << "CCW\n";
        }
    }

    return 0;
}
