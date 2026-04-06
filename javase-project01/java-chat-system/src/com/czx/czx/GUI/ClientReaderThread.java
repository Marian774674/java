package com.czx.czx.GUI;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;


public class ClientReaderThread extends  Thread{
    private Socket socket;
    private ChatGUI chatGUI;
    public ClientReaderThread(Socket socket, ChatGUI chatGUI) {
        this.socket=socket;
        this.chatGUI=chatGUI;
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
//                      服务端发来在线人数更新消息
                        updateClientOnlineUserList(dis);
                        break;
                    case 2:
//                        服务端发来群聊消息
                        break;
                    case 3:
//                    私聊消息，接收消息数据，转发给指定用户
                        break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void updateClientOnlineUserList(DataInputStream dis) throws IOException {
//        读取在线人数
        int count = dis.readInt();
        String[] onlinenameList=new String[count];
//        循环控制读取多少个用户
        for (int i = 0; i < count; i++) {
            String nickname= dis.readUTF();
            onlinenameList[i]=nickname;
        }
//        更新到窗口
        chatGUI.updateOnlineUsers(onlinenameList);
     }
}
