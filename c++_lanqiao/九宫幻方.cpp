//#include<iostream>
//using namespace std;
//
//int maze[3][3];
//
//int row[3];
//int column[3];
//int slash[2];
//
//int sum;
//
//int flag = 0;
//
//bool aaa = false;
//
//int main() {
//
//	for (int i = 0; i < 3; ++i) {
//		for (int j = 0; j < 3; ++j) {
//			cin >> maze[i][j];
//			if (maze[i][j] != 0) {
//				if (++row[i] == 3 && ++flag)sum = maze[i][0] + maze[i][1] + maze[i][2];
//				if (++column[j] == 3 && ++flag)sum = maze[0][j] + maze[1][j] + maze[2][j];
//				if (i == j)if (++slash[0] == 3 && ++flag)sum = maze[0][0] + maze[1][1] + maze[2][2];
//				if (i + j == 2)if (++slash[1] == 3 && ++flag)sum = maze[0][2] + maze[1][1] + maze[2][0];
//			}
//		}
//	}
//	while (flag != 8) {
//		aaa = false;
//		for (int i = 0; i < 3; ++i) {
//			int index;
//			if (row[i] == 2) {
//				aaa = true;
//				int value = 0;
//				for (int j = 0; j < 3; ++j) {
//					if (maze[i][j] == 0)index = j;
//					else value += maze[i][j];
//				}
//				maze[i][index] = sum - value;
//				row[i] = 3;
//				++flag;
//				if (++column[index] == 3)++flag;
//				if (i == index)if (++slash[0] == 3)++flag;
//				if (i + index == 2) if (++slash[1] == 3)++flag;
//			}
//			if (column[i] == 2) {
//				aaa = true;
//				int value = 0;
//				for (int j = 0; j < 3; ++j) {
//					if (maze[j][i] == 0)index = j;
//					else value += maze[j][i];
//				}
//				maze[index][i] = sum - value;
//				column[i] = 3;
//				++flag;
//				if (++row[index] == 3)++flag;
//				if (i == index)if (++slash[0] == 3)++flag;
//				if (i + index == 2) if (++slash[1] == 3)++flag;
//			}
//			if (i < 2) {
//				if (slash[i] == 2) {
//					aaa = true;
//					int value = 0;
//					if (i == 0) {
//						int a, b, c;
//						a = maze[0][0]; b = maze[1][1]; c = maze[2][2];
//						if (a == 0)index = 0;
//						else value += a;
//						if (b == 0)index = 1;
//						else value += b;
//						if (c == 0)index = 2;
//						else value += c;
//						if (index == 0) {
//							maze[0][0] = sum - value;
//							slash[0] = 3;
//							++flag;
//							if (++row[0] == 3)++flag;
//							if (++column[0] == 3)++flag;
//						}
//						if (index == 1) {
//							maze[1][1] = sum - value;
//							slash[0] = 3;
//							++flag;
//							if (++row[1] == 3)++flag;
//							if (++column[1] == 3)++flag;
//							if (++slash[1] == 3)++flag;
//						}
//						if (index == 2) {
//							maze[2][2] = sum - value;
//							slash[0] = 3;
//							++flag;
//							if (++row[2] == 3)++flag;
//							if (++column[2] == 3)++flag;
//						}
//					}
//					else {
//						int a, b, c;
//						a = maze[0][2]; b = maze[1][1]; c = maze[2][0];
//						if (a == 0)index = 0;
//						else value += a;
//						if (b == 0)index = 1;
//						else value += b;
//						if (c == 0)index = 2;
//						else value += c;
//						if (index == 0) {
//							maze[0][2] = sum - value;
//							slash[1] = 3;
//							++flag;
//							if (++row[0] == 3)++flag;
//							if (++column[2] == 3)++flag;
//						}
//						if (index == 1) {
//							maze[1][1] = sum - value;
//							slash[1] = 3;
//							++flag;
//							if (++row[1] == 3)++flag;
//							if (++column[1] == 3)++flag;
//							if (++slash[1] == 3)++flag;
//						}
//						if (index == 2) {
//							maze[2][0] = sum - value;
//							slash[1] = 3;
//							++flag;
//							if (++row[2] == 3)++flag;
//							if (++column[0] == 3)++flag;
//						}
//					}
//				}
//			}
//		}
//		if (!aaa) {
//			break;
//		}
//	}
//	for (int i = 0; i < 3; ++i) {
//		for (int j = 0; j < 3; ++j) {
//			cout << maze[i][j] << " ";
//		}
//		cout << endl;
//	}
//	if (!aaa)cout << "Too Many";
//	return 0;
//}