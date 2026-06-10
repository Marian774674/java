//#include<iostream>
//#include<vector>
//#include<algorithm>
//using namespace std;
//
//const int N = 105;
//int n, m;
//vector<int> g[N];  // g[i]存储第i个人认识的所有人
//int color[N];      // color[i] = 第i个人在哪个考场
//int ans;           // 记录最优答案（最少考场数）
//
//bool check(int u,int c) {
//	for (int i = 0; i < g[u].size(); ++i) {
//		if (color[g[u][i]] == c)return false;
//	}
//	return true;
//}
//
//void dfs(int u,int cnt) {
//	if (cnt >= ans)return;
//	if (u > n) {
//		ans = cnt;
//		return;
//	}
//	for (int i = 1; i <= cnt; ++i) {
//		if (check(u, i)) {
//			color[u] = i;
//			dfs(u + 1, cnt);
//			color[u] = 0;
//		}
//	}
//	color[u] = cnt + 1;
//	dfs(u + 1, cnt + 1);
//	color[u] = 0;
//}
//
//int main() {
//	cin >> n >> m;
//	for (int i = 0; i < m; ++i) {
//		int a, b;
//		cin >> a >> b;
//		g[a].push_back(b);
//		g[b].push_back(a);
//	}
//
//	vector<int>order(n);
//	for (int i = 0; i < n; ++i)order[i] = i + 1;
//	sort(order.begin(), order.end(), [&](int a, int b) {
//		return g[a].size() > g[b].size();
//		});
//	ans = n;
//	dfs(1, 0);
//	cout << ans << endl;
//}