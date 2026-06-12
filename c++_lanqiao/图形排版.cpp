//#include<iostream>
//using namespace std;
//
//const int MAXN = 1e5 + 5;
//
//int n, m;
//int w[MAXN], h[MAXN];
//int a[MAXN];
//
//void ins(int i, int& W, int& H) {
//	if (W + w[i] <= m) H = max(H, h[i]);
//	else H = max(H, (h[i] * (m - W) + w[i] - 1) / w[i]);
//	W = min(m, W + w[i]);
//}
//
//int calh(int i, int W, int H) {
//	while (W < m && i < n) {
//		ins(i++, W, H);
//	}
//	return H + a[i];
//}
//
//int main() {
//	ios::sync_with_stdio(false);
//	cin.tie(0);
//
//	cin >> m >> n;
//	for (int i = 0; i < n; ++i) cin >> w[i] >> h[i];
//	for (int i = n - 1; i >= 0; --i) a[i] = calh(i, 0, 0);
//
//	int res = a[0];
//	int pre_h = 0;
//	int W = 0, H = 0;
//	for (int i = 0; i < n; i++) {
//		int tmp = calh(i + 1, W, H);
//		res = min(res, pre_h + tmp);
//		ins(i, W, H);
//		if (W == m) {
//			pre_h += H;
//			W = 0;
//			H = 0;
//		}
//	}
//
//	cout << res << endl;
//	return 0;
//}