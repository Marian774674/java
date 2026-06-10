//#include<iostream>
//using namespace std;
//
///*
//
//3 4
//S**.
//....
//***F
//
// S为起点，F为终点,*为墙壁，.代表路径
// 问能否从S走到F
//
//*/
//
//char maze[101][101];//迷宫数组
//bool vis[101][101];//标记数组
//int n, m;//n行m列
//int sx, sy;//起点坐标
//int fx, fy;//终点坐标
//bool flag = false;//标记是否找到
////方向数组   右左上下
//int dx[] = { 0,0,1,-1 };
//int dy[] = { 1,-1,0,0 };
//
////搜索深度
//int max_dep = 0;
//
////深搜： 1.先进先出	 2.标记  3.邻接点（方向数组）
//void dfs(int x, int y,int dep) {
//	if (x == fx && y == fy) {
//		max_dep = max(max_dep, dep);
//		flag = true;
//		return;
//	}
//	for (int i = 0; i < 4; ++i) {
//		int bx = x + dx[i];
//		int by = y + dy[i];
//		//如果新生成的位置越界了，无需搜索，跳过本轮生成新位置
//		if (bx < 0 || bx >= n || by < 0 || by >= m)continue;
//		//如果新生成的位置是墙壁（*），无需搜索，跳过本轮生成新位置
//		if (maze[bx][by] == '*')continue;
//		//如果新生成的位置走过了（被标记），无需搜索，跳过本轮生成新位置
//		if (vis[bx][by])continue;
//		vis[bx][by] = 1;
//		dfs(bx, by,dep+1);
//	}
//}
//
//int main() {
//	cin >> n >> m;
//	for (int i = 0; i < n; ++i) {
//		for (int j = 0; j < m; ++j) {
//			cin >> maze[i][j];
//			if (maze[i][j] == 'S') {
//				sx = i, sy = j;
//			}
//			if (maze[i][j] == 'F') {
//				fx = i, fy = j;
//			}
//		}
//	}
//	vis[sx][sy] = 1;
//	dfs(sx, sy,0);//初始状态
//	cout << (flag ? "YES" : "NO\n");
//	cout << endl << "最长搜索深度：" << max_dep;
//	return 0;
//}