package com.czx.hot100.module1;

import java.util.HashMap;
import java.util.Map;

public class hot001Solution2 {
    public int[] twoSum(int[] nums, int target) {
        int[] ans=new int[2];
        Map<Integer,Integer> map=new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            map.put(nums[i],i);
        }
        for(int i=0;i<nums.length;i++){
            int temp=target-nums[i];
            if(map.containsKey(temp)&&i!=map.get(temp)){
                ans[0]=map.get(temp);
                ans[1]=i;
                return ans;
            }
        }
        return ans;
    }
}
