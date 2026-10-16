#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

char Buffer[80];

struct TrieNode {
    std::unordered_map<std::string, int> children; // 形容词 -> 子节点下标
    int count = 0;
};

// Trie 记录所有"没有的牛"; 每个节点的 count 是经过该节点的短语数.
// 用 vector 存节点、下标代替指针, 避免手工管理内存.
struct Trie {
    std::vector<TrieNode> nodes;

    Trie() { nodes.resize(1); }

    void Add(const std::vector<std::string>& words) {
        int current = 0;
        ++nodes[current].count;
        for (const std::string& word : words) {
            current = Go(current, word);
            ++nodes[current].count;
        }
    }

    // 返回形容词 word 对应的子节点下标; 不存在则新建一个.
    int Go(int node, const std::string& word) {
        std::unordered_map<std::string, int>& children = nodes[node].children;
        std::unordered_map<std::string, int>::iterator it = children.find(word);
        if (it != children.end()) return it->second;
        int child = nodes.size();
        children[word] = child;
        nodes.push_back(TrieNode());
        return child;
    }

    // 该层形容词是 word 的"没有的牛"数量; 该形容词不存在则返回 0.
    int ChildCount(int node, const std::string& word) const {
        const std::unordered_map<std::string, int>& children =
            nodes[node].children;
        std::unordered_map<std::string, int>::const_iterator it =
            children.find(word);
        if (it == children.end()) return 0;
        return nodes[it->second].count;
    }
};

// 读入一句 "Farmer John has no ... cow.", 返回其中的形容词列表.
std::vector<std::string> ReadAdjectives() {
    scanf(" Farmer John has no");
    std::vector<std::string> words;
    while (true) {
        scanf("%s", Buffer);
        std::string word = Buffer;
        if (word == "cow.") return words;
        words.push_back(word);
    }
}

int main() {
    int n, k;
    scanf("%d%d", &n, &k);

    Trie trie;
    std::vector<std::vector<std::string>> byLevel;
    for (int i = 0; i < n; ++i) {
        std::vector<std::string> words = ReadAdjectives();
        trie.Add(words);
        if (byLevel.size() < words.size()) byLevel.resize(words.size());
        for (int level = 0; level < (int)words.size(); ++level) {
            byLevel[level].push_back(words[level]);
        }
    }

    // 每层去重排序, 得到该层可选的形容词集合.
    for (std::vector<std::string>& level : byLevel) {
        std::sort(level.begin(), level.end());
        level.erase(std::unique(level.begin(), level.end()), level.end());
    }

    // comboCount[level] = 该层之后(不含本层)的组合数.
    std::vector<int> comboCount(byLevel.size());
    comboCount.back() = 1;
    for (int level = byLevel.size() - 2; level >= 0; --level) {
        comboCount[level] = comboCount[level + 1] * byLevel[level + 1].size();
    }

    --k; // 转成 0 基下标
    // 第 k 头"有的牛": 逐层选形容词. 该层某形容词的组合总数减去
    // 被列入"没有"的组合, 剩下的都是 FJ 有的牛, 据此跳过.
    int current = 0;
    std::string answer;
    for (int level = 0; level < (int)byLevel.size(); ++level) {
        for (const std::string& word : byLevel[level]) {
            int available = comboCount[level] - trie.ChildCount(current, word);
            if (k < available) {
                answer += word;
                answer += ' ';
                current = trie.Go(current, word);
                break;
            } else {
                k -= available;
            }
        }
    }
    if (!answer.empty()) answer.pop_back();
    printf("%s\n", answer.c_str());
    return 0;
}