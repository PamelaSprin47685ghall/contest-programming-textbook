#include <cstdio>
#include <string>
#include <vector>

using namespace std;

void rotateStamp(vector<string>& stamp, int k) {
    vector<string> temp = stamp;
    for (int r = 0; r < k; ++r) {
        for (int c = 0; c < k; ++c) {
            stamp[c][k - 1 - r] = temp[r][c];
        }
    }
}

void solve() {
    int n;
    scanf("%d", &n);
    vector<string> target(n);
    char buffer[25];
    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        target[i] = buffer;
    }

    int k;
    scanf("%d", &k);
    vector<string> stamp(k);
    for (int i = 0; i < k; ++i) {
        scanf("%s", buffer);
        stamp[i] = buffer;
    }

    // canvas 记录能够合法印出的黑色格子
    vector<string> canvas(n, string(n, '.'));

    // 尝试 4 种旋转角度
    for (int rot = 0; rot < 4; ++rot) {
        // 枚举印章盖在画布上的所有可能左上角位置 (r, c)
        for (int r = 0; r + k <= n; ++r) {
            for (int c = 0; c + k <= n; ++c) {
                // 检查若在此处盖章, 是否会盖到原本应该为空白 '.' 的格子上
                bool canStamp = true;
                for (int dr = 0; dr < k && canStamp; ++dr) {
                    for (int dc = 0; dc < k; ++dc) {
                        if (stamp[dr][dc] == '*' && target[r + dr][c + dc] == '.') {
                            canStamp = false;
                            break;
                        }
                    }
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
        rotateStamp(stamp, k);
    }

    if (canvas == target) printf("YES\n");
    else printf("NO\n");
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) solve();
    return 0;
}
