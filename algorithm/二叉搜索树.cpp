#include<iostream>
#include<vector>
#include<queue>
using namespace std;

//二叉搜索树
struct treeNode{
	int value;
	treeNode* left_child;
	treeNode* right_child;
	treeNode(int value) {
		this->value = value;
		left_child = NULL;
		right_child = NULL;
	}
};

treeNode* build_binarySearchTree(vector<int>a) {
	treeNode* root = new treeNode(a[0]);
	for (int i = 1; i < a.size(); ++i) {
		treeNode* temp = new treeNode(a[i]);
		treeNode* temp_root = root;
		while (temp_root) {
			if (temp->value < temp_root->value) {
				if (temp_root->left_child == NULL) {
					temp_root->left_child = temp;
					break;
				}
				else temp_root = temp_root->left_child;
			}
			else if (temp->value > temp_root->value) {
				if (temp_root->right_child == NULL) {
					temp_root->right_child = temp;
					break;
				}
				else temp_root = temp_root->right_child;
			}
		}
	}
	return root;
}
//中序遍历
void InOrder(treeNode*root) {
	if (root == NULL)return;
	InOrder(root->left_child);
	cout << root->value;
	InOrder(root->right_child);
}
//前序遍历
void PreOrder(treeNode* root) {
	if (root == NULL)return;
	cout << root->value;
	PreOrder(root->left_child);
	PreOrder(root->right_child);
}
//后序遍历
void PostOrder(treeNode* root) {
	if (root == NULL)return;
	PostOrder(root->left_child);
	PostOrder(root->right_child);
	cout << root->value;
}
//层次遍历(基于广度优先搜索)
void LayerOrder(treeNode* root) {
	queue<treeNode*>queue;
	queue.push(root);
	treeNode* last = root;//当前行最右节点
	treeNode* next_last = NULL;//下一行最右节点
	while (!queue.empty()) {
		treeNode* temp = queue.front();
		cout << temp->value << " ";
		queue.pop();
		if (temp->left_child != NULL) {
			queue.push(temp->left_child);
			next_last = temp->left_child;
		}
		if (temp->right_child != NULL) {
			queue.push(temp->right_child);
			next_last = temp->right_child;
		}if (temp == last) {
			cout << endl;
			last = next_last;
		}
	}
}
//查找
bool find(treeNode* root, int target) {
	while (root) {
		if (root->value == target)return 1;
		if (target < root->value)root = root->left_child;
		if (target > root->value)root = root->right_child;
	}
}
//递归求树高
int treeHight(treeNode* root) {
	if (root == NULL)return 0;
	int lh = treeHight(root->left_child);
	int rh = treeHight(root->right_child);
	return lh > rh ? lh + 1 : rh + 1;
}
int main() {
	vector<int>a{ 5,4,3,1,2,6,8,9,7 };
	treeNode* root = build_binarySearchTree(a);
	//LayerOrder(root);
	int target;
	while (cin >> target) {
		cout << find(root, target) << endl;;
	}
	return 0;
}