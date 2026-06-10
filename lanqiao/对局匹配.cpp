//#include <iostream>
//#include<vector>
//using namespace std;
//
//const int N = 1e5 + 5;
//int main() {
//	int n, k;
//	cin >> n >> k;
//	int ans = 0;
//	int maxx = 0;
//	vector<int>score(n);
//	vector<int>cnt(N);
//	for (int i = 0; i < n; ++i) {
//		cin >> score[i];
//		++cnt[score[i]];
//		maxx = max(maxx, score[i]);
//	}
//	if (!k) {
//		for (int i = 0; i < N; i++)ans += (cnt[i] != 0);
//		printf("%d", ans);
//		return 0;
//	}
//	for (int i = 0; i < k; ++i) {
//		vector<int>arr(N);
//		vector<int>dp(N);
//		int index = 0;
//		for (int j = i; j <= maxx; j += k) {
//			arr[index++] = cnt[j];
//		}
//		dp[0] = arr[0];
//		for (int j = 1; j < index; ++j) {
//			if (j == 1)dp[1] = max(dp[0], arr[1]);
//			else dp[j] = max(dp[j - 1], dp[j - 2] + arr[j]);
//		}
//		ans += dp[index - 1];
//	}
//	cout << ans;
//	return 0;
//}
