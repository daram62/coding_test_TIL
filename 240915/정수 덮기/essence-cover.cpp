#include <iostream>
#include <algorithm>
using namespace std;

int main() {

    int n, l;
    
    cin >> n >> l;

    int term[n];

    // 구간 중심점 입력
    for (int i = 0; i < n; i++) {
        cin >> term[i];
    }    

    // 정렬
    sort(term, term+n);

    term[n-1] += 1;

    int start = 0, cnt = 0;
    
    for (int i = 0; i < n; i++) {
        if (start < term[i]) {
            start = term[i];
            cnt ++;
            start += 3;
        }
    }

    cout << cnt;
    return 0;
}