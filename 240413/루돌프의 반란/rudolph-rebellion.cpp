// 삼성 SW 역량 테스트 2023년 오후반 1번 문제
// 루돌프의 반란
#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>
using namespace std;

// 7번째 턴에서 이상해
int N, M; // 게임판 크기, 턴 횟수
int P, C, D; // 산타 수, 루돌프 힘, 산타 힘 
int score[32]; // 산타 점수 저장 배열

pair<int, int> rudol; // 루돌프 위치
map<int,int> santa[32]; // 산타 위치 저장 배열

int board[52][52]; // 게임판 
bool isSanta[32]; // 산타 탈락 여부
int isFaint[32]; // 산타 기절 여부 

// 서남동북 1234대각선 방향 순으로 이동 좌표 저장
int dx[8] = {0,1,0,-1,-1,-1,1,1};
int dy[8] = {-1,0,1,0,1,-1,-1,1};

void debug() {
    cout << "루돌프 위치 " << rudol.first << ", " << rudol.second << endl;
    for (int i = 1; i <= P; i++)
    {
        cout << i << "번째 산타 위치 " << santa[i][0] << ", " << santa[i][1] << endl;
    }
}

void Print() {
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void interaction(int index, int direct) {
    for (int i = 1; i <= P; i++)
    {
        if (i == index)
            continue;
        if (santa[i][0] == santa[index][0] && santa[i][1] == santa[index][1])
        {
            //cout << i << "번째 산타와 상호작용 발생" << endl;
            santa[i][0] += dx[direct];
            santa[i][1] += dy[direct];
            if (santa[i][0] < 1 || santa[i][0] > N || santa[i][1] < 1 || santa[i][1] > N)
            {
                //cout << i << "번째 산타 탈락" << endl;
                isSanta[i] = false;
            }
            else 
            {
                board[santa[i][0]][santa[i][1]] = true;
                interaction(i, direct);
            }
                
        }
    }
}

// 몇번 산타가 어디로, 얼만큼 이동하는지 
void move(int index, int direct, int num)
{
    // 기존 좌표 비워주고
    board[santa[index][0]][santa[index][1]] = 0;
    
    // 방향 따라서 이동시키기
    santa[index][0] += dx[direct] * num;
    santa[index][1] += dy[direct] * num;

    // 다 밀려난 후 게임판 밖인지 체크
    if (santa[index][0] < 1 || santa[index][0] > N || santa[index][1] < 1 || santa[index][1] > N)
    { // 게임판 밖이면 
        //cout << index << "번째 산타 탈락" << endl;
        isSanta[index] = false;
    }        
    // 산타 위치 게임판에 업데이트
    else 
    {
        board[santa[index][0]][santa[index][1]] = index;
        //cout << index << "번째 산타 이동 -> " << santa[index][0] << ", " << santa[index][1] << endl;
    }
    // 상호작용 체크
    interaction(index, direct);
}

bool check() {
    bool allfail = true;
    for (int i = 1; i <= P; i++) {
        if (isSanta[i] == true)
            allfail = false;
    }
    return allfail;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> N >> M >> P >> C >> D;

    cin >> rudol.first >> rudol.second;
    board[rudol.first][rudol.second] = -1;

    for (int i = 1; i <= P; i++)
    {
        int num, r, c;
        cin >> num;
        cin >> r >> c;
        santa[num][0] = r;
        santa[num][1] = c;
        board[r][c] = num;
        isSanta[num] = true;
    }
    //debug();

    for (int T = 1; T <= M; T++)
    {
        if (check())
            break;
        // 1. 루돌프 이동
        // 거리 계산해서 목표 산타 정하기
        int min_idx;
        int min_dis = 100000;
       
        for (int i = 1; i <= P; i++)
        {
            // 탈락했거나, 기절한 산타는 빠져나가기
            if (!isSanta[i])
                continue;

            int distance;
            distance = pow(rudol.first - santa[i][0],2) + pow(rudol.second - santa[i][1],2);
            //cout << i << " 산타와의 거리 " << distance << endl;
            // 거리가 가장 가까울 때
            if (distance < min_dis)
            {
                min_dis = distance;
                min_idx = i;
            }
            // 거리가 같을 때
            else if (distance == min_dis)
            {
                // r 좌표가 더 크다면 -> 업데이트
                if (santa[i][0] > santa[min_idx][0])
                {
                    min_idx = i;
                }
                // r 좌표 동일하고, c 좌표가 더 크다면
                else if (santa[i][0] == santa[min_idx][0] && santa[i][1] > santa[min_idx][1])
                {
                    min_idx = i;
                }
            }
        } 
        //cout << "the closest santa is " << min_idx << " [" << santa[min_idx][0] << ", " << santa[min_idx][1] << "]" << endl;
        // 목표 산타 정했으면 이동 ㄱㄱ
        int ru_dir = -1; 
        board[rudol.first][rudol.second] = false;
        if (rudol.first == santa[min_idx][0]) // east, west
        {
            if (rudol.second < santa[min_idx][1]) // east move
            {
                rudol.second ++;
                ru_dir = 2;
            }    
            else // rudol.second > santa[min_idx][1] // west move
            {
                rudol.second --;
                ru_dir = 0;
            }
        }
        else if (rudol.second == santa[min_idx][1])
        {
            if (rudol.first < santa[min_idx][0]) // south move
            {
                rudol.first ++;
                ru_dir = 1;
            }    
            else // rudol > santa // north move
            {
                rudol.first --;
                ru_dir = 3;
            }   
        }
        // 1사분면 대각선 이동
        else if (rudol.first > santa[min_idx][0] && rudol.second < santa[min_idx][1])
        {
            rudol.first --;
            rudol.second ++;
            ru_dir = 4;
        }
        // 2사분면 대각선 이동
        else if (rudol.first > santa[min_idx][0] && rudol.second > santa[min_idx][1])
        {
            rudol.first --;
            rudol.second --;
            ru_dir = 5;
        }
        // 3사분면 대각선 이동
        else if (rudol.first < santa[min_idx][0] && rudol.second > santa[min_idx][1])
        {
            rudol.first ++;
            rudol.second --;
            ru_dir = 6;
        }
        // 4사분면 대각선 이동
        else if (rudol.first < santa[min_idx][0] && rudol.second < santa[min_idx][1])
        {
            rudol.first ++;
            rudol.second ++;
            ru_dir = 7;
        }
        //cout << "rudolf is " << rudol.first << ", " << rudol.second << endl;
        board[rudol.first][rudol.second] = -1;

        //Print();

        // 루돌프 충돌 체크
        for (int i = 1; i <= P; i++)
        {
            // 탈락한 산타는 바로 넘기기
            if (!isSanta[i])
                continue;
            if (rudol.first == santa[i][0] && rudol.second == santa[i][1])
            {
                //cout << i << " crash!" << endl;
                isFaint[i] = 1;
                score[i] += C;
                move(i, ru_dir, C);
            }
        }
        // 산타 이동
        int santa_dir[32] = {0,};
        for (int i = 1; i <= P; i++)
        {
            if (!isSanta[i] || isFaint[i] != 0)
                continue;
           
            // 기존 거리 체크
            int min_distance = pow(rudol.first - santa[i][0], 2) + pow(rudol.second - santa[i][1], 2);
            //cout << i << " 기존 거리 : " << min_distance << endl;

            // 움직일거니까 좌표 비워주고
            board[santa[i][0]][santa[i][1]] = 0;
            santa_dir[i] = -1;
            int nr = 0, nc = 0, ndir;
            for (int j = 0; j < 4; j++)
            {   
                // 서남동북 순으로 체크
                int r = santa[i][0] + dx[j];
                int c = santa[i][1] + dy[j];

                if (r < 1 || r > N || c < 1 || c > N)
                    continue;
                //cout << "r,c -> " << r << " , " << c << endl;
                int distance = pow(rudol.first - r, 2) + pow(rudol.second - c, 2);
                if (distance <= min_distance && board[r][c] <= 0)
                {
                    min_distance = distance;
                    //cout << j << " update " << endl; 
                    nr = r;
                    nc = c;
                    ndir = j;
                }
            }
            if (nr != 0 && nc != 0)
            {
                santa[i][0] = nr;
                santa[i][1] = nc;
                santa_dir[i] = ndir;
            }
            // 다 움직인 후에 다시 좌표 채워주기
            board[santa[i][0]][santa[i][1]] = i;
            //cout << i << "번째 산타 위치 : " << santa[i][0] << " , " << santa[i][1] << endl;
            for (int i = 1; i <= P; i++)
            {
                if (rudol.first == santa[i][0] && rudol.second == santa[i][1])
                {
                    //cout << i << "번쨰 산타와 충돌" << endl;
                    // 충돌하면, 점수 업데이트하고
                    score[i] += D;
                    // 반대 방향으로 D만큼 밀려나기
                    move(i, (santa_dir[i]+2) % 4 , D);
                    // 기절시키기
                    if (isSanta[i])
                        isFaint[i] = 1;
                }
            }
        }
//        Print();
        // 산타가 선인 충돌 체크

        // 턴 종료할 때 탈락하지 않은 산타에게 1점씩 주기
        for (int i = 1; i <= P; i++)
        {
            if (isSanta[i])
                score[i] ++;
            //cout << i << "번째 산타 점수 : " << score[i] << endl;
        }
        board[rudol.first][rudol.second] = -1;
        //cout << T << "번째 턴 후 위치 상태" << endl;
        //Print();

        if (check())
            break;
        // 마지막에 기절 풀어주기
        for (int i = 1; i <= P; i++)
        {
            if (isFaint[i] == 2)
                isFaint[i] = 0;
            else if (isFaint[i] == 1)
                isFaint[i] ++;
        }
    }
    //cout << "result -> " << endl;
    for (int i = 1; i <= P; i++)
    {
        cout << score[i] << " ";
    }
    return 0;
}