#include <iostream>
using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    int age[2];
    char gender[2];

    cin >> age[0] >> gender[0];
    cin >> age[1] >> gender[1];

    if (age[0] >= 19 && gender[0] == 'M'){
        cout << 1;
    }
    else if (age[1] >= 19 && gender[1] == 'M') {
        cout << 1;
    }
    else {
        cout << 0;
    }

    return 0;
}