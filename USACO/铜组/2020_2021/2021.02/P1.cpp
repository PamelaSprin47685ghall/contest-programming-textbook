#include <cmath>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    vector<string> zodiac = {
        "Ox", "Tiger", "Rabbit", "Dragon", "Snake", "Horse",
        "Goat", "Monkey", "Rooster", "Dog", "Pig", "Rat"
    };

    map<string, int> zodiacIndex;
    for (int i = 0; i < 12; ++i) {
        zodiacIndex[zodiac[i]] = i;
    }

    map<string, int> birthYear;
    birthYear["Bessie"] = 0;

    for (int i = 0; i < n; ++i) {
        char cow1Buffer[64], bornBuffer[64], inBuffer[64], relationBuffer[64];
        char animalBuffer[64], yearBuffer[64], fromBuffer[64], cow2Buffer[64];
        scanf("%s%s%s%s%s%s%s%s", cow1Buffer, bornBuffer, inBuffer, relationBuffer,
              animalBuffer, yearBuffer, fromBuffer, cow2Buffer);
        string cow1 = cow1Buffer, born = bornBuffer, in = inBuffer, relation = relationBuffer;
        string animal = animalBuffer, year = yearBuffer, from = fromBuffer, cow2 = cow2Buffer;

        int targetZ = zodiacIndex[animal];
        int baseYear = birthYear[cow2];
        // cow2 出生年份所对应的生肖 (注意模 12 修正为正数)
        int baseZ = (baseYear % 12 + 12) % 12;

        int newYear = baseYear;
        if (relation == "previous") {
            int diff = (baseZ - targetZ + 12) % 12;
            if (diff == 0) diff = 12;
            newYear -= diff;
        } else {
            // relation == "next"
            int diff = (targetZ - baseZ + 12) % 12;
            if (diff == 0) diff = 12;
            newYear += diff;
        }

        birthYear[cow1] = newYear;
    }

    int answer = abs(birthYear["Elsie"] - birthYear["Bessie"]);
    printf("%d\n", answer);
    return 0;
}
