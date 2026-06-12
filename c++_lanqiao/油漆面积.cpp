//#include<iostream>
//#include<vector>
//using namespace std;
//
//const int MAXN = 1e4 + 5;
//
//int n;
//bool maze[MAXN][MAXN];
//long long S = 0;
//
//void dfs(pair<int, int> node1, pair<int, int> node2) {
//	int x1 = node1.first, x2 = node2.first, y1 = node1.second, y2 = node2.second;
//	int bx1 = min(x1, x2), by1 = min(y1, y2), bx2 = max(x1, x2), by2 = max(y1, y2);
//	for (int i = bx1; i < bx2; ++i) {
//		for (int j = by1; j < by2; ++j) {
//			if (maze[i][j] == 1)continue;
//			maze[i][j] = 1;
//			++S;
//		}
//	}
//}
//int main() {
//
//	cin >> n;
//	for (int i = 0; i < n; ++i) {
//		pair<int, int>node1;
//		pair<int, int>node2;
//		cin >> node1.first >> node1.second >> node2.first >> node2.second;
//		dfs(node1, node2);
//	}
//	cout << S;
//	return 0;
//}