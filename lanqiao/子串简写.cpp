//#include<iostream>
//#include<vector>
//#include<algorithm>
//using namespace std;
//
//int K;
//char c1, c2;
//string s;
//int ans;
//vector<int>vec;
//
//
//int main() {
//	cin >> K >> s >> c1 >> c2;
//	int r = s.size() - 1;
//	int c2_num = 0;
//	while (r >= 0) {
//		if (s[r] == c2)++c2_num;
//		vec.push_back(c2_num);
//		--r;
//	}
//	reverse(vec.begin(), vec.end());
//	for (int i = 0; i < vec.size(); ++i) {
//		if (s[i] != c1)continue;
//		int min_end = i + K - 1;
//		if (min_end < vec.size()) {
//			ans += vec[min_end];
//		}
//	}
//	cout << ans;
//	return 0;
//}