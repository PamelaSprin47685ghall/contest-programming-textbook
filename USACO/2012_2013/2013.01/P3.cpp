#include <cstdio>
#include <vector>

// 并查集节点: 记录父节点, 与父节点是否异色 (d), 以及集合大小.
struct Vertex {
    int parent = -1;
    int d = 0;  // 0=与父同色, 1=与父异色.
    int size = 1;
};

struct UnionFind {
    std::vector<Vertex> vertices;

    // 找到 no 的根, 并把 no 与根的关系写入 rootRelation (no 到根的路径异或和).
    int Find(int no, int& rootRelation) {
        if (vertices[no].parent == -1) {
            rootRelation = 0;
            return no;
        }
        int parentRelation = 0;
        int root = Find(vertices[no].parent, parentRelation);
        vertices[no].parent = root;
        vertices[no].d ^= parentRelation;
        rootRelation = vertices[no].d;
        return root;
    }

    // 合并 v0, v1, 要求它们满足关系 d (0=同色, 1=异色). 冲突返回 false.
    bool Union(int v0, int v1, int d) {
        int relation0 = 0, relation1 = 0;
        int root0 = Find(v0, relation0);
        int root1 = Find(v1, relation1);
        d ^= relation0 ^ relation1;
        if (root0 == root1) return (d == 0);
        // 小集合挂到大集合下, 保持树平衡.
        if (vertices[root1].size < vertices[root0].size) {
            std::swap(root0, root1);
        }
        // d 是两棵树的根之间的异色关系 (异或对称, 交换后无需取反).
        vertices[root0].parent = root1;
        vertices[root0].d = d;
        vertices[root1].size += vertices[root0].size;
        return true;
    }
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    UnionFind uf;
    uf.vertices.resize(n);
    for (int i = 0; i < m; ++i) {
        int v0, v1;
        char c;
        scanf("%d%d %c", &v0, &v1, &c);
        // "x y T": x 说 y 说真话, 两者真假状态相同;
        // "x y L": x 说 y 说谎, 两者真假状态相反.
        bool same = (c == 'T');
        if (!uf.Union(v0 - 1, v1 - 1, same ? 0 : 1)) {
            printf("%d\n", i);
            return 0;
        }
    }
    printf("%d\n", m);
    return 0;
}