#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

int n, m, v;
vector<int> graph[1001];
bool visited[1001];

void DFS(int start) {
    visited[start] = true;
    cout << start << ' ';
    
    for(int i = 0; i < graph[start].size(); i++) {
        int next = graph[start][i];
        if (!visited[next]) {
            DFS(next);
        }
    }
}

void BFS(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    
    while(!q.empty()) {
        int cur = q.front();
        q.pop();
        cout << cur << ' ';

        for (int i = 0; i < graph[cur].size(); i++) {
            int next = graph[cur][i];
            if (!visited[next]) {
                q.push(next);
                visited[next] = true;
            }
        }
    }
}

int main() {
    cin >> n >> m >> v;

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }    

    for (int i = 1; i <= n; i++) {
        sort(graph[i].begin(), graph[i].end());
    }

    DFS(v);
    cout << '\n';

    fill(visited, visited + 1001, false);

    BFS(v);

    return 0;
}