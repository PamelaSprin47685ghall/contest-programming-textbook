#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void Solve() {
    int n, c, p;
    std::cin >> n >> c >> p;
    std::vector<std::string> nouns, tverbs, iverbs, conjs;
    for (int i = 0; i < n; ++i) {
        std::string w, t;
        std::cin >> w >> t;
        if (t == "noun") nouns.push_back(w);
        else if (t == "transitive-verb") tverbs.push_back(w);
        else if (t == "intransitive-verb") iverbs.push_back(w);
        else if (t == "conjunction") conjs.push_back(w);
    }

    int bestWords = 0;
    int bestT = 0, bestI = 0, bestConj = 0, bestExtraN = 0;

    int numNouns = nouns.size();
    int numT = tverbs.size();
    int numI = iverbs.size();
    int numConj = conjs.size();

    for (int t = 0; t <= numT; ++t) {
        if (2 * t > numNouns) break;
        int remNouns = numNouns - 2 * t;
        int maxI = std::min(numI, remNouns);

        for (int i = 0; i <= maxI; ++i) {
            int totalSentences = t + i;
            if (totalSentences == 0) continue;

            int maxC = std::min(numConj, totalSentences / 2);
            int minC = std::max(0, totalSentences - p);
            if (minC > maxC) continue;

            int conjUsed = maxC;
            int leftNouns = remNouns - i;
            int extraN = 0;
            if (t > 0) {
                extraN = std::min(leftNouns, c);
            }

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
        std::cout << "0\n\n";
        return;
    }

    std::vector<std::string> sentences;
    for (int i = 0; i < bestI; ++i) {
        std::string s = nouns.back() + " " + iverbs.back();
        nouns.pop_back();
        iverbs.pop_back();
        sentences.push_back(s);
    }
    for (int i = 0; i < bestT; ++i) {
        std::string s = nouns.back() + " " + tverbs.back();
        nouns.pop_back();
        tverbs.pop_back();
        s += " " + nouns.back();
        nouns.pop_back();
        if (i == 0) {
            for (int e = 0; e < bestExtraN; ++e) {
                s += ", " + nouns.back();
                nouns.pop_back();
            }
        }
        sentences.push_back(s);
    }

    std::string output = "";
    int conjIdx = 0;
    for (std::size_t i = 0; i < sentences.size(); ++i) {
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

    std::cout << bestWords << "\n" << output << "\n";
}

int main() {
    int t;
    if (std::cin >> t) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
