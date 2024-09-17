#include <iostream>
using namespace std;
#define MAX_NUM 200
int n, m, s;

void PrintArray(char **array) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << array[i][j];
        }
        cout << endl;
    }

}
int main() {
    // 여기에 코드를 작성해주세요.

    cin >> n >> m >> s;

    char **array = new char*[n];
    for (int i = 0; i < n; i++) {
        array[i] = new char[m];
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> array[i][j];
        }
    }

    PrintArray();

    return 0;
}