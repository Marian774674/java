//#include <iostream>
//#include <math.h>  // 包含 sqrt()、arctan2() 函数
//
//int main() {
//    // 目标点坐标
//    int x = 233;
//    int y = 666;
//
//    // 1. 计算水平移动距离（即目标点到原点的半径 r）
//    double r = sqrt(x * x + y * y);
//
//    // 2. 计算圆心角 θ（弧度制，arctan2(y, x) 直接返回正确角度）
//    double a = atan2(y, x);  // 范围 (-π, π]，此处为正角
//
//    // 3. 计算圆周最短弧长（r * θ，θ已为最短夹角）
//    double l = r * a;
//
//    // 4. 计算总移动距离（水平距离 + 弧长）
//    double dist = r + l;
//
//    int ans = (int)dist;
//    printf("%d\n", ans);
//
//    return 0;
//}
