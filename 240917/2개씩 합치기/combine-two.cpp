#include <iostream>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

int main() {
    long long int n, num;
    long long int sum, score = 0; 
    cin >> n;

    priority_queue<int, vector<int>, greater<int>> minHeap;

    // 숫자 입력
    for (int i = 0; i < n; i++) {
        cin >> num;
        minHeap.push(num);
    }

    // 총 n-1번 합치는 작업을 수행
    for (int i = 0; i < n - 1; i++) {
        int fir = minHeap.top();
        minHeap.pop();

        int sec = minHeap.top();
        minHeap.pop();

        sum = fir + sec;
        score += sum;

        minHeap.push(sum);
    }

    cout << score;
    return 0;
}