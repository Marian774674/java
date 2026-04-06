package com.czx;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Scanner;

public class UDPClientDemo3 {
    public static void main(String[] args) throws Exception {
//        完成UDP通信多收多发：客户端开发
//        1.创建发送端对象
        DatagramSocket socket=new DatagramSocket();
//        2.创建数据包对象封装要发送的数据
//              参数一：数据
//              参数二：数据长度
//              参数三：指定发送的目标地址
//              参数四：指定发送的目标端口
        Scanner sc = new Scanner(System.in);
        while (true) {
            System.out.println("请输入要发送的内容：");
            String s=sc.nextLine();
//            如果输入的是退出命令则结束循环
            if(s.equals("exit")||s.equals("退出")){
                System.out.println("已退出");
                socket.close();
                break;
            }
            byte[] bytes = s.getBytes();
            DatagramPacket packet=new DatagramPacket(bytes,bytes.length, InetAddress.getLocalHost(),8080);

//        3.发送数据
            socket.send(packet);
        }
    }
}
