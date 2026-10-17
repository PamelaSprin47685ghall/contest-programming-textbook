#include <iostream>
#include <string>
#include <vector>

void Rotate(std::vector<std::string>& stamp, int k) {
    std::vector<std::string> temp = stamp;
    for (int r = 0; r < k; ++r) {
        for (int c = 0; c < k; ++c) {
            stamp[c][k - 1 - r] = temp[r][c];
        }
    }
}

void Solve() {
    int n;
    std::cin >> n;
    std::vector<std::string> target(n);
    for (int i = 0; i < n; ++i) std::cin >> target[i];

    int k;
    std::cin >> k;
    std::vector<std::string> stamp(k);
    for (int i = 0; i < k; ++i) std::cin >> stamp[i];

    // canvas 记录能够合法印出的黑色格子
    std::vector<std::string> canvas(n, std::string(n, '.'));

    // 尝试 4 种旋转角度
    for (int rot = 0; rot < 4; ++rot) {
        // 枚举印章盖在画布上的所有可能左上角位置 (r, c)
        for (int r = 0; r + k <= n; ++r) {
            for (int c = 0; c + k <= n; ++c) {
                // 检查若在此处盖章, 是否会盖到原本应该为空白 '.' 的格子上
                bool canStamp = true;
                for (int dr = 0; dr < k; ++dr) {
                    for (int dc = 0; dc < k; ++dc) {
                        if (stamp[dr][dc] == '*' && target[r + dr][c + dc] == '.') {
                            canStamp = false;
                            break;
                        }
                    }
                    if (!canStamp) break;
                }

                // 若完全合法, 则盖下印章 (将对应有墨水的格子涂黑)
                if (canStamp) {
                    for (int dr = 0; dr < k; ++dr) {
                        for (int dc = 0; dc < k; ++dc) {
                            if (stamp[dr][dc] == '*') {
                                canvas[r + dr][c + dc] = '*';
                            }
                        }
                    }
                }
            }
        }
        Rotate(stamp, k);
    }

    if (canvas == target) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
}

int main() {
    int t;
    if (std::cin >> t) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
