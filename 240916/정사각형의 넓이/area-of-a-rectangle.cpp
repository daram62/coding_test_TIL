#include <iostream>
using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    int n, area;

    cin >> n;

    if (n < 5)
        cout << "tiny";
    else {
        area = n * n;
        cout << area;
    }
    return 0;
}