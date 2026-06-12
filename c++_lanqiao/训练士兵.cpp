//#include<iostream>
//#include<vector>
//#include<algorithm>
//using namespace std;
//
//
//
//typedef long long ll;
//ll n, s;
//ll res = 0;
//vector<pair<ll, ll>>vec;
//
//int main() {
//	cin >> n >> s;
//	ll sum = 0;
//	for (ll i = 0;i < n;++i) {
//		ll p, c;
//		cin >> p >> c;
//		sum += p;
//		vec.push_back({ c,p });
//	}
//	sort(vec.begin(), vec.end());
//
//	ll cnt = 0;
//	for (int i = 0;i < n;++i) {
//		if (sum > s) {
//			res += s * (vec[i].first - cnt);
//			cnt = vec[i].first;
//			sum -= vec[i].second;
//		}
//		else res += vec[i].second * (vec[i].first - cnt);
//	}
//	cout << res << endl;
//	return 0;
//}