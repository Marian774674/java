#include <iostream>
#include<vector>
using namespace std;

// 定义最大节点数，题目说 n <= 10^5，所以开大一点
const int MAXN = 100005;
// LOG 用于 LCA 的倍增算法，2^17 > 100000，足够用了
const int LOG = 17;

// g[u] 存储 u 的邻居。pair<邻居节点, 边的编号>
vector<pair<int, int>> g[MAXN];

// depth[u]: 节点 u 的深度
// parent[u][i]: 节点 u 向上跳 2^i 步到达的祖先节点（用于 LCA）
int depth[MAXN];
int parent[MAXN][LOG];

// diff[u]: 差分数组。用来记录节点 u 上的“流量”变化
int diff[MAXN];

// edgeCount[i]: 记录第 i 条边被经过了多少次
int edgeCount[MAXN];

// ---------------- 第一步：预处理深度和祖先 (DFS) ----------------
// u: 当前节点, p: 父节点, d: 当前深度
void dfs_init(int u, int p, int d) {
    depth[u] = d;           // 记录深度
    parent[u][0] = p;       // 记录直接父节点 (向上跳 2^0=1 步)

    // 预处理倍增数组：parent[u][i] = parent[ parent[u][i-1] ][i-1]
    // 意思是：u 向上跳 2^i 步，等于先跳 2^(i-1) 步，再跳 2^(i-1) 步
    for (int i = 1; i < LOG; i++) {
        parent[u][i] = parent[parent[u][i - 1]][i - 1];
    }

    // 遍历 u 的所有邻居
    for (auto& edge : g[u]) {
        int v = edge.first; // 邻居节点
        // 如果邻居不是父节点（说明是子节点），则递归处理
        if (v != p) {
            dfs_init(v, u, d + 1);
        }
    }
}

// ---------------- 第二步：求 LCA (最近公共祖先) ----------------
int lca(int u, int v) {
    // 1. 让 u 和 v 处于同一深度
    if (depth[u] < depth[v]) swap(u, v); // 保证 u 更深

    // 计算深度差，把 u 向上提，直到和 v 一样深
    int diff_depth = depth[u] - depth[v];
    for (int i = 0; i < LOG; i++) {
        // 如果深度差的二进制第 i 位是 1，说明 u 需要向上跳 2^i 步
        if (diff_depth & (1 << i)) {
            u = parent[u][i];
        }
    }

    // 2. 如果此时 u == v，说明 v 就是 u 的祖先，直接返回
    if (u == v) return v;

    // 3. 如果还没相遇，让 u 和 v 一起向上跳
    // 从大步长开始试，只要跳完不重合就跳，直到跳到 LCA 的下一层
    for (int i = LOG - 1; i >= 0; i--) {
        if (parent[u][i] != parent[v][i]) {
            u = parent[u][i];
            v = parent[v][i];
        }
    }
    // 最后 u 和 v 的父节点就是 LCA
    return parent[u][0];
}

// ---------------- 第三步：DFS 统计边的覆盖次数 (差分核心) ----------------
// u: 当前节点, p: 父节点
void dfs_count(int u, int p) {
    for (auto& edge : g[u]) {
        int v = edge.first;   // 子节点
        int eid = edge.second; // 边的编号

        if (v != p) {
            // 先递归处理子节点
            dfs_count(v, u);

            // 【核心逻辑】：
            // 子节点 v 处理完后，diff[v] 里存储的就是“经过边 (u, v) 的路径数量”
            // 为什么？因为我们在起点 +1，终点 +1，LCA -2。
            // 子树里的 +1 会向上传导，-2 会抵消掉多余的传导。
            // 所以 v 传上来的值，就是穿过 (u, v) 这条边的流量。
            diff[u] += diff[v];

            // 记录这条边被经过的次数
            edgeCount[eid] = diff[v];
        }
    }
}

int main() {
    // 加速输入输出
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // 1. 读入树的结构
    for (int i = 1; i < n; i++) {
        int x, y;
        cin >> x >> y;
        // 无向图，存两次。同时记录边的编号 i
        g[x].push_back({ y, i });
        g[y].push_back({ x, i });
    }

    // 2. 预处理 LCA 需要的信息（从节点 1 开始 DFS）
    dfs_init(1, 0, 0);

    // 3. 处理 m 个询问，进行树上差分打标记
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        int l = lca(a, b); // 找到 a 和 b 的最近公共祖先

        // 打标记：
        diff[a]++;   // 起点流量 +1
        diff[b]++;   // 终点流量 +1
        diff[l] -= 2; // 公共祖先处流量 -2 (防止流量溢出到 LCA 上方)
    }

    // 4. 再次 DFS，自底向上统计每条边被经过的次数
    dfs_count(1, 0);

    // 5. 找答案
    // 题目要求：删掉一条边，让所有 m 条路径都不连通。
    // 这意味着这条边必须被所有 m 条路径经过 (edgeCount == m)。
    // 如果有多个，输出编号最大的。
    int ans = -1;
    for (int i = 1; i < n; i++) {
        if (edgeCount[i] == m) {
            ans = i; // 因为是从 1 到 n-1 遍历，后面的会覆盖前面的，自然就是编号最大的
        }
    }

    cout << ans << endl;

    return 0;
}