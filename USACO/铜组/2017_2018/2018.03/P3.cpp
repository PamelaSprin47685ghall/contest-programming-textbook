#include <cstdio>
#include <string>
#include <unordered_set>
#include <vector>

char Buffer[80];

// 把牛名映射成整数编号, 用哈希集合去重.
struct Strings {
    std::vector<std::string> Data;
    std::unordered_set<std::string> Index;

    int GetIndex(const std::string& name) {
        auto result = Index.insert(name);
        if (result.second) Data.push_back(name);
        // 找到该名字在 Data 中的位置.
        for (int i = 0; i < (int)Data.size(); ++i) {
            if (Data[i] == name) return i;
        }
        return -1;
    }

    int Read() {
        scanf("%s", Buffer);
        return GetIndex(Buffer);
    }
};

// 家族树, 用"每个节点指向其母亲"的边存储.
struct FamilyTree {
    std::vector<int> Parent;  // Parent[child] = mother 编号, -1 表示无母亲.
    std::vector<int> Depth;   // 到根(祖先)的距离, -1 表示未算.

    void SetParent(int child, int parent) {
        if ((int)Parent.size() <= child) Parent.resize(child + 1, -1);
        if ((int)Parent.size() <= parent) Parent.resize(parent + 1, -1);
        Parent[child] = parent;
    }

    int ComputeDepth(int no) {
        if (no == -1) return -1;
        if (Depth[no] != -1) return Depth[no];
        // 母亲比女儿深一层, 递归计算.
        return Depth[no] = ComputeDepth(Parent[no]) + 1;
    }

    // 朴素 LCA: 先让较深的牛向上走到同一层, 再一起向上走到相遇.
    // n0, n1 分别记下两头牛向上走的步数.
    int ComputeLCA(int c0, int c1, int* n0, int* n1) {
        auto depthLess = [&](int x, int y) {
            return ComputeDepth(x) < ComputeDepth(y);
        };
        // 保证 c0 不比 c1 浅, 这样先抬 c1.
        if (depthLess(c1, c0)) {
            std::swap(c0, c1);
            std::swap(n0, n1);
        }
        while (depthLess(c0, c1)) {
            ++*n1;
            c1 = Parent[c1];
        }
        while (c0 != c1) {
            ++*n0;
            c0 = Parent[c0];
            ++*n1;
            c1 = Parent[c1];
        }
        return c0;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    Strings names;
    int cow0 = names.Read(), cow1 = names.Read();
    FamilyTree tree;
    for (int i = 0; i < n; ++i) {
        int parent = names.Read(), child = names.Read();
        tree.SetParent(child, parent);
    }
    tree.Depth.assign(tree.Parent.size(), -1);
    // n0, n1 分别是 cow0, cow1 到 LCA 的代数差.
    int n0 = 0, n1 = 0;
    if (tree.ComputeLCA(cow0, cow1, &n0, &n1) == -1) {
        printf("NOT RELATED\n");
    } else if (n0 == 1 && n1 == 1) {
        printf("SIBLINGS\n");
    } else if (1 < n0 && 1 < n1) {
        printf("COUSINS\n");
    } else {
        // n0 和 n1 中较小者是"长辈"那一侧.
        if (n1 < n0) {
            std::swap(n0, n1);
            std::swap(cow0, cow1);
        }
        printf("%s is the ", names.Data[cow0].c_str());
        for (int i = 0; i < n1 - 2; ++i) printf("great-");
        if (n0 == 0) {
            // 长辈是直系祖先.
            if (1 < n1) printf("grand-");
            printf("mother");
        } else {
            printf("aunt");
        }
        printf(" of %s\n", names.Data[cow1].c_str());
    }
    return 0;
}