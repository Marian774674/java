//#include<iostream>
//#include<cstring>
//#include<queue>
//using namespace std;
////双向图要开原来的两倍（我这里是用数组来模拟的，不不必纠结这个，看思路）
//const int N = 200010;
//int to[N], nex[N], head[N], cnt;
////记录每个点的度数
//int d[N];
////数组模拟图的模板，不知道的可以先去学习一下，也可以用其他方式的建图
//void add(int a, int b) {
//    to[cnt] = b, nex[cnt] = head[a], head[a] = cnt++;
//}
//int main()
//{
//    //建图模拟，初始化
//    memset(head, -1, sizeof head);
//    int n, a, b;
//    cin >> n;
//    for (int i = 0; i < n; i++)
//    {
//        cin >> a >> b;
//        //入度和出度都要记录
//        d[a]++, d[b]++;
//        //双向边
//        add(a, b), add(b, a);
//    }
//    //队列存储，度为1的点，好比宽搜
//    queue<int> q;
//    for (int i = 1; i <= n; i++)
//    {
//        if (d[i] == 1) q.push(i);
//    }
//    while (!q.empty())
//    {
//        int x = q.front();
//        q.pop();
//        for (int i = head[x]; i != -1; i = nex[i])
//        {
//            int j = to[i];
//            //将这条边减去，如果剪完后度数变成了1，就说明他不在环内，入队在下一次减去
//            d[j]--;
//            if (d[j] == 1) q.push(j);
//        }
//    }
//    //将所有点从小到大遍历，当你的度数在经过剪枝之后还是大于1，那么说明你在环内
//    for (int i = 1; i <= n; i++)
//    {
//        if (d[i] > 1) cout << i << " ";
//    }
//    return 0;
//}
