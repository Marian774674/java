//#include<iostream>
//#include<stack>
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
//void dfs() {
//	stack<int>s;
//	s.push(0);//从A点开始搜索
//	vis[0] = 1;//入栈就要标记
//	while (!s.empty()) {
//		int temp = s.top();
//		s.pop();
//		cout << char(temp + 'A');
//		for (int i = 6; i >= 0; --i) {
//			if (matrix[temp][i] == 1&&!vis[i]) {
//				s.push(i);
//				vis[i] = 1;
//			}
//		}
//	}
//}
//
//int main() {
//	dfs();
//	return 0;
//}