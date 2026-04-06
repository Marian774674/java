package com.czx.hot100.module2;

public class hot007Solution1 {

    public static int trap(int[] height) {
        int sum=0;
        int l=0,r=height.length-1;
        int leftMax=0,rightMax=0;
        while(l<r){
            leftMax=Math.max(leftMax,height[l]);
            rightMax=Math.max(rightMax,height[r]);
            if(leftMax<rightMax){
                sum+=leftMax-height[l];
                l++;
            }else{
                sum+=rightMax-height[r];
                r--;
            }
        }
        return sum;
    }
}
