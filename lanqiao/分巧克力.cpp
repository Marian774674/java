//#include<iostream>
//#include<vector>
//using namespace std;
//
//int n, k;
//vector<pair<int, int>>vec;
//int ans;
//
//int main() {
//	cin >> n >> k;
//	int maxx = 0;
//	for (int i = 0; i < n; ++i) {
//		int a, b;
//		cin >> a >> b;
//		if (a > b)vec.push_back({ b,a });
//		else vec.push_back({ a,b });
//		if (a > b)maxx = maxx > b ? maxx : b;
//		else maxx = maxx > a ? maxx : a;
//	}
//	for (int i = maxx; i > 0; --i) {
//		int num = 0;
//		for (int j = 0; j < vec.size(); ++j) {
//			num += (vec[j].first / i) * (vec[j].second / i);
//		}
//		if (num >= k) {
//			ans = i;
//			break;
//		}
//	}
//	cout << ans;
//	return 0;
//}