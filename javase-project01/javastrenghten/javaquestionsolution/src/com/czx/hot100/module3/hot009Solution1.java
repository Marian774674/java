package com.czx.hot100.module3;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class hot009Solution1 {
    public static List<Integer> findAnagrams(String s, String p) {
        List<Integer> list = new ArrayList<>();
        if(s.length()<p.length())return list;
        int[] count_s=new int[26];
        int[] count_p=new int[26];
        int len_p=p.length(),len_s=s.length();
        for (int i = 0; i < len_p; i++) {
            count_p[p.charAt(i)-'a']++;
            count_s[s.charAt(i)-'a']++;
        }
        if(Arrays.equals(count_p,count_s))list.add(0);
        for (int i = 0; i < len_s-len_p; i++) {
            int l=s.charAt(i)-'a',r=s.charAt(i+len_p)-'a';
            count_s[l]--;
            count_s[r]++;
            if(Arrays.equals(count_p,count_s))list.add(i+1);
        }

        return list;
    }
}
