#include <cstdio>

using namespace std;

const int MAXN = 30005;
const int MAXST = 210;    // 6 个元素的划分个数 Bell(6) = 203
const int MAXK = 6;
const long long BIG = 1000000000000000000LL;  // 表示"不可达"的代价
const long long MOD = 1000000007;

int N, K;
int hcost[MAXN][MAXK + 1];  // hcost[i][j]：格子 (i,j) 与 (i,j+1) 之间的门
int vcost[MAXK + 1][MAXN];  // vcost[j][i]：格子 (i,j) 与 (i+1,j) 之间的门

// 状态是"前沿"上若干槽位的连通性划分：同一连通块标同一个号，标号按首次出现顺序
// 规范化成 0,1,2,...，再按 7 进制编码。K ≤ 6，槽位最多 6 个，编码小于 7^6。
int pow7[MAXK + 2];
int stateCount[MAXK + 1];
int stateCode[MAXK + 1][MAXST];
int stateId[MAXK + 1][120005];

// 第一行的增长转移：层 k-1 -> 层 k，第二维表示"左门"选不选
int r1To[MAXK + 1][MAXST][2];
// 第 2 行起的转移：层 K -> 层 K，后两维表示"上方的门/左侧的门"选不选
int inTo[MAXK + 1][MAXST][2][2];
// 收尾转移：层 lev -> 层 lev-1，退休的是第 0 个槽位
int clTo[MAXK + 1][MAXST];

void genStates(int level, int pos, int maxUsed, int code) {
    if (pos == level) {
        int id = stateCount[level]++;
        stateCode[level][id] = code;
        stateId[level][code] = id;
        return;
    }
    for (int lab = 0; lab <= maxUsed + 1; ++lab) {
        genStates(level, pos + 1, lab > maxUsed ? lab : maxUsed, code + lab * pow7[pos]);
    }
}

void decode(int code, int len, int* labels) {
    for (int i = 0; i < len; ++i) {
        labels[i] = code % 7;
        code /= 7;
    }
}

// 把标号数组重新按首次出现顺序编号并查表；label 里允许出现新鲜标号 K。
int canonicalId(int len, int* label) {
    int remap[8];
    for (int i = 0; i < 8; ++i) remap[i] = -1;
    int next = 0;
    int code = 0;
    for (int i = 0; i < len; ++i) {
        int lab = label[i];
        if (remap[lab] < 0) remap[lab] = next++;
        code += remap[lab] * pow7[i];
    }
    return stateId[len][code];
}

void relax(long long& cost, long long& way, long long newCost, long long newWay) {
    if (newCost < cost) {
        cost = newCost;
        way = newWay;
    } else if (newCost == cost) {
        way = (way + newWay) % MOD;
    }
}

int main() {
    scanf("%d%d", &N, &K);
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= K - 1; ++j) scanf("%d", &hcost[i][j]);
    }
    for (int j = 1; j <= K; ++j) {
        for (int i = 1; i <= N - 1; ++i) scanf("%d", &vcost[j][i]);
    }

    pow7[0] = 1;
    for (int i = 1; i <= K + 1; ++i) pow7[i] = pow7[i - 1] * 7;
    for (int lev = 0; lev <= K; ++lev) {
        stateCount[lev] = 0;
        for (int c = 0; c < pow7[K]; ++c) stateId[lev][c] = -1;
        genStates(lev, 0, -1, 0);
    }

    // 第一行：前沿从空逐步长成 (1,1..k)，新格子只可能通过左门与前一个格子相连
    for (int k = 1; k <= K; ++k) {
        for (int s = 0; s < stateCount[k - 1]; ++s) {
            int labels[8];
            decode(stateCode[k - 1][s], k - 1, labels);
            r1To[k][s][0] = -1;
            r1To[k][s][1] = -1;
            for (int lb = 0; lb <= 1; ++lb) {
                if (lb && k == 1) continue;  // 第一列没有左邻居
                int nl[8];
                for (int i = 0; i < k - 1; ++i) nl[i] = labels[i];
                nl[k - 1] = (lb ? labels[k - 2] : K);  // K 当新鲜标号
                r1To[k][s][lb] = canonicalId(k, nl);
            }
        }
    }

    // 第 2 行起：处理格子 (i,k) 时决定它到上方 (i-1,k) 与左侧 (i,k-1) 的两条门，
    // 随后槽位 k-1 上的结点 (i-1,k) 的所有边都已定，退出前沿。
    for (int k = 1; k <= K; ++k) {
        for (int s = 0; s < stateCount[K]; ++s) {
            int labels[8];
            decode(stateCode[K][s], K, labels);
            for (int ub = 0; ub <= 1; ++ub) {
                for (int lb = 0; lb <= 1; ++lb) {
                    inTo[k][s][ub][lb] = -1;
                    if (lb && k == 1) continue;  // 第一列没有左邻居
                    int w[8];
                    for (int i = 0; i < K; ++i) w[i] = labels[i];
                    int nodeLab = K;  // 新格子的标号
                    if (ub) nodeLab = w[k - 1];
                    if (lb) {
                        int other = w[k - 2];
                        if (nodeLab == K) {
                            nodeLab = other;
                        } else if (other != nodeLab) {
                            for (int i = 0; i < K; ++i) {
                                if (w[i] == other) w[i] = nodeLab;
                            }
                        }
                    }
                    // 退休检查：若 (i-1,k) 所在块里再没有别的活跃结点，这个块
                    // 往后无从再连，直接判非法（新格子与它同块也算活跃）。
                    int retired = w[k - 1];
                    bool ok = (nodeLab == retired);
                    if (!ok) {
                        for (int i = 0; i < K; ++i) {
                            if (i != k - 1 && w[i] == retired) {
                                ok = true;
                                break;
                            }
                        }
                    }
                    if (!ok) continue;
                    int nl[8];
                    for (int i = 0; i < K; ++i) nl[i] = w[i];
                    nl[k - 1] = nodeLab;
                    inTo[k][s][ub][lb] = canonicalId(K, nl);
                }
            }
        }
    }

    // 收尾：第 N 行各格已没有未定的边，把第 0 个槽位逐个撤出前沿；
    // 只有撤到最后一个结点时才允许它的块是唯一一块。
    for (int lev = 1; lev <= K; ++lev) {
        for (int s = 0; s < stateCount[lev]; ++s) {
            int labels[8];
            decode(stateCode[lev][s], lev, labels);
            clTo[lev][s] = -1;
            if (lev >= 2) {
                bool ok = false;
                for (int i = 1; i < lev; ++i) {
                    if (labels[i] == labels[0]) {
                        ok = true;
                        break;
                    }
                }
                if (!ok) continue;
            }
            int nl[8];
            for (int i = 1; i < lev; ++i) nl[i - 1] = labels[i];
            clTo[lev][s] = canonicalId(lev - 1, nl);
        }
    }

    static long long dpCost[MAXK + 1][MAXST];
    static long long dpWay[MAXK + 1][MAXST];
    for (int lev = 0; lev <= K; ++lev) {
        for (int s = 0; s < stateCount[lev]; ++s) {
            dpCost[lev][s] = BIG;
            dpWay[lev][s] = 0;
        }
    }
    dpCost[0][0] = 0;
    dpWay[0][0] = 1;

    // 第一行
    for (int k = 1; k <= K; ++k) {
        for (int s = 0; s < stateCount[k - 1]; ++s) {
            long long c = dpCost[k - 1][s];
            if (c >= BIG) continue;
            long long w = dpWay[k - 1][s];
            for (int lb = 0; lb <= 1; ++lb) {
                int t = r1To[k][s][lb];
                if (t < 0) continue;
                relax(dpCost[k][t], dpWay[k][t], c + (lb ? hcost[1][k - 1] : 0), w);
            }
        }
    }

    // 第 2 到 N 行
    static long long curCost[MAXST], curWay[MAXST], nxtCost[MAXST], nxtWay[MAXST];
    for (int s = 0; s < stateCount[K]; ++s) {
        curCost[s] = dpCost[K][s];
        curWay[s] = dpWay[K][s];
    }
    for (int i = 2; i <= N; ++i) {
        for (int k = 1; k <= K; ++k) {
            for (int s = 0; s < stateCount[K]; ++s) {
                nxtCost[s] = BIG;
                nxtWay[s] = 0;
            }
            int costUp = vcost[k][i - 1];
            int costLeft = (k >= 2) ? hcost[i][k - 1] : 0;
            for (int s = 0; s < stateCount[K]; ++s) {
                long long c = curCost[s];
                if (c >= BIG) continue;
                long long w = curWay[s];
                for (int ub = 0; ub <= 1; ++ub) {
                    for (int lb = 0; lb <= 1; ++lb) {
                        if (lb && k == 1) continue;
                        int t = inTo[k][s][ub][lb];
                        if (t < 0) continue;
                        relax(nxtCost[t], nxtWay[t], c + (ub ? costUp : 0) + (lb ? costLeft : 0), w);
                    }
                }
            }
            for (int s = 0; s < stateCount[K]; ++s) {
                curCost[s] = nxtCost[s];
                curWay[s] = nxtWay[s];
            }
        }
    }
    for (int s = 0; s < stateCount[K]; ++s) {
        dpCost[K][s] = curCost[s];
        dpWay[K][s] = curWay[s];
    }

    // 收尾
    for (int lev = K; lev >= 1; --lev) {
        for (int s = 0; s < stateCount[lev - 1]; ++s) {
            dpCost[lev - 1][s] = BIG;
            dpWay[lev - 1][s] = 0;
        }
        for (int s = 0; s < stateCount[lev]; ++s) {
            long long c = dpCost[lev][s];
            if (c >= BIG) continue;
            int t = clTo[lev][s];
            if (t < 0) continue;
            relax(dpCost[lev - 1][t], dpWay[lev - 1][t], c, dpWay[lev][s]);
        }
    }

    printf("%lld\n", dpWay[0][0] % MOD);
    return 0;
}
