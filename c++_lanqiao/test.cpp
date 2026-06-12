#include<iostream>
#include<vector>
using namespace std;

int maze[21][21];
int vis[21][21];

int N;
int xarrow[21];
int yarrow[21];

vector<int>routes;

//			 右左下上
int dx[] = { 0,0,1,-1 };
int dy[] = { 1 ,-1,0,0 };

int dfs(int x, int y) {
	if (x == N && y == N /* xarrow==0 && yarrow==0 */) {
		return;
	}
	for (int i = 0;i < 4;++i) {
		//前进
		int tx = x + dx[i];
		int ty = y + dy[i];
		if (tx > 20 && tx < 0 && ty > 20 && ty < 0)continue;
		if (vis[tx][ty] == 1)continue;
		xarrow[tx]--;yarrow[ty]--;
		vis[tx][ty] = 1;
		routes.push_back(tx * N + ty);
		//搜索下一个坐标
		dfs(tx, ty);
		//回溯
		xarrow[tx]++;yarrow[ty]++;
		vis[tx][ty] = 0;
		routes.pop_back();
	}

}

int main() {
	cin >> N;
	for (int i = 0;i < N;++i) {
		cin >> xarrow[i];
	}
	for (int i = 0;i < N;++i) {
		cin >> yarrow[i];
	}
	xarrow[0]--;yarrow[0]--;
	vis[0][0] = 1;
	routes.push_back(0);
	dfs(0, 0);
	for (int i = 0;i < routes.size();++i) {
		cout << routes[i] << " ";
	}
	return 0;
}