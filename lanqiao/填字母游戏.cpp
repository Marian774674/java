//#include <iostream>
//#include<map>
//using namespace std;
//
//map<string, int> m;
//string str;
//
//int game() {
//    // 第一步：检查备忘录
//    if (m.count(str) != 0)
//        return m[str];
//
//    // 第二步：检查游戏是否结束
//    if (str.find("LOL") != -1)
//        return -1;  // 当前玩家必输（对方已赢）
//
//    if (str.find("*") == -1)
//        return 0;   // 平局（无胜者且无空格）
//
//    // 第三步：尝试所有可能的着法
//    int flag = -1;  // 初始化为"必输"
//
//    for (int i = 0; i < str.length(); i++) {
//        if (str[i] == '*') {
//            // 尝试填L
//            str[i] = 'L';
//            int res = game();        // 对方的评估结果
//            str[i] = '*';
//
//            if (res == -1)
//                return m[str] = 1;   // 对方必输 → 我方赢！立即返回
//            else if (res == 0)
//                flag = 0;            // 对方可平 → 我方至少可平
//
//            // 尝试填O
//            str[i] = 'O';
//            res = game();            // 对方的评估结果
//            str[i] = '*';
//
//            if (res == -1)
//                return m[str] = 1;   // 对方必输 → 我方赢！立即返回
//            else if (res == 0)
//                flag = 0;            // 对方可平 → 我方至少可平
//        }
//    }
//
//    return m[str] = flag;
//}
//
//int main() {
//    int n, index = 0, ans[15];
//    cin >> n;
//
//    for (int i = 0; i < n; i++) {
//        cin >> str;
//        ans[index++] = game();
//    }
//
//    for (int i = 0; i < index; i++) {
//        cout << ans[i] << endl;
//    }
//
//    return 0;
//}