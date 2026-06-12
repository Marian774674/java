//#include <iostream>
//#include <string>
//#include <queue>
//#include <unordered_set>   // 集合（去重用）
//using namespace std;
//
//int main() {
//    string start, target;
//    cin >> start >> target;
//
//    // 每个元素是：{当前状态字符串, 已经走了几步}
//    queue<pair<string, int>> q;
//    q.push({ start, 0 });  // 先把初始状态放进去，步数是0
//
//    // 集合：记录已经访问过的状态（避免重复走）
//    unordered_set<string> visited;
//    visited.insert(start);
//
//    // 只要队列不空，就继续找
//    while (!q.empty()) {
//        string state = q.front().first;
//        int steps = q.front().second;
//        q.pop();
//
//        if (state == target) {
//            cout << steps << endl;
//            return 0;
//        }
//
//        // 找到空杯子'*'的位置
//        int pos = state.find('*');
//        int n = state.length();
//
//        // 尝试让空杯子和左右距离1、2、3的青蛙交换
//        int deltas[] = { -3, -2, -1, 1, 2, 3 };
//
//        for (int i = 0; i < 6; i++) {
//            int new_pos = pos + deltas[i];  // 新位置
//
//            // 检查新位置是否合法（不能超出边界）
//            if (new_pos >= 0 && new_pos < n) {
//                // 交换空杯子和青蛙
//                string new_state = state;  // 复制一份
//                swap(new_state[pos], new_state[new_pos]);
//
//                // 如果这个新状态之前没访问过
//                if (visited.find(new_state) == visited.end()) {
//                    visited.insert(new_state);
//                    q.push({ new_state, steps + 1 });
//                }
//            }
//        }
//    }
//
//    return 0;
//}