#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int MAXN = 100005;

int n;
int nameLen[MAXN];         // 每个文件或目录名字的长度
int isFile[MAXN];          // 标记该实体是文件还是目录
vector<int> children[MAXN];
int parent[MAXN];          // 每个实体的父亲, 用来把子树文件数向上累加
int order[MAXN];           // 广度优先遍历顺序: 父亲一定排在儿子前面
int fileCount[MAXN];       // 每个目录的子树里有多少个文件
long long prefixLen[MAXN]; // 从顶层目录下降到某目录时, 路径前缀的长度(不含该目录自己的名字)
long long sumLen[MAXN];    // 站在某目录里时, 到所有文件的相对路径长度之和
int totalFiles;

int main() {
    scanf("%d", &n);
    char name[64];
    for (int i = 1; i <= n; i++) {
        int m;
        scanf("%s %d", name, &m);
        nameLen[i] = strlen(name);
        isFile[i] = (m == 0);
        for (int k = 0; k < m; k++) {
            int id;
            scanf("%d", &id);
            children[i].push_back(id);
            parent[id] = i;
        }
    }

    // 迭代式广度优先遍历, 避免目录串成一条长链时递归爆栈
    order[0] = 1;
    int cnt = 1;
    for (int head = 0; head < cnt; head++) {
        int u = order[head];
        for (size_t k = 0; k < children[u].size(); k++) {
            order[cnt++] = children[u][k];
        }
    }

    // 逆序遍历: 儿子的文件数先算完, 再累加到父亲上
    for (int i = cnt - 1; i >= 0; i--) {
        int u = order[i];
        if (isFile[u]) {
            fileCount[u] = 1;
            totalFiles++;
        }
        // 目录的 fileCount 已经在处理它的儿子时累加好了, 这里统一向上传递
        fileCount[parent[u]] += fileCount[u];
    }

    // 正序遍历求前缀长度, 并统计站在顶层目录时的路径总长:
    // 走到文件儿子的相对路径就是"前缀 + 文件名"
    for (int i = 0; i < cnt; i++) {
        int u = order[i];
        for (size_t k = 0; k < children[u].size(); k++) {
            int c = children[u][k];
            if (isFile[c]) {
                sumLen[1] += prefixLen[u] + nameLen[c];
            } else {
                // 进入目录 c 要多写它的名字和一个斜杠
                prefixLen[c] = prefixLen[u] + nameLen[c] + 1;
            }
        }
    }

    // 换根: 从目录 u 走进它的目录儿子 c 时, c 子树内的文件路径都少了
    // "c 的名字 + 斜杠"这么长, 其余文件都多写了 "../" 三个字符
    for (int i = 0; i < cnt; i++) {
        int u = order[i];
        for (size_t k = 0; k < children[u].size(); k++) {
            int c = children[u][k];
            if (isFile[c]) continue;
            sumLen[c] = sumLen[u]
                      - (long long)fileCount[c] * (nameLen[c] + 1)
                      + (long long)(totalFiles - fileCount[c]) * 3;
        }
    }

    long long best = sumLen[1];
    for (int i = 0; i < cnt; i++) {
        int u = order[i];
        if (!isFile[u] && sumLen[u] < best) best = sumLen[u];
    }
    printf("%lld\n", best);
    return 0;
}
