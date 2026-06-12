//#include <iostream>
//#include<vector>
//using namespace std;
//
//int N;
//vector<int>a;
//vector<int>b;
//int minx;
//int maxx;
//int main()
//{
//	// 请在此输入您的代码
//	cin >> N;
//	for (int i = 0; i < N; ++i) {
//		int temp1, temp2;
//		cin >> temp1;
//		a.push_back(temp1);
//		cin >> temp2;
//		b.push_back(temp2);
//	}
//	maxx = a[0] / b[0];
//	minx = a[0] / (b[0] + 1);
//	for (int i = 1; i < a.size(); ++i) {
//		int max = a[i] / b[i];
//		int min = a[i] / (b[i] + 1);
//		maxx = maxx < max ? maxx : max;
//		minx = minx > min ? minx : min;
//	}
//	cout << "\n";
//	cout << minx + 1 << " " << maxx;
//	return 0;
//}