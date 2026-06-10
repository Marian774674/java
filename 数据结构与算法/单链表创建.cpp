//#include<iostream>
//#include<vector>
//using namespace std;
//
//struct listNode{
//	int value;
//	listNode* next;
//	listNode(int value) {
//		this->value = value;
//		next = NULL;
//	}
//};
//listNode* head = NULL;
//listNode* tail = NULL;
//
//int a[]{ 1,2,3,4,5,6,7,8,9 };
//int n = sizeof(a) / sizeof(int);
//int index = 0;
//void build_list() {
//	listNode* p = NULL;
//	while (index<n) {
//		p = new listNode(a[index++]);
//		if (head == NULL)head = p;
//		else tail->next = p;
//		tail = p;
//	}
//}
//
//void print_list() {
//	listNode* p = head;
//	while (p) {
//		cout << p->value << " ";
//		p = p->next;
//	}
//}
//int main() {
//	build_list();
//	print_list();
//
//	return 0;
//}
