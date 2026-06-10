//#include<iostream>
//#include<vector>
//using namespace std;
//
//const int N = 5 * 1e5 + 5;
//const int LOG = 17;
//int parent[N][LOG];
//int n, q;
//vector<int> g[N];
//
//int depth[N];
//void dfs_init(int u, int p, int d) {
//	parent[u][0] = p;
//	depth[u] = d;
//
//	for (int i = 1; i < LOG; ++i) {
//		parent[u][i] = parent[parent[u][i - 1]][i - 1];
//	}
//
//	for (auto & edge : g[u]) {
//		int v = edge;
//		if (v != p) {
//			dfs_init(v, u, d + 1);
//		}
//	}
//}
//
//int lca(int u, int v) {
//	if (depth[u] < depth[v])swap(u, v);
//	int diff_depth = depth[u] - depth[v];
//	for (int i = 0; i < LOG; ++i) {
//		if (diff_depth & (1 << i)) {
//			u = parent[u][i];
//		}
//	}
//	if (u == v)return v;
//
//	for (int i = LOG - 1; i >= 0; --i) {
//		if (parent[u][i] != parent[v][i]) {
//			u = parent[u][i];
//			v = parent[v][i];
//		}
//	}
//	return parent[u][0];
//}
//
//
//
//int main() {
//	ios::sync_with_stdio(false);
//	cin.tie(0);
//
//	cin >> n >> q;
//	for (int i = 1; i < n; ++i) {
//		int a, b;
//		cin >> a >> b;
//		g[a].push_back(b);
//		g[b].push_back(a);
//	}
//	dfs_init(1, 0, 0);
//	for (int i = 0; i < q; ++i) {
//		int u, v;
//		cin >> u >> v;
//		cout << lca(u, v) << endl;
//	}
//	return 0;
//}