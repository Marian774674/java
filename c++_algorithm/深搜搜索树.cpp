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
//bool flag = false;
////状态：k当前已经选了k个数，sum当前选出的k个数的和为sum
//void dfs(int k, int sum, int dep) {
//	if (k == K && sum == S) {
//		flag = true;
//		return;
//	}
//	//通用终止条件
//	if (dep == n + 1)return;
//	//在第dep层，去选/不去选第dep个数
//	dfs(k + 1, sum + a[dep], dep + 1);
//	dfs(k, sum, dep + 1);
//}
//int main() {
//	cin >> n >> K >> S;
//	for (int i = 0; i < n; ++i)cin >> a[i];
//	dfs(0, 0, 0);
//	cout << (flag ? "YES\n" : "NO\n");
//	return 0;
//}