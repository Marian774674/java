package com.czx.hot100.module1;

import java.util.HashSet;
import java.util.Set;

public class hot003Solution1 {
    public int longestConsecutive(int[] nums) {
        if(nums==null)return 0;
        int max=0;
        Set<Integer> set=new HashSet<>();
        for (int i = 0; i <nums.length; i++) {
            set.add(nums[i]);
        }
        for(Integer num:set){
            if(!set.contains(num-1)){
                int i=1;
                while(set.contains(num+=i))i++;
                i--;
                max=Math.max(max,i);
            }
        }
        return max;
    }
}
