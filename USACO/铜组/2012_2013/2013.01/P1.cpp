#include <cstdio>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

// 镜子按坐标组织: XY[x][y] 与 YX[y][x] 都指向同一块镜子的编号, 便于按行/列查找.
struct Board {
    int n;
    vector<int> direction;  // 0=右,1=上,2=左,3=下; 用异或 1/3 表示 '/' '\' 反射.
    unordered_map<int, map<int, int>> byX;
    unordered_map<int, map<int, int>> byY;

    void Read(int cowCount) {
        n = cowCount;
        direction.resize(n);
        for (int i = 0; i < n; ++i) {
            int x, y;
            char c;
            scanf("%d%d %c", &x, &y, &c);
            // '/' 把方向翻转 1 步, '\' 翻转 3 步.
            direction[i] = (c == '/') ? 1 : 3;
            byX[x][y] = i;
            byY[y][x] = i;
        }
    }

    // 沿当前方向前进到下一面镜子; 若出界返回 true.
    bool Advance(int& x, int& y, int& d) {
        if (d == 0) {  // 向右: 在 y 行找 x 右侧最近的镜子.
            auto it = byY[y].upper_bound(x);
            if (it == byY[y].end()) return true;
            x = it->first;
            d ^= direction[it->second];
        } else if (d == 1) {  // 向上: 在 x 列找 y 上方最近的镜子.
            auto it = byX[x].upper_bound(y);
            if (it == byX[x].end()) return true;
            y = it->first;
            d ^= direction[it->second];
        } else if (d == 2) {  // 向左: 在 y 行找 x 左侧最近的镜子.
            auto it = byY[y].lower_bound(x);
            if (it == byY[y].begin()) return true;
            --it;
            x = it->first;
            d ^= direction[it->second];
        } else {  // 向下: 在 x 列找 y 下方最近的镜子.
            auto it = byX[x].lower_bound(y);
            if (it == byX[x].begin()) return true;
            --it;
            y = it->first;
            d ^= direction[it->second];
        }
        return false;
    }
};

// 从 (0,0) 向右出发, 判断能否在到达终点前先出界.
struct Sight {
    Board* board;
    int endX, endY;

    bool GoFinish(int& x, int& y, int& d) {
        // 先判断当前是否已越过终点所在的行/列, 再决定出界时是否算到达.
        if (d == 0) {
            bool onLine = (y == endY && x <= endX);
            if (board->Advance(x, y, d)) return onLine;
            return onLine && endX <= x;
        }
        if (d == 1) {
            bool onLine = (x == endX && y <= endY);
            if (board->Advance(x, y, d)) return onLine;
            return onLine && endY <= y;
        }
        if (d == 2) {
            bool onLine = (y == endY && endX <= x);
            if (board->Advance(x, y, d)) return onLine;
            return onLine && x <= endX;
        }
        bool onLine = (x == endX && endY <= y);
        if (board->Advance(x, y, d)) return onLine;
        return onLine && y <= endY;
    }

    // 最多走 2N 步; 若没到达终点则必然陷入循环.
    bool See() {
        int x = 0, y = 0, d = 0;
        for (int step = board->n * 2; step > 0; --step) {
            if (GoFinish(x, y, d)) return true;
        }
        return false;
    }
};

int main() {
    int n, endX, endY;
    scanf("%d%d%d", &n, &endX, &endY);
    Board board;
    board.Read(n);

    Sight sight;
    sight.board = &board;
    sight.endX = endX;
    sight.endY = endY;
    if (sight.See()) {
        printf("0\n");
        return 0;
    }
    // 依次翻转每面镜子 (翻转方向 2 步), 找到第一个能到达终点的镜子.
    for (int i = 0; i < n; ++i) {
        board.direction[i] ^= 2;
        if (sight.See()) {
            printf("%d\n", i + 1);
            return 0;
        }
        board.direction[i] ^= 2;
    }
    printf("-1\n");
    return 0;
}
