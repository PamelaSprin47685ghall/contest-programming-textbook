#include <cstdio>
#include <string>

// 方向编号 d 对应的位移: 0=(0,1), 1=(-1,0), 2=(0,-1), 3=(1,0).
// 初始朝向下, 牛的前后左右分别是 F/L/B/R, 与 d=0..3 一一对应.
const int DX[4] = {0, -1, 0, 1};
const int DY[4] = {1, 0, -1, 0};

char buffer[4];

// 记录四只脚踩过的场地范围.
struct Board {
    int minX = 0, maxX = 1, minY = -1, maxY = 0;

    // 脚走到新位置时扩展场地范围.
    void Extend(int x, int y) {
        if (x < minX) minX = x;
        if (maxX < x) maxX = x;
        if (y < minY) minY = y;
        if (maxY < y) maxY = y;
    }

    int Size() const { return (maxX - minX + 1) * (maxY - minY + 1); }
};

struct Foot {
    int x, y;
};

// 模拟牛的舞蹈: 四只脚的位置 + 当前朝向 (方向编号).
struct Cow {
    Board* board;
    int direction = 0;
    Foot feet[4] = {{0, 0}, {1, 0}, {0, -1}, {1, -1}};

    // 把脚 no 朝牛前方方向 d 移动一格; 若踩到别的脚则失败.
    bool MoveOK(int no, int d) {
        d = (d + direction) % 4;
        feet[no].x += DX[d];
        feet[no].y += DY[d];
        board->Extend(feet[no].x, feet[no].y);
        for (int i = 0; i < 4; ++i) {
            if (i != no && feet[i].x == feet[no].x && feet[i].y == feet[no].y) {
                return false;
            }
        }
        return true;
    }

    // 以脚 no 为轴旋转: 方向编号减 1, 其余脚按 (dx,dy)->(dy,-dx) 旋转.
    bool RotateOK(int no) {
        direction = (direction + 3) % 4;
        for (int i = 0; i < 4; ++i) {
            int dX = feet[i].x - feet[no].x;
            int dY = feet[i].y - feet[no].y;
            feet[i].x = feet[no].x + dY;
            feet[i].y = feet[no].y - dX;
            board->Extend(feet[i].x, feet[i].y);
        }
        return true;
    }

    // 解析一条指令: 前两个字符是脚 (FL/FR/RL/RR), 后一个字符是动作.
    bool MoveOrRotate(const std::string& s) {
        int no = 0;
        if (s[0] == 'R') no += 2;
        if (s[1] == 'R') no += 1;
        if (s[2] == 'F') return MoveOK(no, 0);
        if (s[2] == 'L') return MoveOK(no, 1);
        if (s[2] == 'B') return MoveOK(no, 2);
        if (s[2] == 'R') return MoveOK(no, 3);
        return RotateOK(no);  // 'P'
    }

    bool ReadAndProcess() {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            scanf("%s", buffer);
            if (!MoveOrRotate(buffer)) return false;
        }
        return true;
    }
};

int main() {
    Board board;
    Cow cow;
    cow.board = &board;
    if (cow.ReadAndProcess()) {
        printf("%d\n", board.Size());
    } else {
        printf("-1\n");
    }
    return 0;
}