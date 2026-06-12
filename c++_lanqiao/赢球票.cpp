//#include<iostream>
//#include<vector>
//#include<algorithm>
//using namespace std;
//
//int n;
//vector<int>sheets;
//
//bool flag;
//
//int ans;
//
//int dfs(vector<int>vec, int start, int len) {
//	int count = 1;
//	int current_point = start;
//	int maxx=*max_element(vec.begin(), vec.end());
//	while (count <= maxx) {
//		current_point = current_point % n;
//		int current_value = vec[current_point];
//		if (current_value != 0 && current_value == count) {
//			vec[current_point] = 0;
//			return current_value + dfs(vec, current_point + 1, len - 1);
//		}
//		if(current_value!=0)++count;
//		++current_point;
//	}
//	return 0;
//}
//
//int main() {
//	cin >> n;
//	for (int i = 0; i < n; ++i) {
//		int temp;
//		cin >> temp;
//		sheets.push_back(temp);
//	}
//	for (int i = 0; i < sheets.size(); ++i) {
//		int max = dfs(sheets, i, n);
//		ans = max > ans ? max : ans;
//	}
//	cout << ans << endl;
//	return 0;
//}