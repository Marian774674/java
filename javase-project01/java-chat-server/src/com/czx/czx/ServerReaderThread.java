package com.czx.czx;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Collection;

import static com.czx.czx.Server.onlineSocket;

public class ServerReaderThread extends  Thread{
    private Socket socket;
    public ServerReaderThread(Socket socket) {
        this.socket=socket;
    }
    @Override
    public void run() {
        try {
//            客户端发来的信息可能是各种类型的1.登录消息含昵称2.群聊消息3.私聊消息
            DataInputStream dis = new DataInputStream(socket.getInputStream());
            while (true) {
                int type=dis.readInt();
                switch ( type){
                    case 1:
    //                    登录消息，接收昵称数据，更新在线用户列表
                        String nickname = dis.readUTF();
                        onlineSocket.put(socket,nickname);
                        UpdateOnlineUsersList();
                        break;
                    case 2:
                        String message = dis.readUTF();
                        seedMessageToAll(message);
    //                    群聊消息，接收消息数据，转发给所有在线用户
                        break;
                    case 3:
    //                    私聊消息，接收消息数据，转发给指定用户
                        break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("一个客户端退出了" + socket.getInetAddress().getHostAddress());
            Server.onlineSocket.remove(socket);
            UpdateOnlineUsersList();//更新在线用户列表
        }
    }

    private void seedMessageToAll(String message) {
        StringBuilder sb = new StringBuilder();
        String name=onlineSocket.get(socket);
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss EE a");
        String time = dtf.format(now);
        StringBuilder msgResult = sb.append(name).append(" ").append(time).append("\r\n")
                .append(message).append("\r\n");
        for (Socket socket : onlineSocket.keySet()) {
            try {
                DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
                dos.writeInt(2);
                dos.writeUTF(msgResult.toString());
                dos.flush();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    private void UpdateOnlineUsersList() {
        Collection<String> onlineUsers = Server.onlineSocket.values();
        for (Socket socket : onlineSocket.keySet()) {
            try {
                DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
                dos.writeInt(1);
                dos.writeInt(onlineUsers.size());
                for (String onlineUser : onlineUsers) {
                    dos.writeUTF(onlineUser);
                }
                dos.flush();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

}
