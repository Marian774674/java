//#include<iostream>
//#include<vector>
//using namespace std;
//typedef long long ll;
//
//const int MAXN = 1e5 + 5;
//const int LOG = 17;
//
//int N, K;
//vector<pair<int, int>> g[MAXN];
//vector<int> routes;
//
//// dist_root 存距离（边权和），dep 存层数（步数）
//ll dist_root[MAXN];
//int dep[MAXN];
//int parent[MAXN][LOG];
//
//void dfs_init(int u, int p, ll d, int h) {
//    dist_root[u] = d; // 记录距离
//    dep[u] = h;       // 记录层数
//    parent[u][0] = p;
//
//    for (int i = 1; i < LOG; ++i) {
//        if (parent[u][i - 1] != 0) {
//            parent[u][i] = parent[parent[u][i - 1]][i - 1];
//        }
//        else {
//            parent[u][i] = 0;
//        }
//    }
//
//    for (auto& edge : g[u]) {
//        int v = edge.first;
//        if (v != p) {
//            // 距离累加边权，层数 +1
//            dfs_init(v, u, d + edge.second, h + 1);
//        }
//    }
//}
//
//int lca(int u, int v) {
//    // 使用 dep (层数) 来判断深浅
//    if (dep[u] < dep[v]) swap(u, v);
//
//    int diff = dep[u] - dep[v]; // 这里是层数差，是整数步数
//    for (int i = 0; i < LOG; ++i) {
//        if (diff & (1 << i)) { // 注意这里用 1 << i 即可，因为 diff 是 int
//            u = parent[u][i];
//        }
//    }
//
//    if (u == v) return v;
//
//    for (int i = LOG - 1; i >= 0; --i) {
//        if (parent[u][i] != parent[v][i]) {
//            u = parent[u][i];
//            v = parent[v][i];
//        }
//    }
//    return parent[u][0];
//}
//
//ll dist(int u, int v) {
//    int l = lca(u, v);
//    // 使用 dist_root (距离) 来计算
//    return dist_root[u] + dist_root[v] - 2LL * dist_root[l];
//}
//
//int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(0);
//
//    cin >> N >> K;
//    for (int i = 1; i < N; ++i) {
//        int x, y, t;
//        cin >> x >> y >> t;
//        g[x].push_back({ y, t });
//        g[y].push_back({ x, t });
//    }
//
//    routes.resize(K);
//    for (int i = 0; i < K; ++i) {
//        cin >> routes[i];
//    }
//
//    // 初始调用：节点1，父节点0，距离0，层数0
//    dfs_init(1, 0, 0, 0);
//
//    ll total = 0;
//    for (int i = 0; i < K - 1; ++i) {
//        total += dist(routes[i], routes[i + 1]);
//    }
//
//    for (int i = 0; i < K; ++i) {
//        ll ans;
//
//        if (i == 0) {
//            ans = total - dist(routes[0], routes[1]);
//        }
//        else if (i == K - 1) {
//            ans = total - dist(routes[K - 2], routes[K - 1]);
//        }
//        else {
//            ans = total - dist(routes[i - 1], routes[i]) - dist(routes[i], routes[i + 1]) + dist(routes[i - 1], routes[i + 1]);
//        }
//
//        cout << ans;
//        if (i < K - 1) cout << " ";
//    }
//    cout << endl;
//
//    return 0;
//}