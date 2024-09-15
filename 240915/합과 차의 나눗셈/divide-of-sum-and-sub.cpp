#include <iostream>
using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    int a, b, sum, diff;
    float result;

    scanf("%d %d", &a, &b);

    sum = a+b;
    diff = a-b;

    result = (float) sum / diff;

    printf("%.2f", result);
    return 0;
}