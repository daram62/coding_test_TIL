#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

int K, M; // 탐사 반복 횟수, 유물 조각 개수
int map[6][6]; // 유적 지도
queue<int> wall; // 유적 벽
int dir[3] = {0,1,2}; // 90, 180, 270도

int dx[4] = {0, 0, 1, -1}; // 남북
int dy[4] = {1, -1, 0, 0}; // 동서
int final = 0; // 턴 마다 최종 점수

int rotate(int x, int y, int direction);
int check_Reward(int area[6][6]);
void real_rotate(int x, int y, int direction);
int real_Reward();
void fillup_Empty();

void Print() {
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 0. 입력 받기
    cin >> K >> M;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            cin >> map[i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        int w;
        cin >> w;
        wall.push(w);
    }

    // K번 동안 탐사 진행
    for (int TC = 0; TC < K; TC++) {

        final = 0;
        // 1. 탐사 진행
        // 3*3 만 회전 좌표 가능

        int result = 0; // 첫 번쨰 탐사 유물 가치
        int final_result = 0; // 유물 가치 총 합
        pair<int,int> point; // 회전 좌표
        int point_angle = 0; // 회전 각도

        for (int k = 0; k < 3; k++) { // 각도 우선 탐사
            for (int i = 2; i < 5; i++) {
                for (int j = 2; j < 5; j++) {
                    // cout << "i: " << i << " j: " << j << " 각도: " << (k+1)*90 << "\n";
                    int temp = rotate(i, j, k);
                    // cout << "count: " << temp << "\n";

                    if (temp > result) {
                        // 유물 1차 획득 가치 기준으로 최댓값 체크
                        // 다 돌고 최댓값 기준으로 회전 중심 좌표, 각도 선정 후 회전
                        result = temp;
                        point.first = i;
                        point.second = j;
                        point_angle = k;
                    }
                }
            }
        }
        // 탐사 진행 해도 유물 못 찾으면 종료
        if (result == 0) 
            break;

        // 1차 탐사 결과
        // cout << "회전 위치: "; 
        // cout << point.first << " " << point.second << " " << point_angle << "\n";
        // cout << "시작 가치: " << result << "\n";

        // 최종 회전 좌표, 각도 따라 이동
        // cout << "최종 회전 맵" << endl;
        real_rotate(point.first, point.second, point_angle);
        
        // 2. 유물 획득
        // 유물 1차 획득 다시 진행
        while(true) {
            int score = real_Reward();
            if (score == 0)
                break;
            final += score;
            fillup_Empty();
        }

        cout << final << " ";
    }
    return 0;
}

int rotate(int x, int y, int direction) {   
    int temp_map[6][6];
        
    // 전체 복사
    for (int i = 1; i <= 5; i++)
        for (int j = 1; j <= 5; j++)
            temp_map[i][j] = map[i][j];

    // 3x3 블록만 복사
    int block[3][3];
    int rotated[3][3];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            block[i][j] = map[x - 1 + i][y - 1 + j];

    for (int rot = 0; rot <= direction; rot++) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                rotated[j][2 - i] = block[i][j];
        memcpy(block, rotated, sizeof(rotated));
        }

    // temp_map에 덮어쓰기
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            temp_map[x - 1 + i][y - 1 + j] = block[i][j];

    return check_Reward(temp_map);
}

int check_Reward(int area[6][6]) {
    // 공간 돌면서 유물 있는지 체크
    bool visited[6][6] = { false };
    int reward = 0;

    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            // 방문 체크
            if (visited[i][j]) continue;

            int count = 1;
            int value = area[i][j];

            queue<pair<int, int>> q;
            q.push({i,j});
            visited[i][j] = true;

            while(!q.empty()) {
                int x = q.front().first;
                int y = q.front().second;
                q.pop();

                for (int i = 0; i < 4; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    // 범위 체크
                    if (nx < 1 || nx > 5 || ny < 1 || ny > 5) continue;
                    if (visited[nx][ny]) continue;
                    if (area[nx][ny] != value) continue;

                    q.push({nx, ny});
                    visited[nx][ny] = true;
                    count++;
                }
            }
            if (count >= 3) {
                reward += count;
            }
        }
    }
    return reward;
}

void real_rotate(int x, int y, int direction) {   
    
    // 3x3 블록만 복사
    int block[3][3];
    int rotated[3][3];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            block[i][j] = map[x - 1 + i][y - 1 + j];

    for (int rot = 0; rot <= direction; rot++) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                rotated[j][2 - i] = block[i][j];
        memcpy(block, rotated, sizeof(rotated));
    }
        

    // map에 덮어쓰기
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            map[x - 1 + i][y - 1 + j] = rotated[i][j];

    //Print();
}

int real_Reward() {
    // 공간 돌면서 유물 있는지 체크
    // BFS 
    bool visited[6][6] = { false };
    int reward = 0;

    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            // 방문 체크
            if (visited[i][j]) continue;

            int count = 1;
            int value = map[i][j];

            queue<pair<int, int>> q;
            queue<pair<int,int>> record;

            q.push({i,j});
            visited[i][j] = true;
            record.push({i, j});


            while(!q.empty()) {
                int x = q.front().first;
                int y = q.front().second;
                q.pop();

                for (int i = 0; i < 4; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    // 범위 체크
                    if (nx < 1 || nx > 5 || ny < 1 || ny > 5) continue;
                    if (visited[nx][ny]) continue;
                    if (map[nx][ny] != value) continue;

                    q.push({nx, ny});
                    visited[nx][ny] = true;
                    record.push({nx, ny});
                    count++;
                }
            }
            if (count >= 3) {
                reward += count;

                // 유물 획득 후 지우는 과정
                while (!record.empty()) {
                    int r = record.front().first;
                    int c = record.front().second;
                    record.pop();
                    map[r][c] = 0;
                }
            }
        }
    }
    
//    cout << "유물 획득 " << reward << endl;
    return reward;
}

void fillup_Empty() {
    // 열 작은 순, 행 큰 순
    for (int j = 1; j <= 5; j++) {
        for (int i = 5; i > 0; i--) {
            if (map[i][j] == 0) {
                map[i][j] = wall.front();
                wall.pop();
            }
        }
    }
}