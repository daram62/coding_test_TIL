#include <iostream>
using namespace std;
#define MAX_NUM 200
int n, m, s;
char temp;

void TestPrint(int **array) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << array[i][j];
        }
        cout << endl;
    }
}

void PrintArray(int **array) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (array[i][j] <= 0) {
                cout << '.';
            }
            else {
                cout << 'O';
            }
        }
        cout << endl;
    }

}

void DeleteArray(int **array) {
    for (int i = 0; i < n; i++) {
        delete[] array[i];
    }
    delete[] array;
}

void explode(int **new_array, int i, int j) {
    new_array[i][j] = -1;                
    if (i > 0) 
        new_array[i-1][j] = -1;
    if (j > 0) 
        new_array[i][j-1] = -1;               
    if (j < m - 1) 
        new_array[i][j+1] = -1;
    if (i < n - 1) 
        new_array[i+1][j] = -1;            
}

int main() {
    // 여기에 코드를 작성해주세요.

    cin >> n >> m >> s;

    int **array = new int*[n];
    for (int i = 0; i < n; i++) {
        array[i] = new int[m];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> temp;
            if (temp == '.') {
                array[i][j] = -1;
            }
            else if (temp == 'O') {
                array[i][j] = 0;
            }
        }
    }

    for (int i = 1; i <= s; i++) {
        //cout << i << "초 후" << endl;

        int **new_array = new int*[n];
        for (int i = 0; i < n; i++) {
            new_array[i] = new int[m];
            for (int j = 0; j < m; j++) {
                new_array[i][j] = array[i][j];
            }   
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                array[i][j]++;
                new_array[i][j]++;

                if (array[i][j] == 3) {
                    explode(new_array, i, j);
                }
            }
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                array[i][j] = new_array[i][j];
            }
        }
        //TestPrint(array);
    }
  
    //TestPrint(array);
    PrintArray(array);
    DeleteArray(array);

    return 0;
}