#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

void solve() {
    int n, c, p;
    scanf("%d %d %d", &n, &c, &p);
    vector<string> nouns, tverbs, iverbs, conjs;
    char word[105];
    char kind[105];
    for (int i = 0; i < n; ++i) {
        scanf("%s %s", word, kind);
        if (kind[0] == 'n') nouns.push_back(word);
        else if (kind[0] == 't') tverbs.push_back(word);
        else if (kind[0] == 'i') iverbs.push_back(word);
        else conjs.push_back(word);
    }

    int bestWords = 0;
    int bestT = 0, bestI = 0, bestConj = 0, bestExtraN = 0;

    int numNouns = (int)nouns.size();
    int numT = (int)tverbs.size();
    int numI = (int)iverbs.size();
    int numConj = (int)conjs.size();

    for (int t = 0; t <= numT; ++t) {
        if (2 * t > numNouns) break;
        int remNouns = numNouns - 2 * t;
        int maxI = min(numI, remNouns);

        for (int i = 0; i <= maxI; ++i) {
            int totalSentences = t + i;
            if (totalSentences == 0) continue;

            int maxC = min(numConj, totalSentences / 2);
            int minC = max(0, totalSentences - p);
            if (minC > maxC) continue;

            int conjUsed = maxC;
            int leftNouns = remNouns - i;
            int extraN = 0;
            if (t > 0) extraN = min(leftNouns, c);

            int words = 3 * t + 2 * i + conjUsed + extraN;
            if (words > bestWords) {
                bestWords = words;
                bestT = t;
                bestI = i;
                bestConj = conjUsed;
                bestExtraN = extraN;
            }
        }
    }

    if (bestWords == 0) {
        printf("0\n\n");
        return;
    }

    vector<string> sentences;
    for (int i = 0; i < bestI; ++i) {
        string sentence = nouns.back() + " " + iverbs.back();
        nouns.pop_back();
        iverbs.pop_back();
        sentences.push_back(sentence);
    }
    for (int i = 0; i < bestT; ++i) {
        string sentence = nouns.back() + " " + tverbs.back();
        nouns.pop_back();
        tverbs.pop_back();
        sentence += " " + nouns.back();
        nouns.pop_back();
        if (i == 0) {
            for (int e = 0; e < bestExtraN; ++e) {
                sentence += ", " + nouns.back();
                nouns.pop_back();
            }
        }
        sentences.push_back(sentence);
    }

    string output = "";
    int conjIdx = 0;
    for (int i = 0; i < (int)sentences.size(); ++i) {
        if (i > 0) {
            if (conjIdx < bestConj && (i % 2 == 1)) {
                output += " " + conjs[conjIdx++] + " ";
            } else {
                output += ". ";
            }
        }
        output += sentences[i];
    }
    output += ".";

    printf("%d\n%s\n", bestWords, output.c_str());
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) solve();
    return 0;
}
