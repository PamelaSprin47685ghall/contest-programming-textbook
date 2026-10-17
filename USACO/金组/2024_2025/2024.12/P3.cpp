#include <cstdio>
#include <algorithm>
#include <queue>

using namespace std;

const int MAXN = 200005;

struct Job {
    long long start;    // 最晚开始时间 s
    long long length;   // 需要的时间 t
    long long deadline; // s + t，按它从小到大处理
};

int N;
Job jobs[MAXN];

bool cmpJob(const Job &a, const Job &b) {
    return a.deadline < b.deadline;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &N);
        for (int i = 0; i < N; ++i) {
            scanf("%lld %lld", &jobs[i].start, &jobs[i].length);
            jobs[i].deadline = jobs[i].start + jobs[i].length;
        }
        // 若所有工作都能做完，一定存在按 s+t 递增的顺序（交换相邻两项即可），
        // 所以贪心也按这个顺序考虑工作。
        sort(jobs, jobs + N, cmpJob);

        // 大根堆：保存当前选中的工作各自需要的时间，堆顶是最长的那件。
        priority_queue<long long> chosen;
        long long totalTime = 0;
        for (int i = 0; i < N; ++i) {
            chosen.push(jobs[i].length);
            totalTime += jobs[i].length;
            // 选中的这些工作按 d 递增的顺序连续做完，总时长已经超过第 i 件的 d，
            // 说明它们不可能全部做完，必须去掉一件；去掉最长的那件最省时间，
            // 而且"当前集合里删掉一件后仍然可行"这件事只需要在这个时刻检查。
            if (totalTime > jobs[i].deadline) {
                totalTime -= chosen.top();
                chosen.pop();
            }
            // 去掉一件之后必然满足 totalTime <= deadline：加入前 totalTime <= 上一件的 deadline <= 当前 deadline，
            // 而堆顶不小于刚加入的 length，所以只会减得更多，不需要反复删。
        }
        printf("%d\n", (int)chosen.size());
    }
    return 0;
}
