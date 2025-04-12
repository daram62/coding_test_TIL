#include <iostream>
#include <queue>
#include <cmath>
#include <cstring>
using namespace std;

int N, M;
int map_[50][50];
int warriorMap[50][50], nextWarriorMap[50][50];
bool medusaArea[4][50][50];

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

int medusaR, medusaC;
int parkR, parkC;

pair<int, int> warriorsQueue[10000];
int frontIdx = 0, rearIdx = 0;

int getDist(int r1, int c1, int r2, int c2) {
    return abs(r1 - r2) + abs(c1 - c2);
}

bool inRange(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void pushWarrior(int r, int c) {
    warriorsQueue[rearIdx++] = {r, c};
}

bool popWarrior(int &r, int &c) {
    if (frontIdx == rearIdx) return false;
    tie(r, c) = warriorsQueue[frontIdx++];
    return true;
}

void resetWarriorQueue(pair<int, int>* src, int count) {
    for (int i = 0; i < count; i++) {
        warriorsQueue[i] = src[i];
    }
    frontIdx = 0;
    rearIdx = count;
}

bool step1(bool visited[50][50]) {
    int minDist = 1e9, chosenDir = -1, newR = -1, newC = -1;

    for (int d = 0; d < 4; d++) {
        int nr = medusaR + dr[d];
        int nc = medusaC + dc[d];
        if (!inRange(nr, nc) || visited[nr][nc] || map_[nr][nc] == 1) continue;

        bool bfsVisited[50][50] = {};
        queue<tuple<int, int, int>> q;
        q.push({0, nr, nc});
        bfsVisited[nr][nc] = true;

        while (!q.empty()) {
            auto [dist, r, c] = q.front(); q.pop();
            if (r == parkR && c == parkC) {
                if (dist < minDist || (dist == minDist && d < chosenDir)) {
                    minDist = dist;
                    chosenDir = d;
                    newR = nr;
                    newC = nc;
                }
                break;
            }
            for (int i = 0; i < 4; i++) {
                int rr = r + dr[i], cc = c + dc[i];
                if (!inRange(rr, cc) || map_[rr][cc] == 1 || bfsVisited[rr][cc]) continue;
                bfsVisited[rr][cc] = true;
                q.push({dist + 1, rr, cc});
            }
        }
    }

    if (chosenDir == -1) {
        cout << -1 << '\n';
        return false;
    }

    medusaR = newR;
    medusaC = newC;
    visited[medusaR][medusaC] = true;

    pair<int, int> temp[10000];
    int cnt = 0;
    for (int i = frontIdx; i < rearIdx; i++) {
        auto [r, c] = warriorsQueue[i];
        if (r == medusaR && c == medusaC) continue;
        temp[cnt++] = {r, c};
    }
    resetWarriorQueue(temp, cnt);
    warriorMap[medusaR][medusaC] = 0;

    if (medusaR == parkR && medusaC == parkC) {
        cout << 0 << '\n';
        return false;
    }

    return true;
}

pair<int, int> step2() {
    int maxKill = 0, dirChosen = 0;
    memset(medusaArea, false, sizeof(medusaArea));

    for (int d = 0; d < 4; d++) {
        bool rock[50] = {};
        int kill = 0;

        for (int step = 1; step < N; step++) {
            int r = medusaR + step * dr[d];
            int c = medusaC + step * dc[d];
            if (!inRange(r, c)) break;

            medusaArea[d][r][c] = true;
            if (warriorMap[r][c] > 0) {
                if (d <= 1) rock[c] = true;
                else rock[r] = true;
                kill += warriorMap[r][c];
                break;
            }
        }

        int rr = medusaR, cc = medusaC;
        while (inRange(rr, cc)) {
            bool left = false, right = false;
            for (int j = 1; j <= N && (!left || !right); j++) {
                int r = (d <= 1 ? rr + j * dr[d] : rr);
                int c = (d <= 1 ? cc : cc + j * dc[d]);
                if (!inRange(r, c)) break;

                if (d <= 1) {
                    if (!left && c - j >= 0 && !rock[c - j]) {
                        medusaArea[d][r][c - j] = true;
                        if (warriorMap[r][c - j] > 0) {
                            rock[c - j] = true;
                            kill += warriorMap[r][c - j];
                            left = true;
                        }
                    }
                    if (!right && c + j < N && !rock[c + j]) {
                        medusaArea[d][r][c + j] = true;
                        if (warriorMap[r][c + j] > 0) {
                            rock[c + j] = true;
                            kill += warriorMap[r][c + j];
                            right = true;
                        }
                    }
                } else {
                    if (!left && r - j >= 0 && !rock[r - j]) {
                        medusaArea[d][r - j][c] = true;
                        if (warriorMap[r - j][c] > 0) {
                            rock[r - j] = true;
                            kill += warriorMap[r - j][c];
                            left = true;
                        }
                    }
                    if (!right && r + j < N && !rock[r + j]) {
                        medusaArea[d][r + j][c] = true;
                        if (warriorMap[r + j][c] > 0) {
                            rock[r + j] = true;
                            kill += warriorMap[r + j][c];
                            right = true;
                        }
                    }
                }
            }
            rr += dr[d];
            cc += dc[d];
        }

        if (kill > maxKill || (kill == maxKill && d < dirChosen)) {
            maxKill = kill;
            dirChosen = d;
        }
    }

    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            if (medusaArea[dirChosen][r][c] && warriorMap[r][c] > 0) {
                nextWarriorMap[r][c] = warriorMap[r][c];
                warriorMap[r][c] = 0;
            }

    return {maxKill, dirChosen};
}

int step3(int turn, int medusaDir) {
    int sum = 0;
    pair<int, int> temp[10000];
    int cnt = 0;

    for (int i = frontIdx; i < rearIdx; i++) {
        auto [r, c] = warriorsQueue[i];
        if (warriorMap[r][c] == 0 || (r == medusaR && c == medusaC)) {
            temp[cnt++] = {r, c};
            continue;
        }

        int originDist = getDist(r, c, medusaR, medusaC);
        warriorMap[r][c]--;

        int bestDist = 1e9, bestDir = -1, nr = -1, nc = -1;
        for (int j = (turn == 1 ? 0 : 2); j < (turn == 1 ? 4 : 6); j++) {
            int d = j % 4;
            int tr = r + dr[d], tc = c + dc[d];
            if (!inRange(tr, tc) || medusaArea[medusaDir][tr][tc]) continue;
            int newDist = getDist(tr, tc, medusaR, medusaC);
            if (newDist < originDist) {
                if (newDist < bestDist || (newDist == bestDist && d < bestDir)) {
                    bestDist = newDist;
                    bestDir = d;
                    nr = tr;
                    nc = tc;
                }
            }
        }

        if (nr == -1) {
            warriorMap[r][c]++;
            temp[cnt++] = {r, c};
        } else {
            warriorMap[nr][nc]++;
            temp[cnt++] = {nr, nc};
            sum++;
        }
    }

    resetWarriorQueue(temp, cnt);
    return sum;
}

int step4() {
    int atk = 0;
    pair<int, int> temp[10000];
    int cnt = 0;

    for (int i = frontIdx; i < rearIdx; i++) {
        auto [r, c] = warriorsQueue[i];
        if (r == medusaR && c == medusaC) {
            atk++;
            warriorMap[r][c]--;
        } else {
            temp[cnt++] = {r, c};
        }
    }

    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++) {
            while (nextWarriorMap[r][c]--) {
                warriorMap[r][c]++;
                temp[cnt++] = {r, c};
            }
            nextWarriorMap[r][c] = 0;
        }

    resetWarriorQueue(temp, cnt);
    return atk;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> N >> M;
    cin >> medusaR >> medusaC >> parkR >> parkC;

    for (int i = 0; i < M; i++) {
        int r, c;
        cin >> r >> c;
        pushWarrior(r, c);
        warriorMap[r][c]++;
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> map_[i][j];

    bool visited[50][50] = {};

    while (!(medusaR == parkR && medusaC == parkC)) {
        if (!step1(visited)) break;
        auto [killCount, dir] = step2();
        int moveSum = step3(1, dir) + step3(2, dir);
        int atk = step4();
        cout << moveSum << ' ' << killCount << ' ' << atk << '\n';
    }

    return 0;
}
