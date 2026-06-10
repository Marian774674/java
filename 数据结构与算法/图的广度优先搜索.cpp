//#include<iostream>
//#include<queue>
//using namespace std;
//
//int matrix[7][7] = {
//	//0 1 2 3 4 5 6
//	//A B C D E F G
//	/*A*/{0,0,1,1,0,1,0},
//	/*B*/{0,0,1,0,0,0,0},
//	/*C*/{1,1,0,1,0,0,0},
//	/*D*/{1,0,1,0,0,0,0},
//	/*E*/{0,0,0,0,0,0,1},
//	/*F*/{1,0,0,0,0,0,1},
//	/*G*/{0,0,0,0,1,1,0}
//};
//bool vis[7];//标记数组
////广度优先搜索： 1.先进先出（队列）	2.标记：防止重复搜索	3.邻接点（邻接矩阵）
////搜索的最大层数
//int maxx = 0;
//
//struct node {
//	int value, dep;
//};
//
//void bfs() {
//	queue<node>q;
//	q.push(node{ 0,1 });
//	vis[0] = 1;
//	while (!q.empty()) {
//		node temp = q.front();
//		q.pop();
//		maxx = max(maxx, temp.dep);
//		cout << char(temp.value + 'A');
//		for (int i = 0; i < 7; ++i) {
//			if (matrix[temp.value][i] == 1 && !vis[i]) {
//				q.push(node{i,temp.dep+1});
//				vis[i] = 1;
//			}
//		}
//	}
//}
//int main() {
//	bfs();
//	cout << endl << "搜索的最大层数：" << maxx;
//	return 0;
//}