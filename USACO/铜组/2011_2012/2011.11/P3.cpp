#include <algorithm>
#include <cstdio>
#include <vector>

// 把一段旋律排序并整体平移, 使最小值变成 0, 得到它的"规范化"形态.
// 这样两个音程是否相同就等价于它们的规范化形态是否相等.
void Normalize(std::vector<int>& notes) {
    std::sort(notes.begin(), notes.end());
    int minimum = notes[0];
    for (int& note : notes) note -= minimum;
}

int main() {
    int musicSize;
    scanf("%d", &musicSize);
    std::vector<int> music(musicSize);
    for (int& note : music) scanf("%d", &note);

    int chordSize;
    scanf("%d", &chordSize);
    std::vector<int> chord(chordSize);
    for (int& note : chord) scanf("%d", &note);
    Normalize(chord);

    // 枚举音乐里每一段长度与和弦相同的连续片段, 规范化后与和弦比较.
    std::vector<int> answer;
    for (int begin = 0; begin + chordSize <= musicSize; ++begin) {
        std::vector<int> sub(music.begin() + begin, music.begin() + begin + chordSize);
        Normalize(sub);
        if (sub == chord) answer.push_back(begin + 1);
    }

    printf("%d\n", (int)answer.size());
    for (int position : answer) printf("%d\n", position);
    return 0;
}