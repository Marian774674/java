package com.czx.hot100.module5;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class hot014Solution1 {
    public int[][] merge(int[][] intervals) {
        if(intervals.length==0)return new int[0][2];
        Arrays.sort(intervals, (o1,o2)->o1[0]-o2[0]);
        List<int[]> ans = new ArrayList<>();
        for (int i = 0; i < intervals.length; i++) {
            int left=intervals[i][0],right=intervals[i][1];
            if()
        }
    }
}
