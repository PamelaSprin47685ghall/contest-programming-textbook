
#include <algorithm>
#include <cstdio>
#include <vector>

struct Cow {
    int id;
    char dir;
    int x, y;
};

struct Collision {
    int time;
    int stoppedCow;
    int blockerCow;
};

int main() {
    int n;
    scanf("%d", &n);

    std::vector<Cow> allCows(n);
    std::vector<Cow> eastCows;
    std::vector<Cow> northCows;

    for (int i = 0; i < n; ++i) {
        char dir;
        int x, y;
        scanf(" %c%d%d", &dir, &x, &y);
        allCows[i] = Cow{i, dir, x, y};
        if (dir == 'E') {
            eastCows.push_back(allCows[i]);
        } else {
            northCows.push_back(allCows[i]);
        }
    }

    std::vector<Collision> collisions;

    for (const Cow& e : eastCows) {
        for (const Cow& nr : northCows) {
            if (nr.x > e.x && e.y > nr.y) {
                int dx = nr.x - e.x;
                int dy = e.y - nr.y;
                if (dx > dy) {
                    collisions.push_back(Collision{dx, e.id, nr.id});
                } else if (dy > dx) {
                    collisions.push_back(Collision{dy, nr.id, e.id});
                }
            }
        }
    }

    std::sort(collisions.begin(), collisions.end(), [](const Collision& a, const Collision& b) {
        return a.time < b.time;
    });

    std::vector<int> stopDist(n, -1);

    for (const Collision& c : collisions) {
        int victim = c.stoppedCow;
        int blocker = c.blockerCow;

        if (stopDist[victim] != -1) continue;

        int blockerDistToIntersection = 0;
        if (allCows[blocker].dir == 'E') {
            blockerDistToIntersection = allCows[victim].x - allCows[blocker].x;
        } else {
            blockerDistToIntersection = allCows[victim].y - allCows[blocker].y;
        }

        if (stopDist[blocker] != -1 && stopDist[blocker] < blockerDistToIntersection) {
            continue;
        }

        stopDist[victim] = c.time;
    }

    for (int i = 0; i < n; ++i) {
        if (stopDist[i] == -1) {
            printf("Infinity\n");
        } else {
            printf("%d\n", stopDist[i]);
        }
    }

    return 0;
}
