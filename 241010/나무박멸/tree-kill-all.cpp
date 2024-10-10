#include <iostream>

using namespace std;
int n, m, k, c;
int tree[21][21];
int add_tree[21][21];
int kill[21][21];

int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 };

int kill_dx[4] = { -1,-1,1,1 };
int kill_dy[4] = { -1,1,1,-1 };

int total = 0;

bool ThrowBodyOverTheWindow(int x,int y) {
	return (x < 0 || y < 0 || x >= n || y >= n);
}
void printTree() {
	cout << "Tree Print \n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << tree[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void fuck() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tree[i][j] <= 0) continue;
			int stripper = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (ThrowBodyOverTheWindow(nx,ny)) continue;
				if (tree[nx][ny] > 0) stripper++;
			}
			tree[i][j] += stripper;
		}
	}
}
void fire() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			add_tree[i][j] = 0;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tree[i][j] <= 0) continue;

			int stripper = 0;	
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (ThrowBodyOverTheWindow(nx, ny)) continue;
				if (kill[nx][ny]) continue;
				if (tree[nx][ny] == 0) stripper++;
			}

			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (ThrowBodyOverTheWindow(nx, ny)) continue;
				if (kill[nx][ny]) continue;
				if (tree[nx][ny] == 0) {
					add_tree[nx][ny] += tree[i][j] / stripper;
				}
			}

		}
		
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tree[i][j] += add_tree[i][j];
		}
	}
}
void suicide() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (kill[i][j] > 0) kill[i][j]--;
		}
	}
}
void genocide() {
	int maxcnt = 0; int maxi = 0; int maxj = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tree[i][j] <= 0) continue;
			
			int killcnt = tree[i][j];
			for (int dir = 0; dir < 4; dir++) {
				int nx = i; int ny = j;
				for (int size = 0; size < k; size++) {
					nx = nx + kill_dx[dir];
					ny = ny + kill_dy[dir];
					if (ThrowBodyOverTheWindow(nx, ny)) break;
					if (tree[nx][ny] < 0) break;
					if (tree[nx][ny] == 0) {
						break;
					}
					killcnt += tree[nx][ny];
				}
			}
			if (maxcnt < killcnt) {
				maxcnt = killcnt;
				maxi = i;
				maxj = j;
			}
				
		}
	}
	total += maxcnt;
	kill[maxi][maxj] = c;
	tree[maxi][maxj] = 0;
	for (int dir = 0; dir < 4; dir++) {
		int nx = maxi; int ny = maxj;
		for (int size = 0; size < k; size++) {
			nx = nx + kill_dx[dir];
			ny = ny + kill_dy[dir];
			if (ThrowBodyOverTheWindow(nx, ny)) break;
			if (tree[nx][ny] < 0) break;
			if (tree[nx][ny] == 0) {
				kill[nx][ny] = c;
				break;
			}
			kill[nx][ny] = c;	
			tree[nx][ny] = 0;
		}
	}
}
int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> n >> m >> k >> c;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> tree[i][j];
		}
	}
	while (m--) {
		fuck();
		fire();
		suicide();
		genocide();

	}

	cout << total;
	return 0;
}