#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>
using namespace std;
int N, M, F;
int land[6][24][24];
int fire_dist[6][24][24];
int machine_dist[6][24][24];
int mt_start_x, mt_start_y;
int dx[4] = {1, -1, 0, 0}; // 동서남북
int dy[4] = {0, 0, 1, -1};

struct State {
    int dim, y, x;
};

struct Fire {
    int y, x, d, v;
};

Fire fire[14];
State timemachine, exitpoint;

bool is_out(int dim, int y, int x) {
    int len = 0;
    if (dim == 5)
        len = N;
    else 
        len = M;
    return (y < 0 || y >= len || x < 0 || x >= len);
}

State change_dim(int dim, int y, int x) {
    State newState = {-1, -1, -1};
    int len = M;

    switch (dim) {
        // 각 면마다 차원 바꾸는거 구현
        case 0: // 동쪽
            if (x == len) newState = {3, y, 0};
            if (x < 0) newState = {2, y, len - 1};
            if (y == len) newState = {5, mt_start_y + (len - 1 - x), mt_start_x + len};
            if (y < 0) newState = {4, len - 1 - x, len - 1};
            break;
        case 1: // 서쪽
            if (x == len) newState = {2, y, 0};
            if (x < 0) newState = {3, y, len - 1};
            if (y == len) newState = {5, mt_start_y + x, mt_start_x - 1};
            if (y < 0) newState = {4, x, 0};
            break;
        case 2: // 남쪽
            if (x == len) newState = {0, y, 0};
            if (x < 0) newState = {1, y, len - 1};
            if (y == len) newState = {5, mt_start_y + len, mt_start_x + x};
            if (y < 0) newState = {4, len - 1, x};
            break;    
        case 3:  // 북쪽
            if (x == len) newState = {1, y, 0};
            if (x < 0) newState = {0, y, len - 1};
            if (y == len) newState = {5, mt_start_y - 1, mt_start_x + (len - 1 - x)};
            if (y < 0) newState = {4, 0, len - 1 - x};
            break;
        case 4:  // 위쪽
            if (x == len) newState = {0, 0, len - 1 - y};
            if (x < 0) newState = {1, 0, y};
            if (y == len) newState = {2, 0, x};
            if (y < 0) newState = {3, 0, len - 1 - x};
            break;
    }
    return newState;
}

State go_next(int dim, int y, int x, int dir) {
    int ny = y + dy[dir];
    int nx = x + dx[dir];

    if (dim >= 0 && dim <= 4) {
        // 벽면 벗어났을 때
        if (is_out(dim, ny, nx))
            return change_dim(dim, ny, nx);
        else
            return {dim, ny, nx};
    }
    else {
        // 평면에서 벽으로는 안가짐
        if (is_out(dim, ny, nx) || land[dim][ny][nx] == 3)
            return {dim, y, x};
        else 
            return {dim, ny, nx};
    }
}

void fire_bfs() {
    memset(fire_dist, -1, sizeof(fire_dist));
    queue<Fire> q;
    
    for (int i = 0; i < F; i++) {
        q.push(fire[i]);
        fire_dist[5][fire[i].y][fire[i].x] = 0;
    }

    while (!q.empty()) {
        Fire here = q.front();
        q.pop();
        State next = go_next(5, here.y, here.x, here.d);

        // 이동 불가 체크
        if (here.y == next.y && here.x == next.x)
            continue;
        if (land[5][next.y][next.x] != 0) 
            continue;

        int nexttime = fire_dist[5][here.y][here.x] + here.v;

        if (fire_dist[5][next.y][next.x] == -1 || fire_dist[5][next.y][next.x] > nexttime) {
            fire_dist[5][next.y][next.x] = nexttime;
            q.push({next.y, next.x, here.d, here.v});
        }
    }
}

void machine_bfs() {
    memset(machine_dist, -1, sizeof(machine_dist));
    queue<State> q;
    q.push(timemachine);
    machine_dist[timemachine.dim][timemachine.y][timemachine.x] = 0;

    while (!q.empty()) {
        State here = q.front();
        q.pop();
        int curtime = machine_dist[here.dim][here.y][here.x];

        for (int dir = 0; dir < 4; dir++) {
            State next = go_next(here.dim, here.y, here.x, dir);
            int ndim = next.dim, ny = next.y, nx = next.x;
            
            // 장애물, 방문 체크
            if (land[ndim][ny][nx] == 1 || machine_dist[ndim][ny][nx] != -1)
                continue;

            // 불보다 늦게 도착하면
            if (ndim == 5 && fire_dist[ndim][ny][nx] != -1 && fire_dist[ndim][ny][nx] <= curtime + 1)  
                continue;

            machine_dist[ndim][ny][nx] = curtime + 1;
            q.push(next);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> N >> M >> F;

    // 평면
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> land[5][i][j];
            if (land[5][i][j] == 4)
                exitpoint = {5, i, j};
        }
    }
    // 벽면
    for (int d = 0; d < 5; d++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                cin >> land[d][i][j];
                if (land[d][i][j] == 2)
                    timemachine = {d, i, j};
            }
        }
    }
    // 불
    for (int i = 0; i < F; i++) {
        cin >> fire[i].y >> fire[i].x >> fire[i].d >> fire[i].v;
    }

    // 언덕 시작 위치
    for (int i = 0; i < N; i++) {
        bool found = false;
        for (int j = 0; j < N; j++) {
            if (land[5][i][j] == 3) {
                mt_start_y = i;
                mt_start_x = j;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    fire_bfs();
    machine_bfs();

    cout << machine_dist[exitpoint.dim][exitpoint.y][exitpoint.x] << endl;
}
