package com.czx.hot100.module1;

import java.util.*;

public class hot002Solution1 {
    public List<List<String>> groupAnagrams(String[] strs) {
        List<List<String>> list=new ArrayList<>();
        if(strs.length==0)return list;
        Map<String,List<String>> map=new HashMap<>();
        for(int i=0;i<strs.length;i++){
            String temp=strs[i];
            char[] chars = temp.toCharArray();
            Arrays.sort(chars);
            String s=new String(chars);
            if(map.containsKey(s)){
                map.get(s).add(temp);
            }else{
                List<String> listMap=new ArrayList<>();
                listMap.add(temp);
                map.put(s,listMap);
            }
        }
        for(Map.Entry<String,List<String>> entry:map.entrySet()){
            list.add(entry.getValue());
        }
        return list;
    }
}
