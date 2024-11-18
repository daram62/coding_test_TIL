#include <string>
#include <vector>

using namespace std;

vector<long long> solution(int x, int n) {
    int temp = 0;
    vector<long long> answer;
    
    for (int i = 1; i <= n; i++) {
        temp = x*i;
        answer.push_back(temp);
    }
    
    return answer;
}