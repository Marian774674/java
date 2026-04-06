package com.czx;

import com.czx.mapper.UserMapper;
import com.czx.pojo.User;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.List;

@SpringBootTest//测试类
class SpringbootMybatisDemo1ApplicationTests {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void testFindAll(){
        List<User> all = userMapper.findAll();
        all.forEach(System.out::println);
    }
}
