#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
int R, C, K;
int forest[75][75];
pair <int, int> golem[1001]; // c(열), d(출구)
pair <int, int> center[1001]; // 행, 열
bool Exit[75][75];
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, 0, -1}; // 방향 북동남서
int sum; // 행 번호 합
pair <int, int> Center;

void Print() {
    for (int i = 1; i <= R+3; i++) {
        for (int j = 1; j <= C; j++) {
            cout << forest[i][j] << " ";
        }
        cout << endl;
    }
}

bool check_golem(int r, int c) {    
    // 범위 체크
    if (r-1 < 1 || r+1 > R+3 || c-1 < 1 || c+1 > C) 
        return false;
    // 비어있는지 체크
    if (forest[r][c] != 0 || forest[r-1][c] != 0 || forest[r+1][c] != 0 || forest[r][c-1] != 0 || forest[r][c+1] != 0)
        return false;

    return true;
}

void locate_golem(int c, int d, int n) {
    // 남 -> 서 -> 동 체크
    int sR = 2;
    int sC = c;

    // 이동 방향 체크
    while (1) {
        // 남쪽 체크
        sR++;
        if (check_golem(sR, sC)) {
            continue;
        }
        sR--;
        
        // 서쪽 체크
        sC--;
        if (check_golem(sR, sC)) {
            // 남쪽 체크
            sR++;
            if (check_golem(sR, sC)) {
                // 출구 좌표 회전 
                d = (d + 3) % 4;
                continue;
            }
            else sR--;
        }
        sC++;

        // 동쪽 체크
        sC++;
        if (check_golem(sR, sC)) {
            // 남쪽 체크
            sR++;
            if (check_golem(sR, sC)) {
                // 출구 좌표 회전
                d = (d + 1) % 4;
                continue;
            }
            else sR--;
        }
        sC--;
        break;
    }

    // 실제로 골렘 배정
    forest[sR][sC] = n;
    forest[sR-1][sC] = n;
    forest[sR+1][sC] = n;
    forest[sR][sC-1] = n;
    forest[sR][sC+1] = n;
    Center.first = sR;
    Center.second = sC;

    // 출구 좌표 업데이트
    Exit[sR + dy[d]][sC + dx[d]] = true;
}

bool isOut() {
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= C; j++) {
            if (forest[i][j] != 0)
                return true;
        }
    }
    return false;
}

void Clear() {
    for (int i = 1; i <= R+3; i++) {
        for (int j = 1; j <= C; j++) {
            forest[i][j] = 0;
            Exit[i][j] = 0;
        }
    }  
}

bool isValid(int r, int c) {
    return (r >= 1 && r <= R+3 && c >= 1 && c <= C);
}

void move_ghost(int y, int x, int num) {
    queue<pair<int,int>> q;
    int max_y = y;
    bool visited[75][75] = { false };
    visited[y][x] = true;

    for (int i = 0; i < 4; i++) {
        int ny = y + dy[i];
        int nx = x + dx[i];

        visited[ny][nx] = true;
        q.push({ny, nx});
    }

    while (!q.empty()) {
        auto now = q.front();
        q.pop();    
        max_y = max(max_y, now.first);
    
        if (Exit[now.first][now.second]) {        
            for (int i = 0; i < 4; i++) {
                int ny = now.first + dy[i];
                int nx = now.second + dx[i];
                if (visited[ny][nx] == true)
                    continue;
                if (!isValid(ny, nx) || forest[ny][nx] == 0) {
                    continue;
                }
                visited[ny][nx] = true;
                q.push({ny, nx});
            }
        }
        else {
            for (int i = 0; i < 4; i++) {
                int ny = now.first + dy[i];
                int nx = now.second + dx[i];
                if (visited[ny][nx] == true)
                    continue;
                if (!isValid(ny, nx) || forest[ny][nx] == 0)
                    continue;
                if (forest[ny][nx] == forest[now.first][now.second]) {
                    visited[ny][nx] = true;
                    q.push({ny, nx});
                }
            }
        }
    }
    sum += max_y - 3;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 0. 입력
    cin >> R >> C >> K;
    for (int i = 1; i <= K; i++) {
        cin >> golem[i].first >> golem[i].second;
    }
    // 1. 실행
    for (int i = 1; i <= K; i++) {
        int c = golem[i].first;
        int d = golem[i].second;
        // 골렘 배치
        locate_golem(c, d, i);
        //숲 벗어나는지 체크
        if (isOut()) {
            Clear();
            continue;
        }
        // 정령 이동
        move_ghost(Center.first, Center.second, i);
    }

    cout << sum;
    return 0;
}