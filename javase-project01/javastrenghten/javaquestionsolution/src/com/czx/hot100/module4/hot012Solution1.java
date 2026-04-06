package com.czx.hot100.module4;


import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class hot012Solution1 {
    public static void main(String[] args) {
        String s = "ADOBECODEBANC", t = "ABC";
        System.out.println(minWindow1(s, t));
        System.out.println(minWindow2(s, t));
    }
    public static String minWindow1(String s, String t) {
        Map<Character, Integer> ori = new HashMap<Character, Integer>();
        Map<Character, Integer> cnt = new HashMap<Character, Integer>();
        int tLen = t.length();
        for (int i = 0; i < tLen; i++) {
            char c = t.charAt(i);
            ori.put(c, ori.getOrDefault(c, 0) + 1);
        }
        int l = 0, r = -1;
        int len = Integer.MAX_VALUE, ansL = -1, ansR = -1;
        int sLen = s.length();
        while (r < sLen) {
            ++r;
            if (r < sLen && ori.containsKey(s.charAt(r))) {
                cnt.put(s.charAt(r), cnt.getOrDefault(s.charAt(r), 0) + 1);
            }
            while (isEqueal(cnt,ori) && l <= r) {
                if (r - l + 1 < len) {
                    len = r - l + 1;
                    ansL = l;
                    ansR = l + len;
                }
                if (ori.containsKey(s.charAt(l))) {
                    cnt.put(s.charAt(l), cnt.getOrDefault(s.charAt(l), 0) - 1);
                }
                ++l;
            }
        }
        return ansL == -1 ? "" : s.substring(ansL, ansR);
    }

    public static String minWindow2(String s, String t) {
        Map<Character, Integer> win = new HashMap<>();
        Map<Character, Integer> tag = new HashMap<>();
        int len_s = s.length();
        int len_t = t.length();
        int len_max = Integer.MAX_VALUE, ans_l = -1, ans_r = -1;
        String ans;
        for (int i = 0; i < len_t; i++) {
            char c = t.charAt(i);
            tag.put(c, tag.getOrDefault(c, 0) + 1);
        }
        int l = 0, r = -1;
        while (r < len_s) {
            ++r;
            if (r < len_s && tag.containsKey(s.charAt(r))) {
                win.put(s.charAt(r), win.getOrDefault(s.charAt(r), 0) + 1);
            }
            while (isEqueal(win,tag) && l <= r) {
                if (r - l + 1 < len_max) {
                    len_max = r - l + 1;
                    ans_l = l;
                    ans_r = l+len_max;
                }
                if (tag.containsKey(s.charAt(l))) {
                    win.put(s.charAt(l), win.getOrDefault(s.charAt(l), 0) - 1);
                }
                ++l;
            }
        }
        ans = ans_l == -1 ? "" : s.substring(ans_l, ans_r);
        return ans;
    }

    public static Boolean isEqueal(Map<Character, Integer> win, Map<Character, Integer> tag) {
        Iterator<Map.Entry<Character, Integer>> it = tag.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<Character, Integer> entry = it.next();
            Character key = entry.getKey();
            Integer val = entry.getValue();
            if (win.getOrDefault(key, 0) < val) {
                return false;
            }
        }
        return true;
    }
}
