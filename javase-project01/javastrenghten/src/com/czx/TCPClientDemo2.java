package com.czx;

import java.io.DataOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Scanner;

public class TCPClientDemo2 {
    public static void main(String[] args) throws Exception {
//        实现TCP通信下多发多收：客户端开发
//        1.创建客户端对象,请求与服务器建立连接
        Socket socket=new Socket("192.168.235.30",10086);
//        2.获取输出流，发送数据
        OutputStream os=socket.getOutputStream();
        DataOutputStream dos=new DataOutputStream(os);
        Scanner sc = new Scanner(System.in);
        while (true) {
            System.out.println("请输入要发送的内容：");
            String s = sc.nextLine();
            if(s.equals("exit")){
                System.out.println("已退出");
                dos.close();
                socket.close();
                break;
            }
            dos.writeUTF(s);
            dos.flush();
        }
    }
}
