#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> graph[101];
bool visited[101];
int cnt = 0;

void DFS(int node) {
    visited[node] = true;
    
    for (int i = 0; i < graph[node].size(); i++) {
        int next = graph[node][i];
        if (!visited[next]) {
            //cout << node << i << next << endl;
            cnt++;
            DFS(next);
        }
    }
}
int main() {
    // 컴퓨터의 수, 연결된 컴퓨터 쌍의 수
    int n, m;
    cin >> n >> m;

    // 그래프 입력
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    // 정렬
    for (int i = 1; i <= n; i++) {
        sort(graph[i].begin(), graph[i].end());
    }

    // 1번 컴퓨터부터 바이러스 감염 시작
    DFS(1);
    
    cout << cnt << endl;

    return 0;
}