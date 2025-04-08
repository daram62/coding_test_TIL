#include <iostream>
#include <algorithm>
using namespace std;

int N, M, K;
int maze[11][11]; // 1-based 인덱스 사용 위해 크기 +1
pair<int, int> people[10]; // 사람 좌표: 1-based
pair<int, int> Exit;       // 출구 좌표: 1-based
int sum;
int Count;
int dx[4] = {-1, 1, 0, 0}; // 상하좌우
int dy[4] = {0, 0, -1, 1};
int k_min;

// 디버깅용 출력
void Print() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

// 최단 거리 구하기
int get_min(int x, int y) {
    return abs(x - Exit.first) + abs(y - Exit.second);
}

// 정사각형 시작 좌표 찾기
bool isSquare(int x, int y, int side) {
    bool isPerson = false, isExit = false;

    for (int i = 0; i < M; i++) {
        int r = people[i].first, c = people[i].second;
        if (r == 0 && c == 0) continue; // 탈출했으면
        // 사람 있는지 체크
        if (r >= x && r < x + side && c >= y && c < y + side)
            isPerson = true;
    }
    // 출구 있는지 체크
    if (Exit.first >= x && Exit.first < x + side && Exit.second >= y && Exit.second < y + side)
        isExit = true;

    return isPerson && isExit;
}

// 정사각형 회전 함수
void rotate(int x, int y, int side) {
    int temp[11][11];
    // 회전할 공간만 빼서 임시에 저장
    for (int i = 0; i < side; i++) 
        for (int j = 0; j < side; j++)
            temp[i][j] = maze[x + i][y + j];
    // 미로 회전 
    for (int i = 0; i < side; i++)
        for (int j = 0; j < side; j++) {
            int val = temp[i][j];
            if (val > 0) val--; // 내구도 감소
            maze[x + j][y + side - 1 - i] = val;
        }
    // 사람 회전
    for (int i = 0; i < M; i++) {
        int r = people[i].first, c = people[i].second;
        if (r == 0 && c == 0) continue; // 이미 탈출
        if (r >= x && r < x + side && c >= y && c < y + side) {
            // 정사각형 시작 위치에 회전 좌표 넣기 
            int nr = x + (c - y);
            int nc = y + side - 1 - (r - x);
            people[i] = {nr, nc};
        }
    }
    // 출구 좌표 회전
    int er = Exit.first, ec = Exit.second;
    if (er >= x && er < x + side && ec >= y && ec < y + side) {
        int ner = x + (ec - y);
        int nec = y + side - 1 - (er - x);
        Exit = {ner, nec};
    }
}

int main() {
    // 0. 입력
    cin >> N >> M >> K;

    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            cin >> maze[i][j];

    for (int i = 0; i < M; i++)
        cin >> people[i].first >> people[i].second;

    cin >> Exit.first >> Exit.second;
    Count = M;

    // K초 반복
    for (int second = 1; second <= K; second++) {
        // 1. 이동
        for (int i = 0; i < M; i++) {
            int x = people[i].first, y = people[i].second;
            if (x == 0 && y == 0) continue; // 이미 탈출

            int dist = get_min(x, y);

            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx < 1 || ny < 1 || nx > N || ny > N) continue;
                if (maze[nx][ny] != 0) continue;

                int n_dist = get_min(nx, ny);
                // 최단 거리 작아질 때만
                if (n_dist < dist) {
                    people[i] = {nx, ny};
                    sum++;
                    // 탈출 체크
                    if (nx == Exit.first && ny == Exit.second) {
                        people[i] = {0, 0};
                        Count--;
                    }
                    k_min = min(k_min, n_dist);
                    break; // 한 번만 이동
                }
            }
        }
        // 다 탈출 했으면
        if (Count == 0) break;
        // 2. 회전
        bool rotated = false;
        for (int side = 2; side <= N && !rotated; side++) {
            for (int i = 1; i <= N - side + 1 && !rotated; i++) {
                for (int j = 1; j <= N - side + 1 && !rotated; j++) {
                    if (isSquare(i, j, side)) {
                        rotate(i, j, side);
                        rotated = true;
                    }
                }
            }
        }
    }
    cout << sum << "\n" << Exit.first << " " << Exit.second << "\n";
    return 0;
}