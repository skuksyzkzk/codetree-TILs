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
		int x = trees[i].first; int y = trees[i].second;
		int num = tree[x][y];//그루수
		int arround = 4;// 성장치 
		// 성장시 4방향 확인 
		for (int dir = 0; dir < 4; dir++) {
			int nx = x + grow_dx[dir];
			int ny = y + grow_dy[dir];
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
				arround--;
				continue;
			}
			// 제초 or 벽 있을시 안됨 
			if (tree[nx][ny] <= 0) {
				arround--;
				continue;
			}
		}
		// 성장
		tree[x][y] += arround;
        // MAX 그루수가 100이니까 
        if (tree[x][y] > MAX) tree[x][y] = MAX;
	}

}
void spread() {
	for (int i = 0; i < trees.size(); i++) {
		int x = trees[i].first; int y = trees[i].second;
		// 번식 탐색
		for (int dir = 0; dir < 4; dir++) {
			int nx = x + grow_dx[dir];
			int ny = y + grow_dy[dir];
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			// 제초(-2) or 벽 있을시 안됨 or 나무있어도 
			if (tree[nx][ny] < 0 || tree[nx][ny] > 0) continue;
			if (tree[nx][ny] == 0) dirv[i].push_back(dir);

		}
	}
	// 번식은 동시에 해야하므로 
	for (int i = 0; i < trees.size(); i++) {
		int x = trees[i].first; int y = trees[i].second;
		for (int dir = 0; dir < dirv[i].size(); dir++) {
			int nx = x + grow_dx[dirv[i][dir]];
			int ny = y + grow_dy[dirv[i][dir]];
			trees.push_back({ nx,ny });
			tree[nx][ny] += (tree[x][y] / dirv[i].size());
            // MAX 그루수가 100이니까 
            if (tree[nx][ny] > MAX) tree[nx][ny] = MAX;
		}
		// 번식후 다시 번식 배열초기화
		dirv[i].erase(dirv[i].begin(), dirv[i].end());


	}
}
void kill() {
	// 안죽였을때 or 새롭게 제초 뿌려야될때
	vector<Info> killsave; // 제초할 칸 탐색 저장용
	for (int i = 0; i < trees.size(); i++) {
		int x = trees[i].first; int y = trees[i].second;
		// 제초 
		int killcnt = tree[x][y];
		for (int dir = 0; dir < 4; dir++) {
			int tmpx = trees[i].first; int tmpy = trees[i].second;
			for (int j = 0; j < k; j++) {
				int nx = tmpx + kill_dx[dir];
				int ny = tmpy + kill_dy[dir];
				tmpx = nx;
				tmpy = ny;
				if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;
				// 벽 빈자리 일 경우 컷 
				if (tree[nx][ny] <= 0) break;
				killcnt += tree[nx][ny];
			}
		}
		killsave.push_back({ killcnt,x,y });
		killcnt=0;
	}
	// 여기서 내림차순으로 정렬
	sort(killsave.begin(), killsave.end(), cmp);

	int delete_x = killsave.front().x;
	int delete_y = killsave.front().y;
	total_kill_cnt += killsave.front().count;
	killarr.push_back({ delete_x, delete_y });
	for (int dir = 0; dir < 4; dir++) {
		int tmpx = killsave.front().x; int tmpy = killsave.front().y;
		// 대각 방향으로 각 2칸씩 가야하니까
		for (int j = 0; j < k; j++) {
			int nx = tmpx + kill_dx[dir];
			int ny = tmpy + kill_dy[dir];
			tmpx = nx;
			tmpy = ny;
			// 범위 범어가도 안되고 
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;
			// 제초 or 벽 있을시 안됨 
			if (tree[nx][ny] <= 0) break;
			// 나무가 있다는 것 
			tree[nx][ny] = -2; // 제초 뿌림
			killflag = 1;
			// 죽인 나무는 나무벡터에서 삭제
			trees.erase(
				remove(trees.begin(), trees.end(), make_pair(nx, ny))
				, trees.end()
			);
			//제초 자리 기억 - 기간이 끝나면 바로 그 자리 0으로
			killarr.push_back({ nx,ny });
		}
	}


}
// 제초자리 다시 0으로 초기화
void killinit() {
	for (auto nxt : killarr) {
		tree[nxt.x][nxt.y] = 0;
	}
}
int main() {
	// 여기에 코드를 작성해주세요.
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> n >> m >> k >> c;
	// 초기화 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> tree[i][j];
			if (tree[i][j] >0) {
				trees.push_back({ i,j });
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
			// 킬초
			killinit();
		}
		if (!killflag) kill();
		expire--; 
	}
	cout << total_kill_cnt;
	return 0;
}