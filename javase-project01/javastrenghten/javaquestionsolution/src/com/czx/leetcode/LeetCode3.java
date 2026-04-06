package com.czx.leetcode;

import org.junit.Test;

public class LeetCode3 {
    public static void main(String[] args) {
        System.out.println(new Solution().convert("PAYPALISHIRING", 4));
    }
    public static class Solution {
        public String convert(String s, int numRows) {
            if(numRows==1)return s;
            String res = "";
            for (int h = 1; h<=numRows; h++) {
                int gap=2*(numRows-1);
                for(int i=h-1;i<s.length();i+=gap){
                    res+=s.charAt(i);
                    System.out.println( i);
                    if(h!=1&&h!=numRows){
                        int idx=i+2*(numRows-h);
                        if(idx<s.length()){
                            System.out.println(idx);
                            res+=s.charAt(idx);
                        }
                    }
                }
            }
            if(res.equals("PINALSIGYAHRPI")) System.out.println("ok");
            else System.out.println("no");
            return res;
        }
    }
}
