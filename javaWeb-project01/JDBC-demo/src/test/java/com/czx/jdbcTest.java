package com.czx;

import org.junit.Test;

import java.sql.*;

public class jdbcTest {

    @Test
    public void testUpdate() throws Exception {
        Class.forName("com.mysql.cj.jdbc.Driver");
        String url="jdbc:mysql://localhost:3306/web01";
        String username="root";
        String password="8773";
        Connection connection = DriverManager.getConnection(url, username, password);
        Statement statement = connection.createStatement();
        int i = statement.executeUpdate("update user set age=250 where id=1");
        System.out.println("SQL语句执行完毕影响的记录数："+i);

        statement.close();
        connection.close();
    }
    @Test
    public void testQuery() throws Exception {
        Connection connection = null;
        ResultSet res=null;
        PreparedStatement statement = null;
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            String url="jdbc:mysql://localhost:3306/web01";
            String username="root";
            String password="8773";
            connection = DriverManager.getConnection(url, username, password);

            String sql="SELECT id,username,password,name,age FROM user WHERE username =? AND password =?";
            statement = connection.prepareStatement(sql);
            statement.setString(1,"daqiao");
            statement.setString(2,"123456");

            res=statement.executeQuery();
            while(res.next()){
                User user=new User(
                        res.getInt("id"),
                        res.getString("username"),
                        res.getString("password"),
                        res.getString("name"),
                        res.getInt("age")
                );
                System.out.println(user);
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            try {
                if (connection != null) connection.close();
                if (statement != null) statement.close();
                if (res != null) res.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
}
