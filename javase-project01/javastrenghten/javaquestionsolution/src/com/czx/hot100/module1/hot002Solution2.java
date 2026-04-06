package com.czx.hot100.module1;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class hot002Solution2 {
    public List<List<String>> groupAnagrams(String[] strs) {
        List<List<String>> list=new ArrayList<>();
        Map<String, List<String>> map = new HashMap<>();
        for (String str : strs) {
            int[] count=new int[26];
            for(int i=0;i<str.length();i++){
                count[str.charAt(i)-'a']++;
            }
            StringBuilder sb=new StringBuilder();
            for(int i=0;i<26;i++){
                sb.append((char)count[i]);
            }
            String key=sb.toString();
            if(map.containsKey(key)){
                map.get(key).add(str);
            }else{
                List<String> listMap=new ArrayList<>();
                listMap.add(str);
                map.put(key,listMap);
            }
        }
        for(Map.Entry<String,List<String>> entry:map.entrySet()){
            list.add(entry.getValue());
        }
        return list;
    }
}