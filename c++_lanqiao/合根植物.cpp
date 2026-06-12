//#include<iostream>
//using namespace std;
//
//const int N = 1e6 + 5;
//int parent[N];
//int m, n;
//int ans;
//
//void init() {
//	for (int i = 1; i <= m * n; ++i)parent[i] = i;
//}
//int find(int x) {
//	if (parent[x] != x) parent[x] = find(parent[x]);
//	return parent[x];
//}
//void unite(int x, int y) {
//	int xroot = find(x);
//	int yroot = find(y);
//	if (xroot == yroot)return;
//	if (xroot != yroot) {
//		parent[yroot] = xroot;
//		--ans;
//	}
//}
//
//
//int main() {
//	ios::sync_with_stdio(false);
//	cin.tie(nullptr);
//	cin >> m >> n;
//	ans = m * n;
//	int k;
//	cin >> k;
//	init();
//	for (int i = 0; i < k; ++i) {
//		int a, b;
//		cin >> a >> b;
//		unite(a, b);
//	}
//	cout << ans;
//	return 0;
//}