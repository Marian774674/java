//#include<iostream>
//using namespace std;
//
//int N, n;
//struct plane {
//	int T, D, L;
//	plane(int T, int D, int L) {
//		this->T = T;
//		this->D = D;
//		this->L = L;
//	}
//	plane() { T = 0; D = 0; L = 0; }
//};
//int T, D, L;
//
//plane a[11][11];
//int len[11];
//bool flag = false;
//bool vis[11][11];
//
//void dfs(int Time, int NN, int dep) {
//	//6.搜到了
//	if (dep == n + 1) {
//		flag = true;
//		return;
//	}
//	//通用终止条件
//	for (int i = 1; i <= n; ++i) {
//		if (!vis[NN][i] && Time > a[NN][i].D + a[NN][i].T)
//			return;
//	}
//	//1.枚举方案数
//	for (int i = 1; i <= n; ++i) {
//		//2.标记
//		if (!vis[NN][i]) {
//			int startTime = max(Time, a[NN][i].T);
//			//4.搜索下一层
//			if (startTime <= a[NN][i].D + a[NN][i].T) {
//				vis[NN][i] = 1;
//				dfs(startTime + a[NN][i].L, NN, dep + 1);
//				//5.回溯
//				vis[NN][i] = 0;
//			}
//		}
//	}
//}
//
//int main() {
//	cin >> N;
//	for (int i = 1; i <= N; ++i) {
//		cin >> n;
//		len[i] = n;
//		for (int j = 1; j <= n; ++j) {
//			cin >> T >> D >> L;
//			a[i][j] = plane(T, D, L);
//		}
//	}
//	for (int i = 1; i <= N; ++i) {
//		n = len[i];
//		dfs(0, i, 1);
//		cout << (flag ? "YES" : "NO") << "\n";
//		flag = false;
//	}
//	return 0;
//}
