//#include<iostream>
//using namespace std;
//
//int matrix[7][7] = {
//	//0 1 2 3 4 5 6
//	//A B C D E F G
///*A*/{0,0,1,1,0,1,0},
///*B*/{0,0,1,0,0,0,0},
///*C*/{1,1,0,1,0,0,0},
///*D*/{1,0,1,0,0,0,0},
///*E*/{0,0,0,0,0,0,1},
///*F*/{1,0,0,0,0,0,1},
///*G*/{0,0,0,0,1,1,0}
//};
//
//bool vis[7];//标记数组
////深度优先搜索： 1.先进后出（栈/递归）	2.标记：防止重复搜索	3.邻接点（邻接矩阵）
////递归中的参数称为状态---dep:阶段
//int maxx = 0;
//void dfs(int index, int dep) {
//	vis[index] = 1;
//	maxx = max(maxx, dep);
//	cout << char(index + 'A');
//	for (int i = 0; i < 7; ++i) {
//		if (matrix[index][i] == 1 && !vis[i]) {
//			dfs(i, dep + 1);
//		}
//	}
//}
//int main() {
//	dfs(0, 1);
//	cout << endl << "最长搜索深度：" << maxx;
//	return 0;
//}