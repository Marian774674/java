//#include<iostream>
//#include<queue>
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
//
////广搜： 1.先进后出	 2.标记  3.邻接点（方向数组）
//struct node {
//	int x, y;
//	int dep;
//};
//queue<node>q;
//void bfs() {
//	while (!q.empty()) {
//		node temp = q.front();
//		q.pop();
//		if (temp.x == fx && temp.y == fy) {
//			flag = true;
//			cout << "搜索层数:" << temp.dep << endl;
//			return;
//		}
//		for (int i = 0; i < 4; ++i) {
//			int bx = temp.x + dx[i];
//			int by = temp.y + dy[i];
//			//如果新生成的位置越界了，无需搜索，跳过本轮生成新位置
//			if (bx < 0 || bx >= n || by < 0 || by >= m)continue;
//			//如果新生成的位置是墙壁（*），无需搜索，跳过本轮生成新位置
//			if (maze[bx][by] == '*')continue;
//			//如果新生成的位置走过了（被标记），无需搜索，跳过本轮生成新位置
//			if (vis[bx][by])continue;
//			q.push(node{ bx,by,temp.dep + 1 });
//		}
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
//	q.push(node{ sx, sy ,0 });
//	vis[sx][sy] = 1;
//	bfs();
//	cout << (flag ? "YES" : "NO\n");
//	return 0;
//}