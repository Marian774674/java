package com.czx.hot100.module1;

import java.util.*;

public class hot001Solution1 {
    public class Pair{
        public int value;
        public int index;

        public int getIndex() {
            return index;
        }

        public int getValue() {
            return value;
        }
        public Pair(int value, int index) {
            this.value = value;
            this.index = index;
        }
    }
    public int[] twoSum(int[] nums, int target) {
        int[] ans=new int[2];
        List<Pair> list=new ArrayList<>();
        for(int i=0;i<nums.length;i++){
            list.add(new Pair(nums[i], i));
        }
        list.sort((o1, o2)->o1.getValue()-o2.getValue());
        int l=0;
        int right=list.size()-1;
        while(l<right){
            int sum=list.get(l).getValue()+ list.get(right).getValue();
            while(sum> target&&l<right){
                right--;
                sum= list.get(l).getValue() + list.get(right).getValue();
            }
            while(sum< target&&l<right){
                l++;
                sum= list.get(l).getValue() + list.get(right).getValue();
            }
            if(sum==target){
                ans=new int[]{list.get(l).getIndex(), list.get(right).getIndex()};
                return ans;
            }
        }
        return ans;
    }
}
