package com.czx.hot100.module4;


import java.util.PriorityQueue;

public class hot011Solution1 {
    public int[] maxSlidingWindow(int[] nums, int k) {
        int[] ans = new int[nums.length - k + 1];
        PriorityQueue<int[]> queue = new PriorityQueue<>((o1, o2) -> o2[0] - o1[0]);
        for (int i = 0; i < k; ++i) {
            queue.offer(new int[] { nums[i], i });
        }
        ans[0] = queue.peek()[0];
        for (int i = k; i < nums.length; ++i) {
            queue.offer(new int[] { nums[i], i });
            while (queue.peek()[1] <= i - k){
                queue.poll();
            }
            ans[i - k + 1] = queue.peek()[0];
        }
        return ans;
    }
}
