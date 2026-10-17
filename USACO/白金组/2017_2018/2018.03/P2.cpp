#include <algorithm>
#include <cstdio>

using namespace std;

// 本题是交互题：只实现 helpBessie，没有 main，也不允许任何非常量全局/静态变量，
// 两趟观察之间的一切状态都必须经过贝茜的笔记本（get/set）。下面全是编译期常量。
const int BLOCK = 1000;     // 每 1000 个窗口取一个"地标"，笔记本就只用存这么多个位置
const int DIF = 4050;       // 地标区起始格号：单调队列最长约 BLOCK 项，不会和它撞上
const int NO_VALUE = 1000000000; // 车号最大就是 10^9，用它当"还没见过元素"的哨兵

// 笔记本布局：
//   0、1 号格        ：单调队列的队首、队尾指针
//   2、3 号格起      ：队列元素按 (位置, 数值) 交替存放
//   DIF-1 号格       ：下午正在拼装的窗口块编号
//   DIF-2 号格       ：当前块里"落在所有剩余窗口内"的元素的最小值
//   DIF-3 号格       ：已经广播出去的窗口个数
//   DIF 号格起       ：上午记下的地标位置：第 t 格存 f(t*BLOCK)
void helpBessie(int ID) {
    int n = getTrainLength();
    int k = getWindowLength();
    int i = getCurrentCarIndex();

    if (getCurrentPassIndex() == 0) {
        // ===== 上午 =====
        // 目的：求出地标窗口 t*BLOCK 的最小值位置 f(t*BLOCK)，t 取遍所有倍数。
        // 以地标窗口的左端点 m*BLOCK 与右端点右边一格 m*BLOCK+K 为界把数组切段：
        // 一段内的元素要么全在某个地标窗口里，要么全不在，所以每段只需要留一个
        // "段内最小值"，单调队列里放段代表即可，队列长度被 BLOCK 卡住。
        if (i == 0) {
            set(0, 0); // 队列为空
            set(1, -1);
        }
        int top = get(1);
        int back = get(0);
        if (i % BLOCK == 0 || (i >= k && (i - k) % BLOCK == 0)) {
            // 当前位置是某段的第一节车厢：把这一段（已经结束）的代表元素推入队尾
            while (top >= back && get(2 * top + 3) >= ID) top--;
            top++;
            set(2 * top + 2, i);
            set(2 * top + 3, ID);
        } else {
            // 还在同一段里：段代表只需吸收比它小的值就能代表整段的最小值
            if (ID <= get(2 * top + 3)) {
                top--;
                while (top >= back && get(2 * top + 3) >= ID) top--;
                top++;
                set(2 * top + 2, i);
                set(2 * top + 3, ID);
            }
        }
        if (i >= k - 1 && (i + 1 - k) % BLOCK == 0) {
            // 一个地标窗口刚好走完：弹出队首窗口外的段，队首就是它的最小值位置
            while (top >= back && get(2 * back + 2) <= i - k) back++;
            set(DIF + (i + 1 - k) / BLOCK, get(2 * back + 2));
        }
        set(0, back);
        set(1, top);
        return;
    }

    // ===== 下午 =====
    // 块编号 block 负责窗口 [(block-1)*BLOCK, block*BLOCK)，已知道块内地标
    // f((block-1)*BLOCK)。以它为界把元素分成两类：
    //   位置离地标不超过 BLOCK 的：可能只落在本块的部分窗口里，位置必须留住；
    //   更靠后的：一定落在本块所有剩余窗口内（左端点最早是 (block-1)*BLOCK，
    //   这些元素的位置更大），只剩数值有意义，于是记一个"全局最小值"就够。
    int firstLandmark = get(DIF); // f(0)：比它靠前的车不可能是任何窗口的最小值
    if (i < firstLandmark) return;
    if (i == firstLandmark) {
        set(0, 0); // 队列清空，并把地标 f(0) 自己放进去
        set(1, -1);
        set(DIF - 1, 1);
        set(DIF - 2, NO_VALUE);
        set(DIF - 3, 0);
    }
    int block = get(DIF - 1);
    int top = get(1);
    int back = get(0);
    int outputs = get(DIF - 3);
    if (i - get(DIF + block - 1) <= BLOCK) {
        while (top >= back && get(2 * top + 3) >= ID) top--;
        top++;
        set(2 * top + 2, i);
        set(2 * top + 3, ID);
    } else {
        if (ID < get(DIF - 2)) set(DIF - 2, ID);
    }
    if (outputs + k - 1 == i) {
        // 窗口 outputs 的最后一节车厢刚过去：弹出窗口外的队首，它的最小值就定了
        while (top >= back && get(2 * back + 2) < outputs) back++;
        shoutMinimum(min(get(DIF - 2), get(2 * back + 3)));
        outputs++;
    }
    while (BLOCK * block + k - 1 < n && get(DIF + block) == i) {
        // 已经读到下一块的地标 f(block*BLOCK)：本块剩下的窗口的最小值不可能再来自
        // 还没读到的车，可以一次广播完，然后开始下一块
        while (outputs <= BLOCK * block) {
            while (top >= back && get(2 * back + 2) < outputs) back++;
            shoutMinimum(min(get(DIF - 2), get(2 * back + 3)));
            outputs++;
        }
        block++;
        set(DIF - 2, NO_VALUE); // 新块重新统计全局最小值
        top = 0;
        back = 0;
        set(2, i);
        set(3, ID);
    }
    set(DIF - 3, outputs);
    set(DIF - 1, block);
    set(0, back);
    set(1, top);
}
