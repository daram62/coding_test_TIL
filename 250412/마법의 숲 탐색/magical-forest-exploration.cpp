#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int R, C, K, ans;
int dy[4] = { 1, -1, 0, 0 }; // 상하좌우 y 이동
int dx[4] = { 0, 0, 1, -1 }; // 상하좌우 x 이동

int Map[73][73];             // 0: 빈칸, 1~K: 골렘 번호
bool golem_exit[73][73];     // 출구 위치 표시

struct golem_info {
    int c, d; // 열 위치, 방향 (0: 북, 1: 동, 2: 남, 3: 서)
};

vector<golem_info> golem;

// 입력 처리
void input() {
    cin >> R >> C >> K;
    for (int i = 0; i < K; i++) {
        int c, d;
        cin >> c >> d;
        golem.push_back({ c, d });
    }
}

// 십자모양 배치 가능한지 확인
bool isValid(int y, int x) {
    return Map[y][x] == 0 &&
           Map[y-1][x] == 0 && Map[y+1][x] == 0 &&
           Map[y][x-1] == 0 && Map[y][x+1] == 0 &&
           y <= R + 1 && x >= 2 && x <= C - 1;
}

// 출구에서 이동 가능한 칸인지 (다른 골렘 몸체도 포함)
bool isValid2(int y, int x) {
    return y <= R + 2 && x >= 1 && x <= C && Map[y][x] != 0;
}

// 내 골렘 번호만 따라갈 수 있는지
bool isValid3(int y, int x, int num) {
    return y <= R + 2 && x >= 1 && x <= C && Map[y][x] == num;
}

// 맵 초기화
void mapRemove() {
    for (int i = 0; i < 73; i++) {
        for (int j = 0; j < 73; j++) {
            Map[i][j] = 0;
            golem_exit[i][j] = false;
        }
    }
}

// 출구 또는 몸체를 통해 BFS 탐색, 가장 밑 y 좌표 기록
void bfs(int y, int x, int num) {
    queue<pair<int, int>> q;
    int max_y = y;
    bool visited[73][73] = { false };
    visited[y][x] = true;

    // 십자형 4방향 큐에 먼저 추가
    for (int i = 0; i < 4; i++) {
        int ny = y + dy[i];
        int nx = x + dx[i];
        visited[ny][nx] = true;
        q.push({ ny, nx });
    }

    while (!q.empty()) {
        auto now = q.front(); q.pop();
        max_y = max(max_y, now.first);

        if (golem_exit[now.first][now.second]) {
            // 출구면 다른 골렘 몸체도 탐색 가능
            for (int i = 0; i < 4; i++) {
                int ny = now.first + dy[i];
                int nx = now.second + dx[i];
                if (visited[ny][nx]) continue;
                if (!isValid2(ny, nx)) continue;
                visited[ny][nx] = true;
                q.push({ ny, nx });
            }
        } else {
            // 내 골렘 번호만 따라 이동
            for (int i = 0; i < 4; i++) {
                int ny = now.first + dy[i];
                int nx = now.second + dx[i];
                if (visited[ny][nx]) continue;
                if (!isValid3(ny, nx, Map[now.first][now.second])) continue;
                visited[ny][nx] = true;
                q.push({ ny, nx });
            }
        }
    }

    ans += max_y - 2; // 0~1행은 공중이므로 제외
}

// 골렘 하나 떨어뜨려서 배치하는 시뮬레이션
void moveGolem(golem_info a, int num) {
    int sR = 0;       // 공중 시작
    int sC = a.c;
    int sD = a.d;

    while (1) {
        // 1. 남쪽 이동
        sR++;
        if (isValid(sR, sC)) continue;
        sR--;

        // 2. 서쪽 이동 + 반시계 회전
        sC--;
        if (isValid(sR, sC)) {
            sR++;
            if (isValid(sR, sC)) {
                sD = (sD + 3) % 4;
                continue;
            } else sR--;
        }
        sC++;

        // 3. 동쪽 이동 + 시계 회전
        sC++;
        if (isValid(sR, sC)) {
            sR++;
            if (isValid(sR, sC)) {
                sD = (sD + 1) % 4;
                continue;
            } else sR--;
        }
        sC--;

        break; // 3방향 모두 실패
    }

    // 너무 위에서 멈췄으면 실패 → 맵 초기화
    if (sR <= 3) {
        mapRemove();
        return;
    }

    // 골렘 십자모양 배치
    Map[sR][sC] = num;
    Map[sR-1][sC] = num;
    Map[sR+1][sC] = num;
    Map[sR][sC-1] = num;
    Map[sR][sC+1] = num;

    // 출구 위치 표시
    if (sD == 0) golem_exit[sR-1][sC] = true;       // 북
    else if (sD == 1) golem_exit[sR][sC+1] = true;  // 동
    else if (sD == 2) golem_exit[sR+1][sC] = true;  // 남
    else if (sD == 3) golem_exit[sR][sC-1] = true;  // 서

    bfs(sR, sC, num); // 최종 깊이 탐색
}

// 전체 실행
void func() {
    for (int i = 0; i < K; i++) {
        moveGolem(golem[i], i + 1);
    }
    cout << ans << '\n';
}

int main() {
    input();
    func();
    return 0;
}