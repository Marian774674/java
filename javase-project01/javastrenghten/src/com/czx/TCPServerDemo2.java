package com.czx;

import java.net.ServerSocket;
import java.net.Socket;

public class TCPServerDemo2 {
    public static void main(String[] args) throws Exception {
        ServerSocket ss=new ServerSocket(10086);
        while (true) {
            Socket socket=ss.accept();
            System.out.println("一个客户端连接了"+socket.getInetAddress().getHostAddress());
            new ServerReader(socket).start();
        }
    }
}
