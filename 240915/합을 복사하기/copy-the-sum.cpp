#include <iostream>
using namespace std;

int main() {
    int a, b, c, sum;

    a = 1;
    b = 2;
    c = 3;
    sum = a+b+c;

    a = sum;
    b = sum;
    c = sum;

    cout << a << " " << b << " " << c << endl;

    return 0;
}