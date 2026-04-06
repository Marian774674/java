package com.czx.controller;

import com.czx.service.Impl.UserServiceImpl;
import com.czx.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class UserController {
    @Autowired
    private UserService userService=new UserServiceImpl();
    @RequestMapping("/list")
    public List<User> list() throws Exception {
        List<User> userList = userService.packageUser();
        return userList;
    }
//    @RequestMapping("/list")
//    public List<User> list() throws Exception {
////        加载并读取user.txt文件
////        InputStream fis = new FileInputStream(new File("D:\\code\\springboot-Web-quickstart\\src\\main\\resources\\user.txt"));
//        InputStream is = this.getClass().getClassLoader().getResourceAsStream("user.txt");
//        ArrayList<String> lines = IoUtil.readLines(is, StandardCharsets.UTF_8, new ArrayList<>());
////        解析用户信息，封装成User对象到集合中去
//        List<User> userList = lines.stream().map(line -> {
//            String[] parts = line.split(",");
//            Integer id = Integer.parseInt(parts[0]);
//            String username = parts[1];
//            String password = parts[2];
//            String name = parts[3];
//            Integer age = Integer.parseInt(parts[4]);
//            LocalDateTime updateTime = LocalDateTime.parse(parts[5], DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));
//            return new User(id, username, password, name, age, updateTime);
//        }).collect(Collectors.toList());
////        返回数据
//        return userList;
//    }
}
