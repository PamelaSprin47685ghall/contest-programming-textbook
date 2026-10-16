#include <cstdio>
#include <string>
#include <unordered_set>
#include <vector>

// 每个转盘数字允许的取值是 [v-2, v+2] 共 5 个, 两把锁各 3 位,
// 直接枚举所有组合, 用集合去掉重复, 集合大小就是答案.
struct Combination {
    int a, b, c;
};

bool operator==(const Combination& x, const Combination& y) {
    return x.a == y.a && x.b == y.b && x.c == y.c;
}

struct CombinationHash {
    size_t operator()(const Combination& x) const {
        return size_t(x.a) * 1000003 + size_t(x.b) * 997 + size_t(x.c);
    }
};

struct Lock {
    int a, b, c;
};

// 读入一个锁的 3 个数字, 并转成 0 基下标, 方便取模.
Lock ReadLock() {
    Lock lock;
    scanf("%d%d%d", &lock.a, &lock.b, &lock.c);
    --lock.a;
    --lock.b;
    --lock.c;
    return lock;
}

int main() {
    int n;
    scanf("%d", &n);

    std::unordered_set<Combination, CombinationHash> seen;
    for (int i = 0; i < 2; ++i) {
        Lock lock = ReadLock();
        // 枚举 3 个位置各自的 5 种取值, 组合成一个候选开锁组合.
        for (int da = -2; da <= 2; ++da) {
            for (int db = -2; db <= 2; ++db) {
                for (int dc = -2; dc <= 2; ++dc) {
                    Combination candidate;
                    candidate.a = (lock.a + da + n) % n;
                    candidate.b = (lock.b + db + n) % n;
                    candidate.c = (lock.c + dc + n) % n;
                    seen.insert(candidate);
                }
            }
        }
    }

    printf("%d\n", int(seen.size()));
    return 0;
}
