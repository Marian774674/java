package com.czx.javatbasicest;

import java.util.Map;

public class Test03 {
//    public static void main(String[] args) {
//        String info="10001,张无忌,男,2023-07-22 11:11:12,东湖-黄鹤楼#10002,赵敏,女,2023-07-22 09:11:21,黄鹤楼-归元禅寺#10003,周芷若,女,2023-07-22 04:11:21,木兰文化区-东湖#10004,小昭,女,2023-07-22 08:11:21,东湖#10005,灭绝,女,2023-07-22 17:11:21,归元禅寺";
//        String[] sp = info.split("#");
//        List<Student> list=new ArrayList<>();
//        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
//        for(String s:sp){
//            String[] sp1 = s.split(",");
////                将sp1[3]转换为LocalDateTime
//            LocalDateTime time = LocalDateTime.parse(sp1[3],dtf);
//            list.add(new Student(Integer.parseInt(sp1[0]),sp1[1],sp1[2],time,sp1[4]));
//        }
//        Map<String,Integer> map=new HashMap<>();
//        for(Student s:list){
//            String[] split = s.getSelectAddress().split("-");
//            for(String s1:split){
//                if(map.containsKey(s1)){
//                    map.put(s1,map.get(s1)+1);
//                }else{
//                    map.put(s1,1);
//                }
//            }
//        }
//        parseMap(map);
////        用stream找到去的人数最多的景点
//        String address = map.entrySet().stream().max((o1, o2) -> o1.getValue() - o2.getValue()).get().getKey();
//        System.out.println("去的人数最多的景点是：" + address);
//        list.stream().filter(s1->s1.getSelectAddress().contains(address)).forEach(s -> System.out.println(s.getName()+"没有去"+address));
//    }

    private static void parseMap(Map<String, Integer> map) {
        for(Map.Entry<String,Integer> entry:map.entrySet()){
            System.out.println(entry.getKey()+":"+entry.getValue());
        }
    }
}
