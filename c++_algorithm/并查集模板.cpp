//#include<iostream>
//using namespace std;
//// parent[i] 存储 i 的父节点
//int parent[10005];
//
//// 初始化
//void init(int n) {
//    for (int i = 1; i <= n; i++) {
//        parent[i] = i;  // 每个人最初都是自己的老大
//    }
//}
//
//// 查找根节点（带路径压缩）
//int find(int x) {
//    if (parent[x] != x) {
//        parent[x] = find(parent[x]);  // 路径压缩：直接指向老大
//    }
//    return parent[x];
//}
//
//// 合并两个集合
//void unite(int x, int y) {
//    int rootX = find(x);
//    int rootY = find(y);
//    if (rootX != rootY) {
//        parent[rootX] = rootY;  // 让X的老大认Y的老大当老大
//    }
//}
//
//// 判断是否在同一集合
//bool same(int x, int y) {
//    return find(x) == find(y);
//}