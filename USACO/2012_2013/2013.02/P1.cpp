#include <cstdio>
#include <vector>

const int NotVisited = 0;
const int Visiting = 1;
const int NoLoop = 2;  // 该点出发的消息最终会停止.
const int Loop = 3;    // 该点出发的消息会陷入循环.

struct Cows {
    std::vector<int> forward;  // 每头牛把消息传给谁 (0 基), -1 表示传给农夫 (停止).

    void Read() {
        int n;
        scanf("%d", &n);
        forward.resize(n);
        for (int i = 0; i < n; ++i) {
            scanf("%d", &forward[i]);
            --forward[i];
        }
    }
};

// 判断从 no 出发的消息是否陷入循环, 同时给所有途经点打上状态.
struct DFS {
    const Cows* cows;
    std::vector<int> status;

    bool VisitLoop(int no) {
        if (cows->forward[no] == -1) status[no] = NoLoop;
        if (status[no] == NoLoop) return false;
        if (status[no] == Visiting || status[no] == Loop) return true;
        status[no] = Visiting;
        bool hasLoop = VisitLoop(cows->forward[no]);
        status[no] = hasLoop ? Loop : NoLoop;
        return hasLoop;
    }
};

int main() {
    Cows cows;
    cows.Read();
    DFS dfs;
    dfs.cows = &cows;
    dfs.status.resize(cows.forward.size());
    int answer = 0;
    // 能停止的牛就是能收到消息的牛.
    for (int i = 0; i < (int)cows.forward.size(); ++i) {
        if (!dfs.VisitLoop(i)) ++answer;
    }
    printf("%d\n", answer);
    return 0;
}