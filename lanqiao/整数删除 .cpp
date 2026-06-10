//#include<iostream>
//#include<vector>
//#include<queue>
//using namespace std;
//
//long long N, K;
//vector<pair<long long, pair<long long, long long>>>vec;
//
//priority_queue < pair<long long, long long>, vector<pair<long long, long long>>, greater<pair<long long, long long>>>q;
//
//int main() {
//	cin >> N >> K;
//	for (long long i = 0; i < N; ++i) {
//		long long temp;
//		cin >> temp;
//		q.push({ temp,i });
//		vec.push_back({ temp,{1,1} });
//	}
//	for (long long i = 0; i < K; ++i) {
//		pair<long long, long long>p = q.top();
//		long long index = p.second; long long value = p.first;
//		while (value != vec[index].first) {
//			q.pop();
//			q.push({ vec[index].first , index });
//			p = q.top();
//			index = p.second; value = p.first;
//		}
//		q.pop();
//		index = p.second; value = p.first;
//		long long l = index - vec[index].second.first, r = index + vec[index].second.second;
//		if (0 <= r && r < N) {
//			vec[r].first += vec[index].first;
//			vec[r].second.first+=vec[index].second.first;
//		}
//		if (0 <= l && l < N) {
//			vec[l].first += vec[index].first;
//			vec[l].second.second+=vec[index].second.second;
//		}
//		vec[index].first = -1;
//	}
//	for (long long i = 0; i < vec.size(); ++i) {
//		if(vec[i].first!=-1)
//		cout << vec[i].first << " ";
//	}
//	return 0;
//}