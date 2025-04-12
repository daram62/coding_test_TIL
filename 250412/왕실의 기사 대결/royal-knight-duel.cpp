#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

int L, N, Q, answer;
int MAP[41][41];             // 기본 맵: 0 빈칸, 1 함정, 2 벽
int knightMAP[41][41];      // 현재 맵에 위치한 기사 번호
int visited[41][41];        // BFS 방문 여부
int dy[4] = { -1, 0, 1, 0 }; // 상우하좌
int dx[4] = { 0, 1, 0, -1 };
int isDead[31];             // 죽은 기사 체크
int damaged[31];            // 누적 피해량

struct Node { 
    int y; 
    int x; 
}; // 위치 정보용 구조체

struct knightInfo {
    int r, c, h, w, k;       // r, c: 좌상단 위치, h: 높이, w: 너비, k: 체력
};

knightInfo knightList[31];       // 기사 정보
vector<Node> query;              // 왕의 명령 (기사번호, 방향)
int pushList[31];                // 이번 명령으로 밀리는 기사 목록

// 이동 가능한지 BFS로 검사
bool check(int curNum, int dir) {
    memset(pushList, 0, sizeof(pushList));
    memset(visited, 0, sizeof(visited));

    pushList[curNum] = 1;

    queue<Node> q;
    q.push({ knightList[curNum].r, knightList[curNum].c });
    visited[knightList[curNum].r][knightList[curNum].c] = 1;

    while (!q.empty()) {
        Node now = q.front(); 
        q.pop();

        for (int i = 0; i < 4; i++) {
            int ny = now.y + dy[i];
            int nx = now.x + dx[i];

            if (i == dir) { // 이동 방향인 경우
                if (ny < 0 || nx < 0 || ny >= L || nx >= L || MAP[ny][nx] == 2) 
                    return false;
                if (visited[ny][nx] || knightMAP[ny][nx] == 0) 
                    continue;
            } 
            else { // 상우하좌 중 이동방향 아닌 곳: 같은 기사 이어붙이기
                if (ny < 0 || nx < 0 || ny >= L || nx >= L) 
                    continue;
                if (visited[ny][nx] || knightMAP[ny][nx] == 0 || knightMAP[now.y][now.x] != knightMAP[ny][nx]) 
                    continue;
            }

            pushList[knightMAP[ny][nx]] = 1;
            q.push({ ny, nx });
            visited[ny][nx] = 1;
        }
    }
    return true;
}

// 실제로 기사를 미는 함수
void push(int curNum, int dir) {
    if (!check(curNum, dir)) 
        return;

    // 밀릴 기사들의 기존 위치 지우기
    for (int i = 1; i <= N ; i++) {
        if (!pushList[i]) 
            continue;

        for (int y = knightList[i].r; y < knightList[i].r + knightList[i].h; y++)
            for (int x = knightList[i].c; x < knightList[i].c + knightList[i].w; x++)
                knightMAP[y][x] = 0;
    }

    // 밀기 및 데미지 계산
    for (int i = 1; i <= N; i++) {
        if (pushList[i] != 1) continue;

        int trapCount = 0;
        knightList[i].r += dy[dir];
        knightList[i].c += dx[dir];

        // 이동한 위치에서 함정 개수 세기
        for (int y = knightList[i].r; y < knightList[i].r + knightList[i].h; y++)
            for (int x = knightList[i].c; x < knightList[i].c + knightList[i].w; x++)
                if (MAP[y][x] == 1) 
                    trapCount++;

        // 명령받은 기사는 피해 X
        if (i != curNum) {
            damaged[i] += trapCount;
            knightList[i].k -= trapCount;
            if (knightList[i].k <= 0) {
                isDead[i] = 1;
                pushList[i] = 0;
            }
        }

        // 살아있는 기사는 맵에 다시 표시
        if (!isDead[i])
            for (int y = knightList[i].r; y < knightList[i].r + knightList[i].h; y++)
                for (int x = knightList[i].c; x < knightList[i].c + knightList[i].w; x++)
                    knightMAP[y][x] = i;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL); 
    
    // 0. 입력
    cin >> L >> N >> Q;
    // 맵 정보 입력
    for (int i = 0; i < L; i++)
        for (int j = 0; j < L; j++)
            cin >> MAP[i][j];

    // 기사 정보 입력
    for (int i = 1; i <= N; i++) {
        int r, c, h, w, k;
        cin >> r >> c >> h >> w >> k;
        knightList[i] = { r - 1, c - 1, h, w, k };

        // 기사 초기 위치 맵에 반영
        for (int y = r - 1; y < r - 1 + h; y++)
            for (int x = c - 1; x < c - 1 + w; x++)
                knightMAP[y][x] = i;
    }

    // 왕의 명령 입력
    for (int i = 0; i < Q; i++) {
        int num, dir;
        cin >> num >> dir;
        query.push_back({ num, dir });
    }

    // 1. 명령 실행
    for (int i = 0; i < query.size(); i++) {
        int curNum = query[i].y;
        int dir = query[i].x;
        
        if (isDead[curNum]) 
            continue; // 죽은 기사 무시
        push(curNum, dir);
    }

    // 2. 살아있는 기사들의 총 피해량 계산
    for (int i = 1; i <= N; i++)
        if (!isDead[i]) 
            answer += damaged[i];

    cout << answer;
    return 0;
}
