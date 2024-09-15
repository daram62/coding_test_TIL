#include <iostream>
#include <algorithm>
using namespace std;

int main() {

    int n, l;
    cin >> n >> l;

    float term[n];

    // 구간 중심점 입력
    for (int i = 0; i < n; i++) {
        cin >> term[i];
    }    

    // 정렬
    sort(term, term+n);
    
    float cover = term[0] - 0.5 + l;
    int cnt = 1;
    
    for (int i = 1; i < n; i++) {
        if (cover < term[i] + 0.5) {
            cnt ++;
            cover = term[i] - 0.5 + l;
        }
    }

    cout << cnt;
    return 0;
}