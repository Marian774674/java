package com.czx.czx.GUI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataOutputStream;
import java.net.Socket;

public class RegisterGUI extends JFrame {

    private JTextField nicknameField;
    private Socket socket;

    public RegisterGUI() {
        setTitle("局域网聊天 - 登录");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1000, 500);
        setLocationRelativeTo(null); // 居中显示
        setResizable(false);

        // 创建面板
        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();

        // 昵称标签
        JLabel label = new JLabel("昵称：");
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.insets = new Insets(10, 10, 5, 5);
        panel.add(label, gbc);

        // 昵称输入框
        nicknameField = new JTextField(15);
        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.insets = new Insets(10, 5, 10, 10);
        panel.add(nicknameField, gbc);

        // 进入按钮
        JButton enterButton = new JButton("进入");
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.insets = new Insets(10, 10, 10, 5);
        panel.add(enterButton, gbc);

        // 取消按钮
        JButton cancelButton = new JButton("取消");
        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.insets = new Insets(10, 5, 10, 10);
        panel.add(cancelButton, gbc);

        // 按钮事件
        enterButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String nickname = nicknameField.getText().trim();
                if (nickname.isEmpty()) {
                    JOptionPane.showMessageDialog(RegisterGUI.this,
                            "请输入昵称！", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
//                请求一个socket与服务端端建立连接
                try {
                    login(nickname);
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
                // 启动主聊天窗口（示例）
                openChatWindow(nickname, socket);
                dispose(); // 关闭登录窗口
            }
        });

        cancelButton.addActionListener(e -> System.exit(0));

        add(panel);
    }
    public void login(String nickname) throws Exception {
        this.socket = new Socket(Constant.SERVER_IP, Constant.PORT);
        DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
        dos.writeInt(1);
        dos.writeUTF(nickname);
        dos.flush();
    }

    // 模拟打开主聊天窗口
    private void openChatWindow(String nickname,Socket socket) {
        // 这里你可以启动你的主聊天界面，传入 nickname
        JOptionPane.showMessageDialog(this,
                "欢迎，" + nickname + "！\n（此处应打开主聊天窗口）",
                "登录成功", JOptionPane.INFORMATION_MESSAGE);
        new ChatGUI(nickname,socket).setVisible(true);
        // 示例：new ChatMainFrame(nickname).setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                e.printStackTrace();
            }
            new RegisterGUI().setVisible(true);
        });
    }
}
