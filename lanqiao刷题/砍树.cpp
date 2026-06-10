//#include<iostream>
//#include<vector>
//using namespace std;
//
//int N, M;
//const int MAXN = 1e5 + 5;
//const int LOG = 17;
//
//vector<pair<int, int>>g[MAXN];
//
//int depth[MAXN];
//int parent[MAXN][LOG];
//
//int diff[MAXN];
//int edgeCount[MAXN];
//
//void dfs_init(int u, int p, int d) {
//	depth[u] = d;
//	parent[u][0] = p;
//
//	for (int i = 1; i < LOG; ++i) {
//		parent[u][i] = parent[parent[u][i - 1]][i - 1];
//	}
//
//	for (auto& edge : g[u]) {
//		int v = edge.first;
//		if (v != p) {
//			dfs_init(v, u, d + 1);
//		}
//	}
//}
//
//int lca(int u, int v) {
//	if (depth[u] < depth[v])swap(u, v);
//
//	int diff_depth = depth[u] - depth[v];
//	for (int i = 0; i < LOG; ++i) {
//		if (diff_depth & (1 << i)) {
//			u = parent[u][i];
//		}
//	}
//
//	if (u == v)return u;
//
//	for (int i = LOG - 1; i >= 0; --i) {
//		if (parent[u][i] != parent[v][i]) {
//			u = parent[u][i];
//			v = parent[v][i];
//		}
//	}
//
//	return parent[u][0];
//}
//
//void dfs_count(int u,int p) {
//	for (auto& edge : g[u]) {
//		int v = edge.first;
//		int eid = edge.second;
//		if (v != p) {
//			dfs_count(v, u);
//
//			diff[u] += diff[v];
//			edgeCount[eid] = diff[v];
//		}
//	}
//}
//
//int main() {
//	// 加速输入输出
//	ios::sync_with_stdio(false);
//	cin.tie(0);
//
//	cin >> N >> M;
//
//	// 1. 读入树的结构
//	for (int i = 1; i < N; ++i) {
//		int x, y;
//		cin >> x >> y;
//		g[x].push_back({ y, i });
//		g[y].push_back({ x, i });
//	}
//
//	// 2. 预处理
//	dfs_init(1, 0, 0);
//
//	// 3. 处理 m 个询问，进行树上差分打标记
//	for (int i = 0; i < M; ++i) {
//		int a, b;
//		cin >> a >> b;
//
//		int l = lca(a, b);
//
//		++diff[a];
//		++diff[b];
//		diff[l] -= 2;
//	}
//
//	// 4. 再次 DFS，自底向上统计每条边被经过的次数
//	dfs_count(1, 0);
//
//	// 5. 找答案
//	int ans = -1;
//	for (int i = 1; i < N; i++) {
//		if (edgeCount[i] == M) {
//			ans = i; // 因为是从 1 到 n-1 遍历，后面的会覆盖前面的，自然就是编号最大的
//		}
//	}
//
//	cout << ans << endl;
//
//	return 0;
//}