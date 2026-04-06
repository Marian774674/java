package com.czx;

import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class UDPServerDemo3 {
    public static void main(String[] args) throws Exception {
//        完成UDP通信多收多发：服务端开发
//        1.创建数据接收对象,注册端口
        DatagramSocket socket=new DatagramSocket(8080);
//        2.创建一个数据包对象负责接收数据
        byte[] buf = new byte[1024*64];
        DatagramPacket packet=new DatagramPacket(buf,buf.length);
//        3.接收数据,封装到数据包对象中
        while (true) {
            socket.receive(packet);
//        4.获取数据
            int len=packet.getLength();
            String data = new String(buf,0,len);
            System.out.println("服务端收到了:"+data);
//        获取对方IP和端口
            String ip=packet.getAddress().getHostAddress();
            int port=packet.getPort();
            System.out.println("对方IP:"+ip+" 对方端口:"+port);
            System.out.println("--------------------------------------------------------");
        }
    }
}
