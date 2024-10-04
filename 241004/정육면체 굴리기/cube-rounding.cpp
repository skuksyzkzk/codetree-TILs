#include <iostream>
using namespace std;
int board[22][22];

int dx[4] = {0,0,-1,1};
int dy[4] = {1,-1,0,0};

int main() {
    // 여기에 코드를 작성해주세요.
    int n {0},m{0};
    int x {0},y{0},k{0};
    cin >> n >> m >> x >> y >> k;

    int dice[7] ={-1,0,0,0,0,0,0};
    int tmp[7] = {-1,0,0,0,0,0,0};

    for (int i = 0;i<n;i++){
        for (int j = 0; j<m;j++){
            cin >> board[i][j];
        }
    }
    // 처음 설정
    if (board[x][y] != 0) {
        dice[1] = board[x][y];
        board[x][y] = 0;
    }

    // 그 다음 시작
    for (int q = 0 ; q < k; q++){
        int dir;
        cin >> dir;
        copy(begin(dice),end(dice),tmp);

        if (dir == 1){
            if (x+dx[0] < 0 || x+dx[0] >= n || y+dy[0] < 0 || y+dy[0] >= m) continue;
            dice[1] = tmp[3];
            dice[3] = tmp[6];
            dice[5] = tmp[1];
            dice[6] = tmp[5];

            if (board[x+dx[0]][y+dy[0]] == 0 ){
                board[x+dx[0]][y+dy[0]] = dice[1];
            }
            else {
                dice[1] = board[x+dx[0]][y+dy[0]];
                board[x+dx[0]][y+dy[0]] = 0;
            }
            x = x+ dx[0];
            y = y+ dy[0];
        }
        else if (dir == 2){
            if (x+dx[1] < 0 || x+dx[1] >= n || y+dy[1] < 0 || y+dy[1] >= m) continue;
            dice[1] = tmp[5];
            dice[3] = tmp[1];
            dice[5] = tmp[6];
            dice[6] = tmp[3];
            if (board[x+dx[1]][y+dy[1]] == 0 ){
                board[x+dx[1]][y+dy[1]] = dice[1];
            }
            else {
                dice[1] = board[x+dx[1]][y+dy[1]];
                board[x+dx[1]][y+dy[1]] = 0;
            }
            x = x+ dx[1];
            y = y+ dy[1];
        }
        else if (dir == 3) {
            if (x+dx[2] < 0 || x+dx[2] >= n || y+dy[2] < 0 || y+dy[2] >= m) continue;
            dice[1] = tmp[4];
            dice[2] = tmp[1];
            dice[4] = tmp[6];
            dice[6] = tmp[2];
            if (board[x+dx[2]][y+dy[2]] == 0 ){
                board[x+dx[2]][y+dy[2]] = dice[1];
            }
            else {
                dice[1] = board[x+dx[2]][y+dy[2]];
                board[x+dx[2]][y+dy[2]] = 0;
            }
            x = x+ dx[2];
            y = y+ dy[2];
        }
        else { // dir == 4
            if (x+dx[3] < 0 || x+dx[3] >= n || y+dy[3] < 0 || y+dy[3] >= m) continue;
            dice[1] = tmp[2];
            dice[2] = tmp[6];
            dice[4] = tmp[1];
            dice[6] = tmp[4];
            if (board[x+dx[3]][y+dy[3]] == 0 ){
                board[x+dx[3]][y+dy[3]] = dice[1];
            }
            else {
                dice[1] = board[x+dx[3]][y+dy[3]];
                board[x+dx[3]][y+dy[3]] = 0;
            }
            x = x+ dx[3];
            y = y+ dy[3];
        }
        
        cout << dice[6] << "\n";
    }
    return 0;
}