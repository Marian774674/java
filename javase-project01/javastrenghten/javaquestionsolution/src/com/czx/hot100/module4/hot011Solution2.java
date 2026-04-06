package com.czx.hot100.module4;


import java.util.ArrayDeque;

public class hot011Solution2 {
    public int[] maxSlidingWindow(int[] nums, int k) {
        int[] ans=new int[nums.length-k+1];
        ArrayDeque<Integer> deque = new ArrayDeque<>();
        for(int i=0;i<k;++i){
            while(!deque.isEmpty()&&nums[i]>=nums[deque.peekLast()]){
                deque.pollLast();
            }
            deque.offer(i);
        }
        ans[0]=nums[deque.peekFirst()];
        for(int i=k;i<nums.length;++i){
            while(!deque.isEmpty()&&nums[i]>=nums[deque.peekLast()]){
                deque.pollLast();
            }
            deque.offer(i);
            while(deque.peekFirst()<i-k+1){
                deque.pollFirst();
            }
            ans[i-k+1]=nums[deque.peekFirst()];
        }
        return ans;
    }
}
