package com.czx.leetcode;

import java.util.ArrayList;
import java.util.List;

public class LeetCode2 {
    public static void main(String[] args) {
        System.out.println(new Solution().longestPalindrome("babad"));
    }
    public static class Solution {
        public String longestPalindrome(String s) {
            String maxx="";
            for (int i = 0; i < s.length(); i++) {
                String s1=s.substring(i,s.length());
                while (true) {
                    if(!isPalindrome(s1)){
                        if(s1.length()==1)break;
                        s1=s1.substring(0,s1.length()-1);
                    }else{
                        break;
                    }
                }
                if(s1.length()>maxx.length())maxx=s1;
            }
            return maxx;
        }
//        定义一个方法判断是否为回文数
        public static boolean isPalindrome(String s){
            for (int i = 0; i < s.length()/2; i++) {
                if(s.charAt(i)!=s.charAt(s.length()-i-1))return false;
            }
            return true;
        }
    }
}
