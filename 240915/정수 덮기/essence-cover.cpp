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

    int cover = term[0], cnt = 1;
    
    for (int i = 1; i < n; i++) {
        if (cover < term[i]) {
            cover = term[i];
            cnt ++;
            cover += l;
        }
    }

    cout << cnt;
    return 0;
}