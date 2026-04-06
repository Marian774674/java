package com.czx.hot100.module2;

public class hot004Solution1 {
    public void moveZeroes(int[] nums) {
        int right=0;
        int left=0;
        while(right<nums.length){
            if(nums[right]!=0){
                nums[left]=nums[right];
                nums[right]=0;
                left++;
            }
            right++;
        }
    }
}
