#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX = 100; // MAX 나무 그루수 
int n, m, k, c;
int tree[21][21];

// 좌상,우상,우하,좌하 대각선위치 죽일
int kill_dx[4] = { -1,-1,1,1 };
int kill_dy[4] = { -1,1,1,-1 };

int grow_dx[4] = { 0,-1,0,1 };
int grow_dy[4] = { 1,0,-1,0 };

// 죽일 나무들 탐색시 저장할 정보
struct Info {
    int count;
    int x;
    int y;
};

bool cmp(Info a, Info b) {
    if (a.count != b.count) {
        return a.count > b.count;
    }
    else {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
}

vector<pair<int, int>> trees; // 현재 나무 위치
vector<int> dirv[500]; // 번식 가능한 칸 
vector<Info> killarr; // 죽인 칸 정보 

bool killflag;
int total_kill_cnt = 0;

void grow() {
    for (int i = 0; i < trees.size(); i++) {
        int x = trees[i].first;
        int y = trees[i].second;
        int num = tree[x][y]; // 그루수
        int arround = 0; // 성장치

        // 성장시 4방향 확인
        for (int dir = 0; dir < 4; dir++) {
            int nx = x + grow_dx[dir];
            int ny = y + grow_dy[dir];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            // 제초 or 벽 있을시 안됨
            if (tree[nx][ny] > 0) {
                arround++;
            }
        }
        // 성장
        tree[x][y] += arround;
        // MAX 그루수가 100이니까
        if (tree[x][y] > MAX) tree[x][y] = MAX;
    }
}

void spread() {
    vector<pair<int, int>> new_trees;

    for (int i = 0; i < trees.size(); i++) {
        int x = trees[i].first;
        int y = trees[i].second;

        vector<pair<int, int>> empty_cells;

        // 번식 탐색
        for (int dir = 0; dir < 4; dir++) {
            int nx = x + grow_dx[dir];
            int ny = y + grow_dy[dir];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            // 제초(-2) or 벽 있을시 안됨 or 나무있어도
            if (tree[nx][ny] == 0) {
                empty_cells.push_back({ nx, ny });
            }
        }

        int count_empty = empty_cells.size();
        if (count_empty > 0) {
            int spread_amount = tree[x][y] / count_empty;
            for (auto cell : empty_cells) {
                int nx = cell.first;
                int ny = cell.second;
                tree[nx][ny] += spread_amount;
                new_trees.push_back({ nx, ny });
            }
        }
    }

    for (auto nt : new_trees) {
        trees.push_back(nt);
    }
}

void kill() {
    vector<Info> killsave;

    for (int i = 0; i < trees.size(); i++) {
        int x = trees[i].first;
        int y = trees[i].second;
        int killcnt = tree[x][y];

        for (int dir = 0; dir < 4; dir++) {
            int tmpx = x;
            int tmpy = y;

            for (int j = 0; j < k; j++) {
                int nx = tmpx + kill_dx[dir];
                int ny = tmpy + kill_dy[dir];

                if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;
                if (tree[nx][ny] <= 0) break;

                killcnt += tree[nx][ny];
                tmpx = nx;
                tmpy = ny;
            }
        }
        killsave.push_back({ killcnt, x, y });
    }

    sort(killsave.begin(), killsave.end(), cmp);

    int delete_x = killsave.front().x;
    int delete_y = killsave.front().y;
    total_kill_cnt += killsave.front().count;

    tree[delete_x][delete_y] = -2;

    for (int dir = 0; dir < 4; dir++) {
        int tmpx = delete_x;
        int tmpy = delete_y;

        for (int j = 0; j < k; j++) {
            int nx = tmpx + kill_dx[dir];
            int ny = tmpy + kill_dy[dir];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;
            if (tree[nx][ny] <= 0) break;

            tree[nx][ny] = -2;
            trees.erase(remove(trees.begin(), trees.end(), make_pair(nx, ny)), trees.end());

            tmpx = nx;
            tmpy = ny;
        }
    }
}

void killinit() {
    for (auto nxt : killarr) {
        tree[nxt.x][nxt.y] = 0;
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> n >> m >> k >> c;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> tree[i][j];
            if (tree[i][j] > 0) {
                trees.push_back({ i, j });
            }
        }
    }

    int expire = c;

    while (m--) {
        grow();
        spread();

        if (expire == 0) {
            expire = c;
            killflag = 0;
            killinit();
        }

        if (!killflag) kill();
        expire--;
    }

    cout << total_kill_cnt;

    return 0;
}