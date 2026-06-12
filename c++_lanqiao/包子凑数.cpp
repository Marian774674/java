//#include <iostream>
//using namespace std;
//
//const int N = 10005;  // 改小
//int num[105];
//bool dp[N];
//
//// 添加 GCD 函数
//int gcd(int a, int b) {
//    return b == 0 ? a : gcd(b, a % b);
//}
//
//int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(0);
//
//    int n;
//    int des = 0;
//    cin >> n;
//
//    for (int i = 0; i < n; i++) {
//        cin >> num[i];
//    }
//
//    int g = num[0];
//    for (int i = 1; i < n; i++) {
//        g = gcd(g, num[i]);
//    }
//    if (g != 1) {
//        cout << "INF";
//        return 0;
//    }
//
//    dp[0] = true;
//
//    for (int i = 0; i < n; i++) {
//        int a = num[i];
//        for (int j = a; j < N; j++) {
//            if (dp[j - a]) {
//                dp[j] = true;
//            }
//        }
//    }
//
//    for (int i = 1; i < N; i++) {
//        if (!dp[i]) {
//            des++;
//        }
//    }
//
//    cout << des;
//    return 0;
//}