#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<std::string> zodiac = {
        "Ox", "Tiger", "Rabbit", "Dragon", "Snake", "Horse",
        "Goat", "Monkey", "Rooster", "Dog", "Pig", "Rat"
    };

    std::map<std::string, int> zodiacIndex;
    for (int i = 0; i < 12; ++i) {
        zodiacIndex[zodiac[i]] = i;
    }

    std::map<std::string, int> birthYear;
    birthYear["Bessie"] = 0;

    for (int i = 0; i < n; ++i) {
        std::string cow1, born, in, relation, animal, year, from, cow2;
        std::cin >> cow1 >> born >> in >> relation >> animal >> year >> from >> cow2;

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

    int answer = std::abs(birthYear["Elsie"] - birthYear["Bessie"]);
    std::cout << answer << "\n";
    return 0;
}
