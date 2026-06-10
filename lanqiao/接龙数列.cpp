//#include<iostream>
//#include<string>
//#include<algorithm>
//using namespace std;
//
//
//int dp[10] = { 0 };
//int N;
//int ans = 0;
//
//int main() {
//	cin >> N;
//	for (int i = 0; i < N; ++i) {
//		string temp;
//		cin >> temp;
//		int l = temp[0] - '0', r = temp[temp.size() - 1] - '0';
//		dp[r] = max(dp[r], dp[l] + 1);
//		ans = max(ans, dp[r]);
//	}
//	cout << N - ans;
//	return 0;
//}