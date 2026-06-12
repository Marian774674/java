//#include<iostream>
//#include<vector>
//#include<queue>
//#include<algorithm>
//using namespace std;
//
////哈夫曼树节点的创建
//struct treeNode {
//	int weight;
//	treeNode* lchild;
//	treeNode* rchild;
//	treeNode(int weight,treeNode* l,treeNode* r) {
//		this->weight = weight;
//		lchild = l;
//		rchild = r;
//	}
//};
//
//template<class T>
//struct cmp {
//	bool operator()(const T _Left, const T _Right)const {
//		return _Left->weight > _Right->weight;
//	}
//};
////创建哈夫曼树
//treeNode* build_huffmanTree(vector<int>a) {
//	//vector<treeNode*>b;
//	priority_queue<treeNode*,vector<treeNode*>,cmp<treeNode*>>q;
//	for (int i = 0; i < a.size(); ++i) {
//		treeNode* temp = new treeNode(a[i], NULL, NULL);
//		q.push(temp);
//	}
//	treeNode* l = NULL, * r = NULL, * p = NULL;
//	while (q.size() > 1) {
//		/*
//		sort(b.begin(), b.end(), [=](treeNode* A, treeNode* B) {
//			return A->weight > B->weight;
//			}); 
//		*/
//		l = q.top();
//		q.pop();
//		r = q.top();
//		q.pop();
//		p = new treeNode(l->weight + r->weight, l, r);
//		q.push(p);
//	};
//	return p;
//}
//int L = 0;
//int WPL = 0;
////层次遍历
//void LayerOrder(treeNode* root) {
//	queue<treeNode*>q;
//	q.push(root);
//	treeNode* last = root;
//	treeNode* next_last = NULL;
//	while (!q.empty()) {
//		treeNode* temp = q.front();
//		cout << temp->weight<<" ";
//		if (temp->lchild == NULL && temp->rchild == NULL) {
//			WPL += temp->weight * L;
//		}
//		q.pop();
//		if (temp->lchild != NULL) {
//			q.push(temp->lchild);
//			next_last = temp->lchild;
//		}
//		if (temp->rchild != NULL) {
//			q.push(temp->rchild);
//			next_last = temp->rchild;
//		}
//		if (temp == last) {
//			cout << endl;
//			L++;
//			last = next_last;
//		}
//	}
//}
//int main() {
//	vector<int>a{ 10,20,50,100 };
//	treeNode* root = build_huffmanTree(a);
//	LayerOrder(root);
//	cout << WPL;
//	return 0;
//}