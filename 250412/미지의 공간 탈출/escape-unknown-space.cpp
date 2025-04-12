#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

// 방향: 동(0), 서(1), 남(2), 북(3)
const int dy[] = {0, 0, 1, -1};
const int dx[] = {1, -1, 0, 0};

int n, m, f;
int land[6][24][24];            // 각 면의 맵 정보 (0=빈칸, 1=장애물, 2=타임머신, 3=언덕, 4=출구)
int fire_dist[6][24][24];       // 불이 도달하는 시간
int machine_dist[6][24][24];    // 타임머신이 도달하는 시간

int mountain_start_y, mountain_start_x;  // 언덕 시작 위치

struct State {
  int dim, y, x;  // 현재 차원, y좌표, x좌표
};

struct Fire {
  int y, x, d, v; // 시작 위치 y,x / 방향 d / 속도 v
};

Fire fire[14];
State timeMachine, exitPoint;

/// 유효 좌표 체크 함수
bool is_out(int dim, int y, int x) {
  int len = (dim == 5) ? n : m;
  return (y < 0 || y >= len || x < 0 || x >= len);
}

/// 경계를 벗어날 경우 dimension 이동 처리 (핵심 인터페이스 로직)
State change_dim(int dim, int y, int x) {
  State newState = {-1, -1, -1};
  int len = m;  // 벽은 m x m 크기

  switch (dim) {
    // 각 면에 대한 경계 넘는 경우 구현
    // 예: 동쪽면에서 동쪽 벗어나면 -> 북쪽면으로 전환
    case 0:  // 동쪽
      if (x == len) newState = {3, y, 0}; // 동의 동쪽 -> 북
      if (x < 0) newState = {2, y, len - 1}; // 동의 서쪽 -> 남
      if (y == len) newState = {5, mountain_start_y + (len - 1 - x), mountain_start_x + len};
      if (y < 0) newState = {4, len - 1 - x, len - 1}; // 동의 북쪽 -> 위
      break;
    case 1:  // 서쪽
      if (x == len) newState = {2, y, 0};
      if (x < 0) newState = {3, y, len - 1};
      if (y == len) newState = {5, mountain_start_y + x, mountain_start_x - 1};
      if (y < 0) newState = {4, x, 0};
      break;
    case 2:  // 남쪽
      if (x == len) newState = {0, y, 0};
      if (x < 0) newState = {1, y, len - 1};
      if (y == len) newState = {5, mountain_start_y + len, mountain_start_x + x};
      if (y < 0) newState = {4, len - 1, x};
      break;
    case 3:  // 북쪽
      if (x == len) newState = {1, y, 0};
      if (x < 0) newState = {0, y, len - 1};
      if (y == len) newState = {5, mountain_start_y - 1, mountain_start_x + (len - 1 - x)};
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

/// dir 방향으로 이동 시 다음 위치 구하기
State go_next(int dim, int y, int x, int dir) {
  int ny = y + dy[dir];
  int nx = x + dx[dir];

  if (dim >= 0 && dim <= 4) {
    // 벽면에서 경계 벗어났을 때 차원 전환
    if (is_out(dim, ny, nx)) 
        return change_dim(dim, ny, nx);
    else 
        return {dim, ny, nx};
  } else {
    // 평면에서는 벽으로 "올라가는" 이동 안 됨
    if (is_out(dim, ny, nx) || land[dim][ny][nx] == 3) 
        return {dim, y, x};
    else 
        return {dim, ny, nx};
  }
}

/// 불의 BFS (실제로는 직선 전파 시뮬레이션)
void fire_bfs() {
  memset(fire_dist, -1, sizeof(fire_dist));
  queue<Fire> q;

  for (int i = 0; i < f; i++) {
    q.push(fire[i]);
    fire_dist[5][fire[i].y][fire[i].x] = 0;
  }

  while (!q.empty()) {
    Fire here = q.front(); q.pop();
    State next = go_next(5, here.y, here.x, here.d);

    // 이동 불가 (장애물, 경계, 언덕 위)
    if (here.y == next.y && here.x == next.x) continue;
    if (land[5][next.y][next.x] != 0) continue;

    int nextTime = fire_dist[5][here.y][here.x] + here.v;

    if (fire_dist[5][next.y][next.x] == -1 || fire_dist[5][next.y][next.x] > nextTime) {
      fire_dist[5][next.y][next.x] = nextTime;
      q.push({next.y, next.x, here.d, here.v});
    }
  }
}

/// 타임머신 BFS
void machine_bfs() {
  memset(machine_dist, -1, sizeof(machine_dist));
  queue<State> q;

  q.push(timeMachine);
  machine_dist[timeMachine.dim][timeMachine.y][timeMachine.x] = 0;

  while (!q.empty()) {
    State here = q.front(); 
    q.pop();
    int curTime = machine_dist[here.dim][here.y][here.x];

    for (int dir = 0; dir < 4; dir++) {
        State next = go_next(here.dim, here.y, here.x, dir);
        int ndim = next.dim, ny = next.y, nx = next.x;

        // 장애물 또는 이미 방문
        if (land[ndim][ny][nx] == 1 || machine_dist[ndim][ny][nx] != -1) 
            continue;

        // 불보다 늦게 도착하는 경우
        if (ndim == 5 && fire_dist[ndim][ny][nx] != -1 && fire_dist[ndim][ny][nx] <= curTime + 1)
            continue;

        machine_dist[ndim][ny][nx] = curTime + 1;
        q.push(next);
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  cin >> n >> m >> f;

  // 평면 입력
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      cin >> land[5][i][j];
      if (land[5][i][j] == 4) exitPoint = {5, i, j};
    }

  // 벽면 입력
  for (int d = 0; d < 5; d++)
    for (int i = 0; i < m; i++)
      for (int j = 0; j < m; j++) {
        cin >> land[d][i][j];
        if (land[d][i][j] == 2) timeMachine = {d, i, j};
      }

  // 불 입력
  for (int i = 0; i < f; i++)
    cin >> fire[i].y >> fire[i].x >> fire[i].d >> fire[i].v;

  // 언덕 시작 위치 저장
  for (int i = 0; i < n; i++) {
    bool found = false;
    for (int j = 0; j < n; j++) {
      if (land[5][i][j] == 3) {
        mountain_start_y = i;
        mountain_start_x = j;
        found = true;
        break;
      }
    }
    if (found) break;
  }

  // 시뮬레이션 수행
  fire_bfs();
  machine_bfs();

  // 정답 출력
  cout << machine_dist[exitPoint.dim][exitPoint.y][exitPoint.x] << '\n';
}