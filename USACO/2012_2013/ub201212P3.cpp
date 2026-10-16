#include <algorithm>
#include <cstdio>
#include <vector>

const int DX[4] = {1, 0, -1, 0};
const int DY[4] = {0, 1, 0, -1};

// 收集 values 里出现的值并排序去重, 返回去重后的有序表.
std::vector<int> UniqueSorted(std::vector<int>& values) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    return values;
}

struct Point {
    int x, y;
};

struct Fence {
    int x0, y0, x1, y1;
};

// 读入并离散化所有坐标, 把每个坐标映射到 2*rank+2 的位置, 中间留出空隙.
struct Input {
    int xSize = 0;
    int ySize = 0;
    std::vector<Point> cows;
    std::vector<Fence> fences;

    void Read() {
        int n, c;
        scanf("%d%d", &n, &c);
        fences.resize(n);
        for (Fence& f : fences) {
            scanf("%d%d%d%d", &f.x0, &f.y0, &f.x1, &f.y1);
            if (f.x1 < f.x0) std::swap(f.x0, f.x1);
            if (f.y1 < f.y0) std::swap(f.y0, f.y1);
        }
        cows.resize(c);
        for (Point& cow : cows) scanf("%d%d", &cow.x, &cow.y);
    }

    void DiscretizeXY() {
        std::vector<int> xs;
        std::vector<int> ys;
        for (Fence& f : fences) {
            xs.push_back(f.x0);
            xs.push_back(f.x1);
            ys.push_back(f.y0);
            ys.push_back(f.y1);
        }
        for (Point& cow : cows) {
            xs.push_back(cow.x);
            ys.push_back(cow.y);
        }
        std::vector<int> uniqueXs = UniqueSorted(xs);
        std::vector<int> uniqueYs = UniqueSorted(ys);
        // 把每个坐标替换成它的排名, 之后 Map 用排名定位.
        for (Fence& f : fences) {
            f.x0 = (int)(std::lower_bound(uniqueXs.begin(), uniqueXs.end(), f.x0) - uniqueXs.begin());
            f.x1 = (int)(std::lower_bound(uniqueXs.begin(), uniqueXs.end(), f.x1) - uniqueXs.begin());
            f.y0 = (int)(std::lower_bound(uniqueYs.begin(), uniqueYs.end(), f.y0) - uniqueYs.begin());
            f.y1 = (int)(std::lower_bound(uniqueYs.begin(), uniqueYs.end(), f.y1) - uniqueYs.begin());
        }
        for (Point& cow : cows) {
            cow.x = (int)(std::lower_bound(uniqueXs.begin(), uniqueXs.end(), cow.x) - uniqueXs.begin());
            cow.y = (int)(std::lower_bound(uniqueYs.begin(), uniqueYs.end(), cow.y) - uniqueYs.begin());
        }
        xSize = 2 * (int)uniqueXs.size() + 3;
        ySize = 2 * (int)uniqueYs.size() + 3;
    }
};

// 把离散化后的坐标映射到放大后的网格坐标 (每个原坐标占一格, 周围留空).
int Map(int rank) { return 2 * rank + 2; }

// 网格: ' ' 空地, 'F' 栅栏/已访问, 'C' 牛.
struct Board {
    int xSize, ySize;
    std::vector<std::vector<char>> data;

    void Build(const Input& in) {
        xSize = in.xSize;
        ySize = in.ySize;
        data.assign(xSize, std::vector<char>(ySize, ' '));
        for (const Point& cow : in.cows) {
            data[Map(cow.x)][Map(cow.y)] = 'C';
        }
        auto paint = [&](int x0, int x1, int y0, int y1) {
            for (int x = x0; x <= x1; ++x) {
                for (int y = y0; y <= y1; ++y) data[x][y] = 'F';
            }
        };
        for (const Fence& f : in.fences) {
            paint(Map(f.x0), Map(f.x1), Map(f.y0), Map(f.y1));
        }
        // 四周封一圈栅栏, 保证外部区域是连通的封闭空间.
        paint(0, 0, 0, ySize - 1);
        paint(0, xSize - 1, 0, 0);
        paint(xSize - 1, xSize - 1, 0, ySize - 1);
        paint(0, xSize - 1, ySize - 1, ySize - 1);
    }

    // 从 (x,y) 出发的连通区域里有多少头牛 (栅栏挡住, 不会跨过).
    int Visit(int x, int y) {
        if (data[x][y] == 'F') return 0;
        int count = (data[x][y] == 'C') ? 1 : 0;
        data[x][y] = 'F';
        for (int i = 0; i < 4; ++i) {
            count += Visit(x + DX[i], y + DY[i]);
        }
        return count;
    }

    // 每个连通区域里的牛数取最大.
    int Compute() {
        int answer = 0;
        for (int x = 0; x < xSize; ++x) {
            for (int y = 0; y < ySize; ++y) {
                answer = std::max(answer, Visit(x, y));
            }
        }
        return answer;
    }
};

int main() {
    Input input;
    input.Read();
    input.DiscretizeXY();
    Board board;
    board.Build(input);
    printf("%d\n", board.Compute());
    return 0;
}