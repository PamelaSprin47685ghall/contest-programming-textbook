#include <cstdio>
#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <vector>

using namespace std;

const int MOD = 1234567;
const int MAXDIGIT = 10;
const int MAXN = 20001;
const int SEG = 1 << 16;

int addMod(int x, int y) {
    x += y;
    if (x >= MOD) x -= MOD;
    return x;
}

int subMod(int x, int y) {
    return addMod(x, MOD - y);
}

int mulMod(int x, int y) {
    return (int)((1LL * x * y) % MOD);
}

int parent[MAXN];
set<int> partPath[MAXN];
set<int> fullPath[MAXN];
vector<pair<int, int> > childPattern[MAXN];
map<int, int> treeDp[MAXN];

int segmentTree[SEG];

void refresh(int position) {
    for (int i = position >> 1; i > 0; i >>= 1) {
        segmentTree[i] = mulMod(segmentTree[i << 1], segmentTree[(i << 1) + 1]);
    }
}

int assignedDp[1 << 16];
int assignedReady[1 << 16];
int assignedStart[1 << 16];
vector<int> assignedChild[1 << 16];

// 线段树存的是每个孩子在“当前后缀”下的方案数之积。
// 先把恰好匹配这个后缀的孩子换成它自己的方案数，再递归更长的后缀，最后改回去。
void solveAssigned(int node, int state) {
    vector<int> previous;
    int index = assignedStart[state];
    for (; index < (int)childPattern[node].size() && childPattern[node][index].first == state; ++index) {
        int child = childPattern[node][index].second;
        int position = (SEG / 2) + child;
        previous.push_back(segmentTree[position]);
        segmentTree[position] = treeDp[child][state];
        refresh(position);
    }
    assignedDp[state] = segmentTree[1];
    for (int i = 0; i < (int)assignedChild[state].size(); ++i) {
        solveAssigned(node, assignedChild[state][i]);
    }
    index = assignedStart[state];
    for (int j = 0; index < (int)childPattern[node].size() && childPattern[node][index].first == state; ++index, ++j) {
        int position = (SEG / 2) + childPattern[node][index].second;
        segmentTree[position] = previous[j];
        refresh(position);
    }
}

int main() {
    int nodeCount, patternCount;
    scanf("%d %d", &nodeCount, &patternCount);

    parent[0] = -1;
    partPath[0].insert(0);
    for (int node = 1; node < nodeCount; ++node) {
        scanf("%d", &parent[node]);
        partPath[node].insert(0);
        childPattern[parent[node]].push_back(make_pair(0, node));
    }

    for (int i = 0; i < patternCount; ++i) {
        int start;
        char text[8];
        scanf("%d %s", &start, text);
        int state = 0;
        int length = 0;
        while (text[length] != '\0') {
            state = (state << 4) | (text[length] - '0' + 1);
            ++length;
        }
        fullPath[start].insert(state);
        // 先记在模式起点，再沿父亲往上走。题面保证再往上至少还有 4 步，不会走到根的父亲。
        int node = start;
        for (int kept = length - 1; kept > 0; --kept, node = parent[node]) {
            int suffix = state & ((1 << (4 * kept)) - 1);
            partPath[node].insert(suffix);
            childPattern[parent[node]].push_back(make_pair(suffix, node));
        }
    }

    for (int node = 0; node < nodeCount; ++node) {
        sort(childPattern[node].begin(), childPattern[node].end());
        childPattern[node].erase(unique(childPattern[node].begin(), childPattern[node].end()), childPattern[node].end());
    }

    memset(assignedReady, -1, sizeof(assignedReady));
    for (int i = 0; i < SEG; ++i) segmentTree[i] = 1;

    for (int node = nodeCount - 1; node >= 0; --node) {
        // 只给这个节点真正出现过的后缀建状态，避免清空整张 6 万项的表。
        assignedReady[0] = node;
        assignedChild[0].clear();
        assignedStart[0] = 0;
        for (int i = 0; i < (int)childPattern[node].size(); ++i) {
            int state = childPattern[node][i].first;
            if (state == 0 || (i > 0 && state == childPattern[node][i - 1].first)) continue;
            assignedReady[state] = node;
            assignedChild[state].clear();
            assignedStart[state] = i;
            while (assignedReady[state >> 4] != node) {
                assignedReady[state >> 4] = node;
                assignedChild[state >> 4].clear();
                assignedChild[state >> 4].push_back(state);
                assignedStart[state >> 4] = i;
                state >>= 4;
            }
            assignedChild[state >> 4].push_back(state);
        }
        solveAssigned(node, 0);

        for (set<int>::iterator it = partPath[node].begin(); it != partPath[node].end(); ++it) {
            int state = *it;
            while (state != 0 && state < (1 << 12)) state <<= 4;
            int result = 0;
            for (int digit = 1; digit <= MAXDIGIT; ++digit) {
                int extended = (digit << 16) | state;
                if (fullPath[node].find(extended) != fullPath[node].end()) continue;
                int reduced = extended >> 4;
                while (assignedReady[reduced] != node) reduced >>= 4;
                result = addMod(result, assignedDp[reduced]);
            }
            treeDp[node][*it] = result;
        }
    }

    int allWays = 1;
    for (int i = 0; i < nodeCount; ++i) allWays = mulMod(allWays, MAXDIGIT);
    printf("%d\n", subMod(allWays, treeDp[0][0]));
    return 0;
}
