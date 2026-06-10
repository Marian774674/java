//#include <iostream>
//#include<vector>
//using namespace std;
//
//vector<int>a;
//vector<int>b;
//int n;
//int k = 0;
//int x;
//bool f = false;
//int main()
//{
//	// 请在此输入您的代码
//	cin >> n;
//	for (int i = 0; i < n; ++i) {
//		int temp;
//		cin >> temp;
//		a.push_back(temp);
//	}
//	for (int i = 0; i < n; ++i) {
//		int temp;
//		cin >> temp;
//		b.push_back(temp);
//	}
//	while (k < n) {
//		bool flag = true;
//		for (int i = 0; i < b.size(); ++i) {
//			int index = (i + k) % n;
//			if (i == 0)x = b[i] - a[index];
//			if (b[i] - a[index] != x) {
//				k++;
//				flag = false;
//				break;
//			}
//		}
//		if (flag) {
//			f = true;
//			break;
//		}
//	}
//	cout << (f ? k : -1);
//	return 0;
//}