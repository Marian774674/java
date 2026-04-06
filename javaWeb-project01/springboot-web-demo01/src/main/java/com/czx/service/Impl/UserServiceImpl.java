package com.czx.service.Impl;

import com.czx.dao.Impl.UserDaoImpl;
import com.czx.dao.UserDao;
import com.czx.controller.User;
import com.czx.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.stream.Collectors;

//@Component
@Service
public class UserServiceImpl implements UserService {
    @Autowired
    private UserDao userDao=new UserDaoImpl();
    @Override
    public List<User> packageUser() {
        List<String> lines = userDao.reader();
        List<User> userList = lines.stream().map(line -> {
            String[] parts = line.split(",");
            Integer id = Integer.parseInt(parts[0]);
            String username = parts[1];
            String password = parts[2];
            String name = parts[3];
            Integer age = Integer.parseInt(parts[4]);
            LocalDateTime updateTime = LocalDateTime.parse(parts[5], DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));
            return new User(id, username, password, name, age, updateTime);
        }).toList();
        return userList;
    }
}
