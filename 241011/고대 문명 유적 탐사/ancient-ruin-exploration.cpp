#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
#include <algorithm>

#define X first
#define Y second

using namespace std;
struct Dig {
	int value;
	int x;
	int y;
	int rollcnt;
};
bool cmp(Dig& a,Dig& b){
	if (a.value != b.value) {
		return a.value > b.value;
	}
	else {
		if (a.rollcnt != b.rollcnt) return a.rollcnt < b.rollcnt;
		else if (a.y != b.y) return a.y < b.y;
		else return a.x < b.x;
	}
}
bool cmp2(pair<int,int> a, pair<int, int> b) {
	if (a.second != b.second) return a.second < b.second;
	else return a.first > b.first;
}
int area[6][6];
bool vis[6][6];
int wall[301];
int k, m;
int wall_ptr = 0;
bool isDone = 0;
int total = 0;

int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 };

bool isOut(int x,int y) {
	return (x < 0 || y < 0 || x >= 5 || y >= 5);
}
void initVis() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			vis[i][j] = 0;
		}
	}
}
void roll(int x,int y) {
	int tmp[6][6];
	memmove(tmp, area, sizeof(area));
	area[x - 1][y - 1] = tmp[x + 1][y - 1]; area[x -1][y ] = tmp[x ][y - 1]; area[x - 1][y + 1] = tmp[x - 1][y - 1];
	area[x ][y - 1] = tmp[x + 1][y]; area[x][y] = tmp[x][y]; area[x][y + 1] = tmp[x- 1][y];
	area[x + 1][y - 1] = tmp[x + 1][y +1]; area[x+1][y] = tmp[x][y + 1]; area[x + 1][y + 1] = tmp[x - 1][y + 1];
}

int getting() {
	int size = 0;
	queue<pair<int, int>> q;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (vis[i][j]) continue;
			int piece = 1;
			q.push({ i,j });
			vis[i][j] = 1;		
			while (!q.empty()) {
				pair<int, int> cur = q.front(); q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];
					if (isOut(nx, ny)) continue;
					if (vis[nx][ny]) continue;
					if (area[nx][ny] != area[cur.X][cur.Y]) continue;
					piece++;
					vis[nx][ny] = 1;
					q.push({ nx,ny });
				}
			}
			if (piece >= 3) {
				size += piece;
				isDone = 0;
			}
		}
	}
	return size;
}
vector<pair<int, int>> gettropy() {
	int size = 0;
	vector<pair<int, int>> ret;
	queue<pair<int, int>> q;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (vis[i][j]) continue;
			vector<pair<int, int>> save;
			int piece = 1;
			q.push({ i,j });
			vis[i][j] = 1;
			save.push_back({ i,j });
			while (!q.empty()) {
				pair<int, int> cur = q.front(); q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];
					if (isOut(nx, ny)) continue;
					if (vis[nx][ny]) continue;
					if (area[nx][ny] != area[cur.X][cur.Y]) continue;
					piece++;
					vis[nx][ny] = 1;
					q.push({ nx,ny });
					save.push_back({ nx,ny });
				}
			}
			if (piece >= 3) {
				size += piece;
				for (auto z : save)
					ret.push_back({ z.first,z.second });
			}
		}
	}
	if (size == 0) isDone = 1;
	initVis();
	return ret;
}
void explore() {
	vector<Dig> v;
	int tmp[6][6];
	memcpy(tmp, area, sizeof(area));
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			int maxvalue = 0; int maxi = 0; int maxj = 0; int maxrollcnt = 0;
			for (int z = 0; z < 3; z++) {
				if (z == 0) {
					roll(i, j);
					int sum = getting();
					if (maxvalue < sum) {
						maxvalue = sum;
						maxi = i;
						maxj = j;
						maxrollcnt = 1;
					}
					initVis();
				}
				else if (z == 1) {
					roll(i, j);
					int sum = getting();
					if (maxvalue < sum) {
						maxvalue = sum;
						maxi = i;
						maxj = j;
						maxrollcnt = 2;
					}
					initVis();
				}
				else {
					roll(i, j);
					int sum = getting();
					if (maxvalue < sum) {
						maxvalue = sum;
						maxi = i;
						maxj = j;
						maxrollcnt = 3;
					}
					initVis();
				}
			}
			Dig dummy;
			dummy.value = maxvalue;
			dummy.x = maxi;
			dummy.y = maxj;
			dummy.rollcnt = maxrollcnt;
			if (maxvalue > 0) v.push_back(dummy);

			memcpy(area,tmp,sizeof(area));
		}
	}
	if (v.size() == 0) {
		isDone = 1;
		return;
	}
	memcpy(area, tmp, sizeof(area));
	sort(v.begin(), v.end(), cmp);
	// 첫번째가 제일 큰것
	Dig& bigone = v.front();
	if (bigone.rollcnt == 1) {
		roll(bigone.x, bigone.y);
	}
	else if (bigone.rollcnt == 2) {
		roll(bigone.x, bigone.y);
		roll(bigone.x, bigone.y);
	}
	else if (bigone.rollcnt == 3) {
		roll(bigone.x, bigone.y);
		roll(bigone.x, bigone.y);
		roll(bigone.x, bigone.y);
	}

	vector<pair<int, int>> result = gettropy();
	sort(result.begin(), result.end(), cmp2);// 정렬 그냥 이 순서로 채우면 됨 
	total += result.size();
	// 벽 채우기
	for (auto z : result) {
		area[z.first][z.second] = wall[wall_ptr];
		wall_ptr++;
	}

}

void chain() {
	vector<pair<int, int>> result = gettropy();
	sort(result.begin(), result.end(), cmp2);// 정렬 그냥 이 순서로 채우면 됨 
	total += result.size();
	// 벽 채우기
	for (auto z : result) {
		area[z.first][z.second] = wall[wall_ptr];
		wall_ptr++;
	}
}
int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> k >> m;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> area[i][j];
		}
	}
	for (int i = 0; i < m; i++)
		cin >> wall[i];

	while (k--) {
		total = 0;
		explore();
		if (isDone) break;
		while (!isDone) {
			chain();
		}
		cout << total << " ";
	}
	return 0;
}