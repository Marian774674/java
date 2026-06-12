//#include<iostream>
//#include<vector>
//#include<algorithm>
//using namespace std;
//
//const int N = 105;
//int n, m;
//vector<int> g[N];  // g[i]存储第i个人认识的所有人
//int color[N];      // color[i] = 第i个人在哪个考场
//int ans;           // 记录最优答案（最少考场数）
//
//// 检查：第u个人能否分配到考场c？
//bool check(int u, int c) {
//    // 遍历u认识的所有人
//    for (int v : g[u]) {
//        // 如果u认识的某个人v已经在考场c了
//        if (color[v] == c) {
//            return false;  // u不能去考场c
//        }
//    }
//    return true;  // 可以去考场c
//}
//
//// DFS：给第u个人分配考场，当前已经用了cnt个考场
//void dfs(int u, int cnt) {
//    // 【剪枝】如果当前考场数已经>=最优解，没必要继续
//    if (cnt >= ans) return;
//
//    // 【成功】所有人都分配完了
//    if (u > n) {
//        ans = cnt;  // 更新最优解
//        return;
//    }
//
//    // 【尝试1】把第u个人放到已有的考场1~cnt
//    for (int c = 1; c <= cnt; ++c) {
//        if (check(u, c)) {  // 如果能放
//            color[u] = c;   // 分配
//            dfs(u + 1, cnt); // 继续处理下一个人
//            color[u] = 0;   // 【回溯】撤销分配
//        }
//    }
//
//    // 【尝试2】给第u个人开一个新考场
//    color[u] = cnt + 1;
//    dfs(u + 1, cnt + 1);
//    color[u] = 0;
//}
//
//int main() {
//    cin >> n >> m;
//
//    // 读入认识关系
//    for (int i = 0; i < m; ++i) {
//        int a, b;
//        cin >> a >> b;
//        g[a].push_back(b);
//        g[b].push_back(a);
//    }
//
//    // 【优化】按度数排序（认识人多的人先处理）
//    vector<int> order(n);
//    for (int i = 0; i < n; ++i) order[i] = i + 1;
//    sort(order.begin(), order.end(), [&](int a, int b) {
//        return g[a].size() > g[b].size();
//        });
//
//    ans = n;  // 最坏情况：n个人需要n个考场
//    dfs(1, 0);  // 从第1个人开始，当前用了0个考场
//
//    cout << ans << endl;
//    return 0;
//}