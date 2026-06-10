//#define _CRT_SECURE_NO_WARNINGS
//#include<iostream>
//#include<queue>
//using namespace std;
//
////迷宫
//int maze[55][55];
//
////方向数组  右左下 上 左上，右下，右上，左下
//int dx[] = { 0,0,1,-1 };
//int dy[] = { 1,-1,0,0 };
//int sx[] = { 0,0,1,-1, -1,1,-1,1 };
//int sy[] = { 1,-1,0,0, -1,1,1,-1 };
////标记数组
//int vis[55][55] = { 0 };
////测试数据组数
//int T;
////迷宫长宽
//int M, N;
////答案
//int ans;
//vector<int>anss;
//int dep;
//
//int vis_value = 4;
//
//struct node {
//	int x, y;
//};
//queue<node>q;
//queue<node>q_sea;
//void bfs_island() {
//	while (!q.empty()) {
//		node temp = q.front();
//		q.pop();
//		int tx, ty;
//		for (int i = 0; i < 4; ++i) {
//			tx = temp.x + dx[i]; ty = temp.y + dy[i];
//			if (tx < 0 || tx > M + 1 || ty < 0 || ty > N + 1)continue;
//			if (vis[tx][ty] == vis_value)continue;
//			if (maze[tx][ty]) {
//				vis[tx][ty] = vis_value;
//				q.push(node{ tx,ty });
//			}
//		}
//	}
//}
//
//void bfs_sea(node s) {
//	while (!q_sea.empty()) {
//		node temp = q_sea.front();
//		q_sea.pop();
//		int tx, ty;
//		for (int i = 0; i < 8; ++i) {
//			tx = temp.x + sx[i]; ty = temp.y + sy[i];
//			if (tx < 0 || tx > M + 1 || ty < 0 || ty > N + 1)continue;
//			if (vis[tx][ty] == vis_value)continue;
//			if (maze[tx][ty] == 1) {
//				q.push(node{ tx,ty });
//				vis[tx][ty] = vis_value;
//				++ans;
//				bfs_island();
//			}
//			if (maze[tx][ty] == 0) {
//				vis[tx][ty] = vis_value;
//				q_sea.push(node{ tx,ty });
//			}
//		}
//	}
//}
//
//int main() {
//	ios::sync_with_stdio(false);
//	cin.tie(0);
//
//	cin >> T;
//	while (T--) {
//		cin >> M >> N;
//		while (!q.empty()) q.pop();
//		while (!q_sea.empty()) q_sea.pop();
//		//填充海水
//		for (int i = 0; i <= N + 1; i++)
//		{
//			maze[0][i] = 0;
//			maze[M+1][i] = 0;
//		}
//		for (int i = 1; i <= M; i++)
//		{
//			maze[i][0] = 0;
//			maze[i][N + 1] = 0;
//		}
//		for (int i = 1; i <= M; ++i) {
//			for (int j = 1; j <= N; ++j) {
//				char temp;
//				cin >> temp;
//				maze[i][j] = temp - '0';
//			}
//		}
//	/*	for (int i = 0; i <= M+1 ; ++i) {
//			for (int j = 0; j <= N + 1; ++j) {
//				cout << maze[i][j];
//			}
//			cout << endl;
//		}*/
//		vis[0][0] = vis_value;
//		q_sea.push(node{ 0,0 });
//		bfs_sea(node{ 0,0 });
//		anss.push_back(ans);
//		ans = 0;
//		++vis_value;
//	}
//	for (int i = 0; i < anss.size(); ++i) {
//		cout << anss[i] << endl;
//	}
//	return 0;
//}