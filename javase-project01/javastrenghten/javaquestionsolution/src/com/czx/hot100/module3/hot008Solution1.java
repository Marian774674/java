package com.czx.hot100.module3;

import java.util.ArrayList;
import java.util.List;

public class hot008Solution1 {
    public static int lengthOfLongestSubstring(String s) {
        List<Character> list = new ArrayList<>();
        int maxx=0;
        for(int i=0;i<s.length();i++){
            if(!list.contains(s.charAt(i))){
                list.add(s.charAt(i));
            }
            else{
                if(maxx<list.size())maxx=list.size();
                int index=list.indexOf(s.charAt(i));
                list.subList(0,index+1).clear();
                list.add(s.charAt(i));
            }
        }
        if(maxx<list.size())maxx=list.size();
        return maxx;
    }
}
