//#include<iostream>
//using namespace std;
//
///*
//全排列
//input:
//abc
//output:
//abc
//acb
//bac
//bca
//cab
//cba
//*/
//string s;
//char ans[101];
//bool vis[101];
//void dfs(int dep) {
//	//搜到了
//	if (dep == s.size()+1) {//前s.siae()个元素已经搜索到了
//		//搜多少打印多少
//		for (int i = 1; i <= dep-1; ++i) {
//			printf("%c", ans[i]);
//		}
//		printf("\n");
//		return;
//	}
//
//	//1.枚举方案数
//	for (int i = 0; i < s.size(); ++i) {
//		if (!vis[i]) {
//			//2.标记
//			vis[i] = 1;
//			//3.搜索当前层
//			ans[dep] = s[i];
//			//4.搜索下一层
//			dfs(dep + 1);
//			//5.回溯
//			vis[i] = 0;
//		}
//	}
//}
//int main() {
//	cin >> s;
//	dfs(1);
//	return 0;
//}