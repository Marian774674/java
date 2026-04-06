package com.czx.czx;

import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class Server {
//    定义一个集合存储多个Socket(Map)集合,键是管道，值是用户名称
    public static final Map<Socket, String> onlineSocket = new HashMap<>();
    public static void main(String[] args){
        System.out.println("服务器启动...");
        try {
//            注册端口
            ServerSocket serverSocket = new ServerSocket(Constant.PORT);
//            主程序负责接收客户端连接请求
            while (true) {
                System.out.println("等待客户端连接...");
                Socket socket = serverSocket.accept();
                new ServerReaderThread(socket).start();
                System.out.println("一个客户端连接成功...");
            }
        } catch (Exception e) {
           e.printStackTrace();
        }
    }
}
