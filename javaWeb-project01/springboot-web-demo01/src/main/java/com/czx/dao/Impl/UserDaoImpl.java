package com.czx.dao.Impl;

import cn.hutool.core.io.IoUtil;
import com.czx.dao.UserDao;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Repository;

import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

@Repository
public class UserDaoImpl implements UserDao {

    @Override
    public List<String> reader() {
        InputStream is = this.getClass().getClassLoader().getResourceAsStream("user.txt");
        List<String> lines = IoUtil.readLines(is, StandardCharsets.UTF_8, new ArrayList<>());
        return lines;
    }
}
