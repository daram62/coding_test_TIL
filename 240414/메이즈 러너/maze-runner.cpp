// SS 2023년도 상반기 오후반 1번 - 메이즈 러너
#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>
#include <queue>
using namespace std;

int N, M, K; // 미로 크기, 참가자 수, 게임 시간
int maze[12][12]; // 미로 빈칸, 내구도 담는 배열
int r_maze[12][12]; // 미로 내 참가자 위치 담는 배열
int temp_maze[12][12];
int temp_r_maze[12][12];
map<int, int> runner[12]; // 참가자 위치 저장 배열
bool isRunner[12]; // 참가자 존재 여부 변수
pair <int, int> Exit; // 출구 좌표
pair <int, int> square;
int cnt; // 이동 횟수
// 상하좌우 방향 저장 
// 인덱스 1부터 쓸거라 그냥 [0] 여기는 0으로 지정함
int dx[5] = {0,-1,1,0,0}; 
int dy[5] = {0,0,0,-1,1};

void Print();
bool AllEscape();
bool Find_square(int x, int y, int len, int idx);
void rotate(int len);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> N >> M >> K;
    
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> maze[i][j]; // 0 - 빈칸, 1~9 - 벽의 내구도
        }
    }
    for (int i = 1; i <= M; i++) {
        int r, c;
        cin >> r >> c;
        runner[i][0] = r; // 참가자 위치 저장
        runner[i][1] = c;
        r_maze[r][c] = i; // 참가자 위치 참가자_미로에 저장
        isRunner[i] = true; // 참가자 살아있는지 체크
    }
    cin >> Exit.first >> Exit.second;
    maze[Exit.first][Exit.second] = -1;
    
    // 메인 기능 구현 ㄱㄱ
    for (int T = 1; T <= K; T++) {
        if (AllEscape()) // 모두 탈출했으면 게임 끝내기
            break;
        
        // 참가자 이동 시작
        for (int i = 1; i <= M; i++) {
            if (!isRunner[i])
                continue;
            int dir = 0;
            int min_distance = abs(runner[i][0] - Exit.first) + abs(runner[i][1] - Exit.second);
            int nx, ny;
            for (int j = 1; j <= 4; j++) {
                nx = runner[i][0] + dx[j];
                ny = runner[i][1] + dy[j];
                if (maze[nx][ny] >= 1 && maze[nx][ny] <= 9) // 벽이라면
                    continue;
                int distance = abs(nx - Exit.first) + abs(ny - Exit.second);
                if (distance < min_distance) {
                    min_distance = distance;
                    dir = j;
                }
            }   
            if (dir != 0) {
                r_maze[runner[i][0]][runner[i][1]] = 0;
                runner[i][0] += dx[dir];
                runner[i][1] += dy[dir];
                cnt++;
                if (runner[i][0] == Exit.first && runner[i][1] == Exit.second) {
                    isRunner[i] = false;
                }
                else 
                    r_maze[runner[i][0]][runner[i][1]] = i;            
            }
        }
        //Print();
        // 미로 회전 시작 
        
        // 정사각형 길이와 안에 있을 참가자 인덱스 찾기
        int square_idx = 0;
        int square_len = N;
        for (int i = 1; i <= M; i++) {
            if (!isRunner[i])
                continue;
            int sub_x, sub_y, len;
            sub_x = abs(runner[i][0] - Exit.first);
            sub_y = abs(runner[i][1] - Exit.second); 
            len = max(sub_x, sub_y) + 1;
            if (len < square_len) {
                square_len = len;
                square_idx = i;
            }                
            else if (len == square_len) {
                if (runner[i][0] < runner[square_idx][0]) {
                    square_idx = i;
                }
                else if (runner[i][0] == runner[square_idx][0]) {
                    if (runner[i][1] < runner[square_idx][1]) {
                        square_idx = i;
                    }
                }
            }
        }
        // 정사각형 정확한 위치 찾기
        bool found = false;
        for (int i = 1; i <= N - square_len + 1; i++) {
            for (int j = 1; j <= N - square_len + 1; j++) {
                found = Find_square(i, j, square_len, square_idx);
                if (found) {
                    break;
                }
            }
            if (found) 
                break;
        }
        // 회전 하자 ~
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                temp_maze[i][j] = 0;
                temp_r_maze[i][j] = 0;
            }
        }
        rotate(square_len);
        //Print();
    }
    cout << cnt << endl;
    cout << Exit.first << " " << Exit.second << endl;
    return 0;
}

void Print() {
    cout << "빈칸, 벽 표시" << endl;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
    cout << "참가자 위치 표시" << endl;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << r_maze[i][j] << " ";
        }
        cout << endl;
    }
}

bool AllEscape() {
    bool ans = true;
    for (int i = 1; i <= M; i++) {
        if (isRunner[i])
            ans = false;
    }
    return ans;
}

bool Find_square(int x, int y, int len, int idx) {
    bool isExit = false;
    bool isIdx = false;
    for (int i = x; i <= x+len-1; i++) {
        for (int j = y; j <= y+len-1; j++) {
            if (maze[i][j] == -1) // exit ?
                isExit = true;
            else if (r_maze[i][j] == idx) // idx ? 
                isIdx = true;
        }
    }
    if (isExit && isIdx) {
        square.first = x;
        square.second = y;
        return true;
    }
    return false;
}

void rotate(int len) {
    int x, y;
    x = square.first;
    y = square.second;
    
    // 미로 - 벽, 출구
    // 행렬 변환 
    queue <int> num;
    for (int i = x; i <= x+len-1; i++) {
        for (int j = y; j <= y+len-1; j++) {
            //cout << maze[i][j] << " ";
            num.push(maze[i][j]);
        }
    }
    for (int i = y; i <= y+len-1; i++) {
        for (int j = x; j <= x+len-1; j++) {
            int temp = num.front();
            num.pop();
            //cout << "temp : " << temp << endl;
            //cout << j << ", " << i << endl;
            temp_maze[j][i] = temp;
        }
    }
    // 열 스왑하기
    for (int i = x; i <= x+len-1; i++) {
        for (int j = y; j <= y+len-1; j++) {
            int temp = y;
            if (y == 1)
                temp = 0;
            maze[i][len+1-j+temp] = temp_maze[i][j];

            if (maze[i][len+1-j+temp] >= 1 && maze[i][len+1-j+temp] <= 9)
                maze[i][len+1-j+temp] --;
            else if (maze[i][len+1-j+temp] == -1) {
                Exit.first = i;
                Exit.second = len+1-j+temp;
            }
        }
    }
    // 미로 - 참가자 위치
    // 행렬 변환 
    queue <int> part;
    for (int i = x; i <= x+len-1; i++) {
        for (int j = y; j <= y+len-1; j++) {    
            part.push(r_maze[i][j]);    
        }
    }
    for (int i = y; i <= y+len-1; i++) {
        for (int j = x; j <= x+len-1; j++) {    
            int temp = part.front();
            part.pop();
            temp_r_maze[j][i] = temp;    
        }
    }
    // 열 스왑하기
    for (int i = x; i <= x+len-1; i++) {
        for (int j = y; j <= y+len-1; j++) {
            int temp = y;
            if (y == 1)
                temp = 0;
            r_maze[i][len+1-j+temp] = temp_r_maze[i][j];
            
            if (r_maze[i][len+1-j+temp] > 0) {
                runner[r_maze[i][len+1-j+temp]][0] = i;
                runner[r_maze[i][len+1-j+temp]][1] = len+1-j+temp;
            }
        }
    }
}