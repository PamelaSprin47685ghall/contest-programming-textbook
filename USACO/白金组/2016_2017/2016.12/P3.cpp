#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000;  // 位置数上限
const int MAXMODEL = 10;  // 每个位置的型号数上限

vector<long long> upgrade[MAXN]; // 每个位置相对最便宜型号的额外花费，去掉最便宜的 0 那一档
long long stepFirst[MAXN];       // 按"第一次升级花费"排好序后，每个位置最便宜的一次升级
long long prefixWorst[MAXN];     // 前 idx+1 个位置都换到最贵型号时，额外花费之和
long long prefixWays[MAXN];      // 前 idx+1 个位置的型号组合数，超过 wanted 时按 wanted 截断
int upgradeable;                 // 可选型号多于一种的位置数
int wanted;                      // 要造的机器人数量
long long counted;               // 计数时已经数到的机器人个数，达到 wanted 就提前收手
long long savingSum;             // 枚举便宜机器人时累计的 (阈值 - 实际额外花费)

// 数一数额外花费不超过 budget 的机器人有多少个，最多数到 wanted 个
void countRobots(int idx, long long budget) {
    if (counted >= wanted) return;
    if (idx >= 0 && budget >= prefixWorst[idx]) {
        // 剩下的位置全部换到最贵也花不完，组合数直接一次加完
        counted += prefixWays[idx];
        return;
    }
    if (idx >= 0 && budget < upgrade[idx][0]) {
        // 连这个位置最便宜的一次升级都买不起：往前跳到还买得起的最靠后位置，
        // 中间这些位置只能保持最便宜型号，不值得一层层递归
        idx = upper_bound(stepFirst, stepFirst + idx, budget) - stepFirst - 1;
    }
    if (idx < 0) {
        counted++; // 每个位置都定下了型号，得到一个机器人
        return;
    }
    countRobots(idx - 1, budget); // 这个位置保持最便宜型号
    for (int i = 0; i < (int)upgrade[idx].size() && upgrade[idx][i] <= budget; ++i) {
        countRobots(idx - 1, budget - upgrade[idx][i]); // 换成第 i+1 贵的型号
    }
}

// 把额外花费不超过 budget 的机器人逐个走一遍，累加 (阈值 - 实际额外花费)；
// 这些机器人就是 k 个最便宜机器人里额外花费严格小于阈值的那一批，总数少于 k
void enumerateRobots(int idx, long long budget) {
    if (idx >= 0 && budget < upgrade[idx][0]) {
        idx = upper_bound(stepFirst, stepFirst + idx, budget) - stepFirst - 1;
    }
    if (idx < 0) {
        savingSum += budget + 1;
        return;
    }
    enumerateRobots(idx - 1, budget);
    for (int i = 0; i < (int)upgrade[idx].size() && upgrade[idx][i] <= budget; ++i) {
        enumerateRobots(idx - 1, budget - upgrade[idx][i]);
    }
}

int main() {
    int n;
    scanf("%d%d", &n, &wanted);

    long long baseCost = 0;
    vector<long long> modelCost; // 每个位置的型号价格；反复清空复用，避免重复分配
    for (int i = 0; i < n; ++i) {
        int modelCount;
        scanf("%d", &modelCount);
        modelCost.clear();
        for (int j = 0; j < modelCount; ++j) {
            long long price;
            scanf("%lld", &price);
            modelCost.push_back(price);
        }
        sort(modelCost.begin(), modelCost.end());
        baseCost += modelCost[0];
        if (modelCount > 1) {
            // 把每个位置化成"在最低价基础上加多少钱"，最便宜的 0 不存
            for (int j = 1; j < modelCount; ++j) {
                upgrade[upgradeable].push_back(modelCost[j] - modelCost[0]);
            }
            upgradeable++;
        }
    }

    // 按第一次升级花费排序：这样"买不起就整段跳过"时能二分定位
    sort(upgrade, upgrade + upgradeable);

    long long worstExtra = 0;
    long long waysProduct = 1;
    for (int i = 0; i < upgradeable; ++i) {
        stepFirst[i] = upgrade[i][0];
        worstExtra += upgrade[i].back();
        prefixWorst[i] = worstExtra;
        waysProduct *= (long long)upgrade[i].size() + 1;
        if (waysProduct > wanted) waysProduct = wanted;
        prefixWays[i] = waysProduct;
    }

    // 二分出第 wanted 便宜的机器人额外花费：它是"数够 wanted 个"所需的最小阈值
    long long low = 0, high = worstExtra;
    while (low < high) {
        long long mid = (low + high) / 2;
        counted = 0;
        countRobots(upgradeable - 1, mid);
        if (counted < wanted) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    // 额外花费严格小于 low 的机器人全是最便宜的若干个，
    // 剩下的名额每个都恰好花 low，于是总和可以整批算出来
    if (low > 0) {
        enumerateRobots(upgradeable - 1, low - 1);
    }
    printf("%lld\n", (baseCost + low) * wanted - savingSum);
    return 0;
}
