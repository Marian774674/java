//#include<iostream>
//#include<vector>
//using namespace std;
//
////¶ş·ÖËÑË÷
//int binarySearch(vector<int>&a,int l,int r,int target) {
//    if (l > r)return -1;
//    int mid = (l + r) / 2;
//    if (a[mid] == target)
//        return mid;
//    if (a[mid] < target)
//        return binarySearch(a, mid + 1, r, target);
//    if (a[mid] > target)
//        return binarySearch(a, l, mid - 1, target);
//}
////
//
//int main() {
//    vector<int>a{ 1,2,3,4,5,6,7,8,9 };
//    int target;
//    while (cin >> target) {
//        cout << binarySearch(a, 0, a.size() - 1, target) << endl;;
//    }
//
//    return 0;
//}