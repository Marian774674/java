//#include<iostream>
//#include<vector>
//#include <iomanip>
//#include<algorithm>
//using namespace std;
//
//int n;
//vector<pair<int, int>>ranges;
//
//bool check(int x) {
//	vector<pair<int, int>>a(ranges);
//	int idx = 0;
//	double coverd = 0;
//	while (coverd <= 20000 && idx < n) {
//		bool flag = false;
//
//		for (int i = 0; i < a.size(); ++i) {
//			pair<int, int>node = a[i];
//			int l = node.first; int r = node.second;
//			int len = r - l;
//			if (l - x <= coverd && coverd <= r + x) {
//				flag = true;
//				if (l + x >= coverd) {
//					coverd += len;
//				}
//				else {
//					coverd = r + x;
//				}
//				a.erase(a.begin() + i);
//				break;
//			}
//		}
//		idx++;
//	}
//
//	return coverd >= 20000;
//}
//
//int main() {
//	cin >> n;
//	for (int i = 0; i < n; ++i) {
//		int l, r;
//		cin >> l >> r;
//		ranges.push_back({ 2 * l,2 * r });
//	}
//	sort(ranges.begin(), ranges.end(), [](pair<int, int> a, pair<int, int> b) {
//		return a.second < b.second;
//		});
//
//
//	int l = 0, r = 20000;
//	int ans = r;
//	while (l <= r) {
//		int mid = (l + r) / 2;
//		bool flag = check(mid);
//		if (flag) {
//			ans = mid;
//			r = mid - 1;
//		}
//		else l = mid + 1;
//	}
//
//	if (ans % 2 == 0) {
//		cout << ans / 2 << endl;
//	}
//	else {
//		cout << fixed << setprecision(1) << ans / 2.0 << endl;
//	}
//	return 0;
//}