package com.czx;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServerDemo1 {
    public static void main(String[] args) throws Exception {
//        实现TCP通信下多发多收：服务端开发
//        创建服务端ServerSocket对象，绑定端口号，监听客户端连接
            ServerSocket ss=new ServerSocket(10086);
//          等待客户端连接
            Socket socket=ss.accept();
//          获取输入流，接收数据
            InputStream is=socket.getInputStream();
            DataInputStream dis=new DataInputStream(is);
        while (true) {
            String data = dis.readUTF();
            System.out.println("服务端收到了:"+data);
            System.out.println(socket.getInetAddress().getHostAddress());
            System.out.println(socket.getPort());
            System.out.println("--------------------------------------------------------");
        }
    }
}
