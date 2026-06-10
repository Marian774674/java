//#include<iostream>
//#include<queue>
//#include<vector>
//using namespace std;
//
//struct node {
//	int x, y;
//};
//
//template <class T>
//struct cmp {
//	  bool operator()(const T& _Left, const T& _Right) const{
//		return _Left.x > _Right.x;
//	}
//};
//
//int main() {
//	vector<node>a{ {5,4},{3,1},{2,6},{8,9} };
//	//priority_queue<int,vector<int>,less<int>>q1(a.begin(),a.end());
//	priority_queue<node,vector<node>,cmp<node>>q2(a.begin(),a.end());
//	while (!q2.empty()) {
//		cout << q2.top().x << " ";
//		q2.pop();
//	}
//	return 0;
//}