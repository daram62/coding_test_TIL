#include <iostream>
#include <queue>
#include <vector>
#include <functional>

using namespace std;
#define MAX_NUM 100000

int main() {

    int n, num;
    int sum, score = 0; 
    cin >> n;

    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int i = 0; i < n; i++) {
        cin >> num;
        minHeap.push(num);
    }

    int fir, sec;

    for (int i = 0; i < n-1; i++) {
        fir = minHeap.top();
        minHeap.pop();

        sec = minHeap.top();
        minHeap.pop();
        
        sum = fir + sec;
        score += sum;

        minHeap.push(sum);
    }

    cout << score;
    return 0;
}