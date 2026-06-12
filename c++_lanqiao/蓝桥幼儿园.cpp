//#include<iostream>
//#include<vector>
//using namespace std;
//
//const int MAXN = 2 * 1e5;
//int n, m;
//int parent[MAXN];
//
//void init(int n) {
//	for (int i = 1; i < n; ++i) {
//		parent[i] = i;
//	}
//}
//int find(int x) {
//	if (parent[x] != x) {
//		parent[x] = find(parent[x]);
//	}
//	return parent[x];
//}
//void unite(int x, int y) {
//	int rootX = find(x);
//	int rootY = find(y);
//	if (rootX != rootY) {
//		parent[rootX] = rootY;
//	}
//}
//bool same(int x, int y) {
//	return find(x) == find(y);
//}
//
//int main() {
//	ios::sync_with_stdio(false);
//	cin.tie(0);
//
//	cin >> n >> m;
//	init(n);
//	vector<string>ans;
//	for (int i = 0; i < m; ++i) {
//		int op, a, b;
//		cin >> op >> a >> b;
//		if (op == 2) {
//			ans.push_back(same(a, b) ? "YES" : "NO");
//		}
//		else {
//			unite(a, b);
//		}
//	}
//	for (auto& a : ans) {
//		cout << a << endl;;
//	}
//	return 0;
//}