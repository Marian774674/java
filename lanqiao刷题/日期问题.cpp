//#include <iostream>
//#include <string>
//#include <algorithm>
//#include <vector>
//using namespace std;
//
//int mounth[13] = { 0,31,0,31,30,31,30,31,31,30,31,30,31 };
//
//bool is_run(int year) {
//	if (year % 100 == 0) {
//		if (year % 400 == 0)return true;
//		else return false;
//	}
//	else {
//		if (year % 4 == 0)return true;
//		else return false;
//	}
//}
//
//struct Data {
//	int y; int m; int d;
//	int index;
//	Data(int a, int b, int c, int i) {
//		y = a; m = b; d = c; index = i;
//	}
//};
//
//bool cmp(const Data& a, const Data& b) {
//	if (a.y != b.y) return a.y < b.y;
//	if (a.m != b.m) return a.m < b.m;
//	return a.d < b.d;
//}
//int main()
//{
//	//02/03/04
//	string s;
//	cin >> s;
//	bool f1 = 0, f2 = 0, f3 = 0;
//	string ans1, ans2, ans3;
//	string s1 = s.substr(0, 2);
//	string s2 = s.substr(3, 2);
//	string s3 = s.substr(6, 2);
//	int n1 = stoi(s1);
//	int n2 = stoi(s2);
//	int n3 = stoi(s3);
//
//	//年月日
//	string a1, a2, a3;
//	int y1, y2, y3;
//	if (n1 >= 60)a1 = "19" + s1;
//	else a1 = "20" + s1;
//	y1 = stoi(a1);
//	if (n2 > 0 && n2 <= 12) {
//		if (n2 == 2) {
//			if (is_run(y1) && n3 <= 29 && n3 > 0)f1 = 1;
//			else if (!is_run(y1) && n3 <= 28 && n3 > 0)f1 = 1;
//		}
//		else {
//			if (n3 <= mounth[n2] && n3 > 0)f1 = 1;
//		}
//	}
//	ans1 = a1 + '-' + s2 + '-' + s3;
//	//月日年
//	if (n3 >= 60)a2 = "19" + s3;
//	else a2 = "20" + s3;
//	y2 = stoi(a2);
//	if (n1 > 0 && n1 <= 12) {
//		if (n1 == 2) {
//			if (is_run(y2) && n2 <= 29 && n2 > 0)f2 = 1;
//			else if (!is_run(y2) && n2 <= 28 && n2 > 0)f2 = 1;
//		}
//		else {
//			if (n2 <= mounth[n1] && n2 > 0)f2 = 1;
//		}
//	}
//	ans2 = a2 + '-' + s1 + '-' + s2;
//	//日月年
//	if (n3 >= 60)a3 = "19" + s3;
//	else a3 = "20" + s3;
//	y3 = stoi(a3);
//	if (n2 > 0 && n2 <= 12) {
//		if (n2 == 2) {
//			if (is_run(y3) && n1 <= 29 && n1 > 0)f3 = 1;
//			else if (!is_run(y3) && n1 <= 28 && n1 > 0)f3 = 1;
//		}
//		else {
//			if (n1 <= mounth[n2] && n1 > 0)f3 = 1;
//		}
//	}
//	ans3 = a3 + '-' + s2 + '-' + s1;
//	if (ans1 == ans2)f1 = 0;
//	if (ans1 == ans3)f1 = 0;
//	if (ans3 == ans2)f2 = 0;
//	vector<Data>vec;
//	if (f1)vec.push_back(Data(y1, n2, n3, 1));
//	if (f2)vec.push_back(Data(y2, n1, n2, 2));
//	if (f3)vec.push_back(Data(y3, n2, n1, 3));
//	sort(vec.begin(), vec.end(), cmp);
//	for (int i = 0; i < vec.size(); ++i) {
//		if (vec[i].index == 1)cout << ans1 << endl;
//		if (vec[i].index == 2)cout << ans2 << endl;
//		if (vec[i].index == 3)cout << ans3 << endl;
//	}
//	return 0;
//}