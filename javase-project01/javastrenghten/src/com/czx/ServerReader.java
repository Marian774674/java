package com.czx;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;

public class ServerReader extends  Thread{
    private Socket socket;
    public ServerReader(Socket socket) {
        this.socket=socket;
    }
    @Override
    public void run() {
        try {
            InputStream is =socket.getInputStream();
            DataInputStream dis=new DataInputStream(is);
            while(true){
                String msg=dis.readUTF();
                System.out.println("服务端收到："+msg);
                System.out.println("客户端IP："+socket.getInetAddress().getHostAddress());
                System.out.println("客户端端口："+socket.getPort());
                System.out.println("--------------------------------------------------------");
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("一个客户端退出了" + socket.getInetAddress().getHostAddress());
        }
    }
}
