//#include <iostream>
//#include <string>
//#include<map>
//#include<set>
//using namespace std;
//
//int n, m, k;
//string arr[6];
//map<string, long long>cnt[6];
//set<string>se;
//
//long long dfs(set<string> se) {
//	long long res = 0;
//	for (auto& key : se) {
//		int count = 0;
//		string com = key;
//		long long a = 1;
//		for (int i = 1;i <= n;++i) {
//			if (cnt[i].find(com)==cnt[i].end() || cnt[i][com] == 0)continue;
//			a = (a * cnt[i][com]) % (long long)(1e9 + 7);
//			count++;
//		}
//		if (count >= m) {
//			res = (res + a) % (long long)(1e9 + 7);
//		}
//	}
//	return res;
//}
//
//int main()
//{
//	cin >> n >> m >> k;
//	for (int i = 1;i <= n;++i) {
//		cin >> arr[i];
//	}
//	for (int i = 1;i <= n;++i) {
//		int l = 0;
//		while (l + k - 1 < (int)arr[i].size()) {
//			string key = arr[i].substr(l, k);
//			se.insert(key);
//			cnt[i][key] += 1;
//			l++;
//		}
//	}
//	long long ans = 0;
//	ans += dfs(se);
//	cout << ans % (long long)(1e9 + 7);
//	return 0;
//}