package com.czx.hot100.module5;

public class hot013Solution1 {
    public int maxSubArray(int[] nums) {
        int max=0,sum=0;
        for (int num:nums) {
            sum=Math.max(num,sum+num);
            max=Math.max(sum,max);
        }
        return max;
    }
}
