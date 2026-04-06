package com.czx.czx.GUI;
import javax.swing.*;
import java.awt.*;
import java.net.Socket;

public class ChatGUI extends JFrame {

    private JTextArea messageDisplayArea; // 消息展示区
    private JTextField messageInputField; // 消息输入框
    private JList<String> onlineUserList; // 在线用户列表
    private DefaultListModel<String> userListModel;
    private String nickname;
    private Socket socket;

    public ChatGUI(String nickname, Socket socket) {
        this.nickname = nickname;
        this.socket = socket;
//        立即把客户端这个Socket管道交给一个独立的线程专门负责读取客户端Socket从服务端收到的在线人数更新消息或者群聊消息
        new ClientReaderThread(socket,this).start();
        setTitle("局域网聊天 - " + nickname);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(700, 500);
        setLocationRelativeTo(null);

        // === 1. 消息展示区域（顶部）===
        messageDisplayArea = new JTextArea();
        messageDisplayArea.setEditable(false);
        messageDisplayArea.setLineWrap(true);
        messageDisplayArea.setWrapStyleWord(true);
        JScrollPane messageScrollPane = new JScrollPane(messageDisplayArea);
        messageScrollPane.setBorder(BorderFactory.createTitledBorder("聊天消息"));

        // === 2. 底部：输入框 + 发送按钮 ===
        JPanel inputPanel = new JPanel(new BorderLayout());
        messageInputField = new JTextField();
        JButton sendButton = new JButton("发送");

        // 回车发送
        messageInputField.addActionListener(e -> sendMessage());
        sendButton.addActionListener(e -> sendMessage());

        inputPanel.add(messageInputField, BorderLayout.CENTER);
        inputPanel.add(sendButton, BorderLayout.EAST);
        inputPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

        // === 3. 右侧：在线用户列表 ===
        onlineUserList = new JList<>();
        onlineUserList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        JScrollPane userScrollPane = new JScrollPane(onlineUserList);
        userScrollPane.setBorder(BorderFactory.createTitledBorder("在线用户 (" + onlineUserList.getSize() + ")"));
        userScrollPane.setPreferredSize(new Dimension(150, 0)); // 宽度固定，高度自适应

        // === 4. 整体布局 ===
        setLayout(new BorderLayout());

        // 中间：消息显示 + 输入框
        JPanel centerPanel = new JPanel(new BorderLayout());
        centerPanel.add(messageScrollPane, BorderLayout.CENTER);
        centerPanel.add(inputPanel, BorderLayout.SOUTH);

        add(centerPanel, BorderLayout.CENTER);
        add(userScrollPane, BorderLayout.EAST);

        // 示例：模拟收到一条欢迎消息
        appendMessage("系统", "欢迎加入聊天室，" + nickname + "！");
    }

    // 发送消息逻辑
    private void sendMessage() {
        String text = messageInputField.getText().trim();
        if (!text.isEmpty()) {
            // 这里应该通过网络发送消息（例如 UDP/TCP）
            // 为演示，直接本地显示
            appendMessage("我", text);
            messageInputField.setText("");
        }
    }

    // 添加消息到聊天窗口
    public void appendMessage(String sender, String message) {
        messageDisplayArea.append("[" + sender + "] " + message + "\n");
        messageDisplayArea.setCaretPosition(messageDisplayArea.getDocument().getLength()); // 自动滚动到底部
    }

    // 更新在线用户列表（供网络模块调用）
    public void updateOnlineUsers(String[] onlinenameList) {
        onlineUserList.setListData(onlinenameList);
    }
}