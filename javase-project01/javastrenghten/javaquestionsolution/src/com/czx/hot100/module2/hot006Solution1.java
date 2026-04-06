package com.czx.hot100.module2;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class hot006Solution1 {

    public static List<List<Integer>> threeSum(int[] nums) {
        Arrays.sort(nums);
        List<List<Integer>> arrays=new ArrayList<>();
        for (int i = 0; i < nums.length; i++) {
            if(i-1>=0&&nums[i]==nums[i-1])continue;
            int l=i+1;
            int r=nums.length-1;
            while(l<r){
                int sum=nums[i]+nums[l]+nums[r];
                if(sum>0){
                    r--;
                    while(l<r&&nums[r]==nums[r+1])r--;
                }
                else if(sum<0){
                    l++;
                    while(l<r&&nums[l]==nums[l-1])l++;
                }
                else{
                    List<Integer> array = new ArrayList<>();array.add(nums[i]);array.add(nums[l]);array.add(nums[r]);arrays.add(array);
                    l++;
                    while(l<r&&nums[l]==nums[l-1])l++;
                }
            }
        }
        return arrays;
    }
}
