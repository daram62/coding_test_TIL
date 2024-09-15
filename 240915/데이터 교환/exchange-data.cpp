#include <iostream>
using namespace std;

int main() {
    int a, b, c, temp;

    a = 5;
    b = 6;
    c = 7;
    temp = 0;

    temp = b;
    b = a;
    a = c;    
    c = temp;

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    
    return 0;
}