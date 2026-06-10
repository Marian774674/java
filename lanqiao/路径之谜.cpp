#include<iostream>
#include<vector>
using namespace std;

int N;
int maze[21][21];
bool vis[21][21];
//			 右左下上
int dx[] = { 0,0,1,-1 };
int dy[] = { 1 ,-1,0,0 };

int xarrow[21];
int yarrow[21];

int fx;
int fy;

int routes[21 * 21];

int flag;

bool is_find = false;

int cal(int x, int y) {
	return x * N + y;
}

void dfs(int sx, int sy, int dep) {
	if (flag == 0 && sx == fx && sy == fy) {
		is_find = true;
		return;
	}
	for (int i = 0; i < 4; ++i) {
		int tx = sx + dx[i]; int ty = sy + dy[i];
		if (tx < 0 || tx >= N || ty < 0 || ty >= N)continue;
		if (vis[tx][ty])continue;
		if (xarrow[tx] == 0 || yarrow[ty] == 0)continue;
		//搜索前准备
		vis[tx][ty] = 1;
		if (--xarrow[tx] == 0)--flag; if (--yarrow[ty] == 0)--flag;
		routes[dep + 1] = (cal(tx, ty));
		//搜索
		dfs(tx, ty, dep + 1);
		if (is_find)return;
		//回溯
		vis[tx][ty] = 0;
		if (++xarrow[tx] == 1)++flag; if (++yarrow[ty] == 1)++flag;
		routes[dep + 1] = 0;
	}
}

int main() {
	cin >> N;
	for (int i = 0; i < N; ++i) {
		int temp;
		cin >> temp;
		yarrow[i] = temp;
	}
	for (int i = 0; i < N; ++i) {
		int temp;
		cin >> temp;
		xarrow[i] = temp;
	}
	fx = N - 1; fy = N - 1;
	flag = 2 * N;
	routes[0] = (cal(0, 0));
	vis[0][0] = 1;
	if (--xarrow[0] == 0)--flag; if (--yarrow[0] == 0)--flag;
	dfs(0, 0, 0);

	for (int i = 0; i < 21 * 21; ++i) {
		if (i != 0 && routes[i] == 0)break;
		cout << routes[i] << " ";
	}
	return 0;
}