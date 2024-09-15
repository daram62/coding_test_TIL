#include <iostream>
using namespace std;

int main() {

    int n, l;
    
    cin >> n >> l;

    int term[n];

    for (int i = 0; i < n; i++) {
        cin >> term[i];
    }    
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