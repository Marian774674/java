package com.czx.hot100.module2;

public class hot005Solution1 {
    public int maxArea(int[] height) {
        int l=0;
        int r=height.length-1;
        int maxx=Math.min(height[l],height[r])*(r-l);
        while(l<r){
            if (height[r] > height[l]) l++;
            else r--;
            int v=Math.min(height[r],height[l])*(r-l);
            maxx=v>maxx?v:maxx;
        }
        return maxx;
    }
}
