#include <cstdio>
#include <limits>
#include <vector>

// 对每一种牛奶，假设它是被污染的那一份，检查是否所有生病的人
// 都在喝过它之后才生病；若是，则喝过它的总人数就是一种可能的答案。
struct Record {
    int Person; // 喝奶的人
    int Milk;   // 喝的牛奶编号
    int Time;   // 喝奶时间
};

struct BadMilk {
    int N; // 人数
    int M; // 牛奶种类数
    int D; // 喝奶记录条数
    int S; // 生病人数
    std::vector<Record> records;
    std::vector<int> sickTime; // 每个人生病的时间，未生病为无穷大
};

void read(BadMilk& b) {
    scanf("%d%d%d%d", &b.N, &b.M, &b.D, &b.S);
    b.records.resize(b.D);
    for (Record& r : b.records) {
        scanf("%d%d%d", &r.Person, &r.Milk, &r.Time);
    }
    b.sickTime.assign(b.N + 1, std::numeric_limits<int>::max());
    for (int i = 0; i < b.S; ++i) {
        int p, t;
        scanf("%d%d", &p, &t);
        b.sickTime[p] = t;
    }
}

// 假设第 no 种牛奶被污染时，最多有多少人需要治疗；若矛盾返回 0。
int needCure(const BadMilk& b, int no) {
    // poisonTime 记录每个人喝第 no 种牛奶的最早时间；
    // 用 max-1 表示"没喝过"（区别于真正的无穷大生病时间）。
    std::vector<int> poisonTime(b.N + 1, std::numeric_limits<int>::max() - 1);
    for (const Record& r : b.records) {
        if (r.Milk == no) {
            poisonTime[r.Person] = std::min(poisonTime[r.Person], r.Time);
        }
    }
    int answer = 0;
    for (int i = 1; i <= b.N; ++i) {
        // 生病时间不晚于喝奶时间，说明这杯奶不可能是病因；
        // 生病却没喝过这杯奶的人也会触发这个矛盾。
        if (b.sickTime[i] <= poisonTime[i]) return 0;
        if (poisonTime[i] != std::numeric_limits<int>::max() - 1) {
            ++answer; // 喝过这杯奶的人需要治疗
        }
    }
    return answer;
}

int main() {
    BadMilk b;
    read(b);
    int answer = 0;
    for (int no = 1; no <= b.M; ++no) {
        answer = std::max(answer, needCure(b, no));
    }
    printf("%d\n", answer);
    return 0;
}