#include <iostream>
#include <algorithm>
#include <vector>
#include <queue> 
#include <cmath> 
#include <cstring> 
using namespace std;

int N, M;
int map_[50][50];
int warriorMap[50][50], nextWarriorMap[50][50];
bool medusaArea[4][50][50];

int dr[4] = {-1, 1, 0, 0}; // 상, 하, 좌, 우
int dc[4] = {0, 0, -1, 1};

int medusaR, medusaC;
int parkR, parkC;

queue<pair<int, int>> warriors;

int getDist(int r1, int c1, int r2, int c2) {
    return abs(r1 - r2) + abs(c1 - c2);
}

bool inRange(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

// step 1: 메두사 이동
bool step1(vector<vector<bool>>& visited) {
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<>> pq;

    for (int d = 0; d < 4; d++) {
        int nr = medusaR + dr[d];
        int nc = medusaC + dc[d];
        if (!inRange(nr, nc) || visited[nr][nc] || map_[nr][nc] == 1) continue;

        vector<vector<bool>> bfsVisited(N, vector<bool>(N, false));
        queue<tuple<int, int, int>> q;
        q.push({0, nr, nc});
        bfsVisited[nr][nc] = true;

        int dist = -1;
        while (!q.empty()) {
            auto [d_, r, c] = q.front(); q.pop();
            if (r == parkR && c == parkC) {
                dist = d_;
                break;
            }
            for (int i = 0; i < 4; i++) {
                int rr = r + dr[i], cc = c + dc[i];
                if (!inRange(rr, cc) || map_[rr][cc] == 1 || bfsVisited[rr][cc]) continue;
                bfsVisited[rr][cc] = true;
                q.push({d_ + 1, rr, cc});
            }
        }

        if (dist != -1) pq.push({dist, d, nr, nc});
    }

    if (pq.empty()) {
        cout << -1 << '\n';
        return false;
    }

    auto [_, dir, nr, nc] = pq.top();
    medusaR = nr;
    medusaC = nc;
    visited[medusaR][medusaC] = true;

    // 전사 제거
    if (warriorMap[medusaR][medusaC] > 0) {
        int sz = warriors.size();
        queue<pair<int, int>> temp;
        for (int i = 0; i < sz; i++) {
            auto [r, c] = warriors.front(); warriors.pop();
            if (r == medusaR && c == medusaC) continue;
            temp.push({r, c});
        }
        warriors = temp;
        warriorMap[medusaR][medusaC] = 0;
    }

    if (medusaR == parkR && medusaC == parkC) {
        cout << 0 << '\n'; // ← 이 줄 추가 필요
        return false;
    }

    return true;
}

// step 2: 석화 마법
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
                    if (!left && c - j >= 0) {
                        if (rock[c - j]) left = true;
                        else {
                            medusaArea[d][r][c - j] = true;
                            if (warriorMap[r][c - j] > 0) {
                                rock[c - j] = true;
                                kill += warriorMap[r][c - j];
                                left = true;
                            }
                        }
                    }
                    if (!right && c + j < N) {
                        if (rock[c + j]) right = true;
                        else {
                            medusaArea[d][r][c + j] = true;
                            if (warriorMap[r][c + j] > 0) {
                                rock[c + j] = true;
                                kill += warriorMap[r][c + j];
                                right = true;
                            }
                        }
                    }
                } else {
                    if (!left && r - j >= 0) {
                        if (rock[r - j]) left = true;
                        else {
                            medusaArea[d][r - j][c] = true;
                            if (warriorMap[r - j][c] > 0) {
                                rock[r - j] = true;
                                kill += warriorMap[r - j][c];
                                left = true;
                            }
                        }
                    }
                    if (!right && r + j < N) {
                        if (rock[r + j]) right = true;
                        else {
                            medusaArea[d][r + j][c] = true;
                            if (warriorMap[r + j][c] > 0) {
                                rock[r + j] = true;
                                kill += warriorMap[r + j][c];
                                right = true;
                            }
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

    // 석화 적용
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            if (medusaArea[dirChosen][r][c] && warriorMap[r][c] > 0) {
                nextWarriorMap[r][c] = warriorMap[r][c];
                warriorMap[r][c] = 0;
            }

    return {maxKill, dirChosen};
}

// step 3: 전사 이동 (이동 우선순위 다름)
int step3(int turn, int medusaDir) {
    int sum = 0;
    int sz = warriors.size();
    queue<pair<int, int>> temp;

    for (int i = 0; i < sz; i++) {
        auto [r, c] = warriors.front(); warriors.pop();
        if (warriorMap[r][c] == 0 || (r == medusaR && c == medusaC)) {
            temp.push({r, c});
            continue;
        }

        int originalDist = getDist(r, c, medusaR, medusaC);
        warriorMap[r][c]--;

        vector<tuple<int, int, int, int>> candidates;
        for (int j = (turn == 1 ? 0 : 2); j < (turn == 1 ? 4 : 6); j++) {
            int d = j % 4;
            int nr = r + dr[d], nc = c + dc[d];
            if (!inRange(nr, nc) || medusaArea[medusaDir][nr][nc]) continue;
            int newDist = getDist(nr, nc, medusaR, medusaC);
            if (newDist >= originalDist) continue;
            candidates.emplace_back(newDist, d, nr, nc);
        }

        if (candidates.empty()) {
            warriorMap[r][c]++;
            temp.push({r, c});
            continue;
        }

        sort(candidates.begin(), candidates.end());
        auto [_, __, nr, nc] = candidates[0];
        warriorMap[nr][nc]++;
        temp.push({nr, nc});
        sum++;
    }

    warriors = temp;
    return sum;
}

// step 4: 공격
int step4() {
    int atk = 0;
    int sz = warriors.size();
    queue<pair<int, int>> temp;

    for (int i = 0; i < sz; i++) {
        auto [r, c] = warriors.front(); warriors.pop();
        if (r == medusaR && c == medusaC) {
            atk++;
            warriorMap[r][c]--;
            continue;
        }
        temp.push({r, c});
    }

    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            while (nextWarriorMap[r][c]--) {
                warriorMap[r][c]++;
                temp.push({r, c});
            }

    memset(nextWarriorMap, 0, sizeof(nextWarriorMap));
    warriors = temp;
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
        warriors.push({r, c});
        warriorMap[r][c]++;
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> map_[i][j];

    vector<vector<bool>> visited(N, vector<bool>(N, false));

    while (!(medusaR == parkR && medusaC == parkC)) {
        if (!step1(visited)) break;
        auto [killCount, dir] = step2();
        int moveSum = step3(1, dir) + step3(2, dir);
        int atk = step4();
        cout << moveSum << ' ' << killCount << ' ' << atk << '\n';
    }

    return 0;
}