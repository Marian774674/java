//#include <iostream>
//using namespace std;
//
//typedef long long ll;
//
//const ll MAXN = 1e5 + 5;
//ll vis[MAXN];
//
//int main()
//{
//	ios::sync_with_stdio(false);
//	cin.tie(0);
//	ll n, k;
//	cin >> n >> k;
//	ll ans = 0;
//	ll sum = 0;
//	for (ll i = 0; i < n; ++i) {
//		ll temp;
//		cin >> temp;
//		sum += temp;
//		ll a = sum % k;
//		if (a == 0) {
//			++ans;
//			ans += vis[a];
//			++vis[a];
//		}
//		else {
//			ans += vis[a];
//			++vis[a];
//		}
//	}
//	cout << ans << endl;
//	return 0;
//}