//#include<iostream>
//#include<vector>
//using namespace std;
//
//int cnt[89] = { 0 };
//string a, b;
//string q;
//
//void a_get(char tag) {
//	bool flag = false;
//	while (1) {
//		char temp = q[q.size() - 1];
//		--cnt[(int)temp];
//		a += temp;
//		q.erase(q.end() - 1);
//		if (flag && temp == tag)break;
//		flag = true;
//	}
//
//	char temp1 = a[0];
//	q += temp1;
//	a.erase(a.begin());
//	if (++cnt[(int)temp1] == 2)a_get(temp1);
//	return;
//}
//void b_get(char tag) {
//	bool flag = false;
//	while(1) {
//		char temp = q[q.size()-1];
//		--cnt[(int)temp];
//		b += temp;
//		q.erase(q.end() - 1);
//		if ( flag && temp == tag)break;
//		flag = true;
//	}
//
//	char temp1 = b[0];
//	q += temp1;
//	b.erase(b.begin());
//	if (++cnt[(int)temp1] == 2)b_get(temp1);
//	return;
//}
//
////K8XKA2A95A
////27K5J5Q6K4
//int main() {
//	cin >> a >> b;
//	while (a.size() && b.size()) {
//		if (a.size()) {
//			char temp = a[0];
//			q += temp;
//			a.erase(a.begin());
//			if (++cnt[(int)temp] == 2)a_get(temp);
//			if (a.size() == 0)break;
//		}
//		if (b.size()) {
//			char temp = b[0];
//			q += temp;
//			b.erase(b.begin());
//			if (++cnt[(int)temp] == 2)b_get(temp);
//			if (b.size() == 0)break;
//		}
//	}
//	if (a.size() != 0)cout << a;
//	if (b.size() != 0)cout << b;
//	return 0;
//}