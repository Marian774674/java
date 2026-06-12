//#define _CRT_SECURE_NO_WARNINGS
//#include<iostream>
//using namespace std;
//
////深搜搜索树模型
///*n个数，从中选出K个，要求选出来的K个数和为S
//input：
//n:3		K:2		S:6
//arr:1 2 5
//output:
//yes
//*/
//
//int n, K, S;//目标状态
//int a[101];
//int ans[101];
//bool vis[101];
//bool flag = false;
////搜索与回溯算法
//void dfs(int k, int sum, int dep) {
//	//6.搜到了
//	if (k == K && sum == S) {
//		flag = true;
//		for (int i = 1; i <= k; ++i) 
//			printf("%d", ans[i]);
//		printf("\n");
//		return;
//	}
//	//通用终止条件
//	if (dep == n + 1)return;
//	//1.枚举方案数
//	for (int i = dep; i <= n; ++i) {
//		if (!vis[i]/*重复性剪枝 && a[i]>ans[dep - 1]*/) {	
//			//2.标记，防止重复搜索
//			vis[i] = 1;
//			//3.针对当前层进行搜索
//			ans[dep] = a[i];
//			//4.进入下一层搜索下一个数
//			dfs(k + 1, sum + ans[dep], dep + 1);
//			//5.回溯
//			vis[i] = 0;
//		}
//	}
//}
//int main() {
//	cin >> n >> K >> S;
//	for (int i = 1; i <= n; ++i)scanf("%d", &a[i]);//cin >> a[i];
//	dfs(0, 0, 1);
//	cout << (flag ? "YES\n" : "NO\n");
//	return 0;
//}