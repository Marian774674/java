//#include <iostream>
//#include<vector>
//#include<math.h>
//using namespace std;
//
//int A, B;
//bool tower[50][50];
//int cnt = 0;
//int h;
//
//void dfs(int x, int y) {
//	if (B == 0 && A == 0) {
//		cnt++;
//		return;
//	}
//	if (x <= h && y <= x) {
//		if (y == 0) {
//			if (B != 0) {
//				tower[x][y] = 1;
//				B--;
//				dfs(x, y + 1);
//				B++;
//			}
//			if (A != 0) {
//				tower[x][y] = 0;
//				A--;
//				dfs(x, y + 1);
//				A++;
//			}
//		}
//		else {
//			bool t = tower[x - 1][y - 1];
//			bool l = tower[x][y - 1];
//			if (t) {
//				if (l && A != 0) {
//					A--;
//					tower[x][y] = 0;
//					if (y == x)dfs(x + 1, 0);
//					else dfs(x, y + 1);
//					A++;
//				}
//				if (!l && B != 0) {
//					B--;
//					tower[x][y] = 1;
//					if (y == x)dfs(x + 1, 0);
//					else dfs(x, y + 1);
//					B++;
//				}
//			}
//			else {
//				if (l && B != 0) {
//					B--;
//					tower[x][y] = 1;
//					if (y == x)dfs(x + 1, 0);
//					else dfs(x, y + 1);
//					B++;
//				}
//				if (!l && A != 0) {
//					A--;
//					tower[x][y] = 0;
//					if (y == x)dfs(x + 1, 0);
//					else dfs(x, y + 1);
//					A++;
//				}
//			}
//		}
//	}
//}
//
//int main()
//{
//	cout << "=== 航空订票系统 ===" << endl;
//	cin >> A >> B;
//	h = sqrt((A + B) * 2) - 1;
//	tower[0][0] = 0;
//	A--;
//	dfs(1, 0);
//	tower[0][0] = 1;
//	A++;B--;
//	dfs(1, 0);
//	cout << cnt << endl;
//	return 0;
//}